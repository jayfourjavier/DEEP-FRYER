#include <Arduino.h>
#include <WiFi.h>
#include "esp_system.h"
#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"
#include "Adafruit_MAX31865.h"
#include "soc/soc.h"          //disable brownout problems
#include "soc/rtc_cntl_reg.h" //disable brownout problems

#include "Relay.h"
#include "Buzzer.h"
#include "LimitSwitch.h"
#include "webpage.h"

// ==================================================================================================================================================
// WIFI ACCESS POINT
// ==================================================================================================================================================

const char *AP_SSID = "SMART_DEEP_FRYER";
const char *AP_PASSWORD = "14789632580";

IPAddress AP_IP(192, 168, 4, 1);
IPAddress AP_GATEWAY(192, 168, 4, 1);
IPAddress AP_SUBNET(255, 255, 255, 0);

// ==================================================================================================================================================
// HARDWARE PINS
// ==================================================================================================================================================

#define PT100_CS_PIN 5

#define HEATER_RELAY_PIN 33
#define UPWARD_RELAY_PIN 26
#define DOWNWARD_RELAY_PIN 25

#define BUZZER_PIN 4

// CHANGE THESE TO YOUR ACTUAL LIMIT SWITCH PINS
#define UPPER_LIMIT_PIN 16
#define LOWER_LIMIT_PIN 17

#define LIMIT_ACTIVE LOW

// Set these above the normal mechanical travel time of the basket.
#define BASKET_LOWERING_TIMEOUT_SECONDS 60UL // LOWERING BASKET TIMEOUT IN SECONDS
#define BASKET_RAISING_TIMEOUT_SECONDS 60UL  // RAISING BASKET TIMEOUT IN SECONDS
#define SECONDS_TO_MILLIS(seconds) ((unsigned long)(seconds) * 1000UL)
#define BASKET_LOWERING_TIMEOUT_MS SECONDS_TO_MILLIS(BASKET_LOWERING_TIMEOUT_SECONDS)
#define BASKET_RAISING_TIMEOUT_MS SECONDS_TO_MILLIS(BASKET_RAISING_TIMEOUT_SECONDS)

// ==================================================================================================================================================
// PT100
// ==================================================================================================================================================

#define RREF 430.0     // Reference resistor value in ohms
#define RNOMINAL 100.0 // Nominal resistance of PT100 at 0°C in ohms

Adafruit_MAX31865 pt100(PT100_CS_PIN);

// ==================================================================================================================================================
// HARDWARE OBJECTS
// ==================================================================================================================================================

Relay heaterRelay(HEATER_RELAY_PIN, "Heater Relay", true);
Relay upwardRelay(UPWARD_RELAY_PIN, "Upward Relay", true);
Relay downwardRelay(DOWNWARD_RELAY_PIN, "Downward Relay", true);

Buzzer buzzer(BUZZER_PIN, "Buzzer", false);

// ==================================================================================================================================================
// WEB SERVER
// ==================================================================================================================================================

AsyncWebServer server(80); // HTTP server on port 80
AsyncWebSocket ws("/ws");  // WebSocket endpoint at /ws

// ==================================================================================================================================================
// FRYER STATES
// ==================================================================================================================================================

enum FryerState
{
    IDLE,       // Waiting for user to select product and start
    PREHEATING, // Heating to target temperature
    READY,      // Ready to start frying (temperature reached)
    LOWERING,   // Lowering basket into oil
    FRYING,     // Frying in progress
    RAISING,    // Raising basket out of oil
    FAULT       // Fault state (temperature sensor fault, limit switch fault, etc.)
};

FryerState fryerState = IDLE;

// Pending actions and faults
bool pendingStart = false;
String lastFault = "";
// pending action: raise after playing DONE pattern
bool pendingRaiseAfterDone = false;
// pending actions following PREMOVE tone
bool pendingLowerAfterPremove = false;
bool pendingRaiseAfterPremove = false;

// ==================================================================================================================================================
// RECIPE
// ==================================================================================================================================================

String selectedProduct = "";
String selectedIcon = "";

float targetTemperature = 120.0;

int selectedDuration = 0;
int remainingTime = 0;

// ==================================================================================================================================================
// TEMPERATURE
// ==================================================================================================================================================

float currentTemperature = 120.0;
bool temperatureValid = false;

const float IDLE_TEMPERATURE = 120.0; // temperature to maintain when idle (to keep oil warm)
const float HEATER_ON_OFFSET = 2.0;   // heater turns on below target by this amount

// ==================================================================================================================================================
// OVERTEMPERATURE / RUNAWAY SAFEGUARDS
// ==================================================================================================================================================

const float OVERTEMP_OFFSET = 20.0;            // user requested: target +20°C triggers over-temp
const float ABSOLUTE_MAX_TEMP = 240.0;         // absolute hard limit
const float RUNAWAY_DELTA_C = 5.0;             // rise in degrees considered runaway
const unsigned long RUNAWAY_WINDOW_MS = 10000; // window to measure rise (10s)

// tracking for runaway detection
unsigned long tempWindowStartMillis = 0;
float tempWindowStartTemp = 0.0;

// ==================================================================================================================================================
// TIMERS
// ==================================================================================================================================================

unsigned long stateStartMillis = 0;
unsigned long lastTemperatureMillis = 0;
unsigned long lastStatusMillis = 0;
unsigned long lastFrySecondMillis = 0;

// ==================================================================================================================================================
// FUNCTION DECLARATIONS
// ==================================================================================================================================================

const char *getStateName(FryerState state);
void setFryerState(FryerState newState);

void broadcastStatus();
void sendStatus(AsyncWebSocketClient *client);

void saveRecipe(const String &product, const String &icon, float temperature, int duration);
void clearRecipe();

void startFrying();
void stopFryer();

void readTemperature();
void updateHeater();

void updateBasket();
void updateFryer();

bool upperLimitReached();
bool lowerLimitReached();

void stopBasketMotor();

void handleWebSocketMessage(AsyncWebSocketClient *client, uint8_t *data, size_t len);
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

// ==================================================================================================================================================
// STATE NAME
// ==================================================================================================================================================

const char *getStateName(FryerState state)
{
    switch (state)
    {
    case IDLE:
        return "IDLE";

    case PREHEATING:
        return "PREHEATING";

    case READY:
        return "READY";

    case LOWERING:
        return "LOWERING";

    case FRYING:
        return "FRYING";

    case RAISING:
        return "RAISING";

    case FAULT:
        return "FAULT";
    }

    return "IDLE";
}

// ==================================================================================================================================================
// SET FRYER STATE
// ==================================================================================================================================================

void setFryerState(FryerState newState)
{
    FryerState oldState = fryerState;

    if (oldState == newState)
    {
        return;
    }

    fryerState = newState;
    stateStartMillis = millis();

    Serial.printf("[STATE] %s -> %s\n", getStateName(oldState), getStateName(newState));

    switch (newState)
    {
    case IDLE:
        heaterRelay.off();
        stopBasketMotor();
        buzzer.mode(BUZZER_DONE);
        break;

    case PREHEATING:
        stopBasketMotor();
        buzzer.mode(BUZZER_BEEP);
        break;

    case READY:
        heaterRelay.off();
        stopBasketMotor();
        buzzer.mode(BUZZER_READY);
        break;

    case LOWERING:
        heaterRelay.off();
        upwardRelay.off();
        downwardRelay.on();
        buzzer.mode(BUZZER_LOWERING);
        break;

    case FRYING:
        stopBasketMotor();
        buzzer.mode(BUZZER_START);
        break;

    case RAISING:
        heaterRelay.off();
        downwardRelay.off();
        upwardRelay.on();
        buzzer.mode(BUZZER_RAISING);
        break;

    case FAULT:
        heaterRelay.off();
        stopBasketMotor();
        // Choose fatal vs non-fatal buzzer pattern depending on fault
        if (lastFault == "ABS_OVERTEMP")
        {
            buzzer.mode(BUZZER_FATAL);
        }
        else
        {
            buzzer.mode(BUZZER_ERROR);
        }
        break;
    }
}

// ==================================================================================================================================================
// LIMIT SWITCHES
// ==================================================================================================================================================

// Debounce configuration for mechanical limit switches
const unsigned long LIMIT_DEBOUNCE_MS = 50UL; // 50 ms debounce

DebouncedLimitSwitch upperLimitSwitch(UPPER_LIMIT_PIN, "Upper", LIMIT_ACTIVE, LIMIT_DEBOUNCE_MS);
DebouncedLimitSwitch lowerLimitSwitch(LOWER_LIMIT_PIN, "Lower", LIMIT_ACTIVE, LIMIT_DEBOUNCE_MS);

bool upperLimitReached()
{
    return upperLimitSwitch.isActive();
}

bool lowerLimitReached()
{
    return lowerLimitSwitch.isActive();
}

// ==================================================================================================================================================
// STOP BASKET MOTOR
// ==================================================================================================================================================

void stopBasketMotor()
{
    upwardRelay.off();
    downwardRelay.off();
}

// ==================================================================================================================================================
// TEST ACTUATOR
// ==================================================================================================================================================

void testActuator()
{
    if (upperLimitReached())
    {
        upwardRelay.on();
        downwardRelay.off();
        return;
    }

    if (lowerLimitReached())
    {
        upwardRelay.off();
        downwardRelay.on();
        return;
    }

    stopBasketMotor();
}

// ==================================================================================================================================================
// BROADCAST STATUS
// ==================================================================================================================================================

void broadcastStatus()
{
    // Use a statically allocated JsonDocument to avoid runtime heap allocations
    StaticJsonDocument<256> doc;

    doc["type"] = "status";
    doc["temperature"] = currentTemperature;
    doc["target"] = targetTemperature;
    doc["remaining"] = remainingTime;
    doc["state"] = getStateName(fryerState);
    doc["product"] = selectedProduct;
    doc["icon"] = selectedIcon;
    doc["upper"] = upperLimitReached();
    if (lastFault.length() > 0)
    {
        doc["fault"] = lastFault;
    }

    String output;

    serializeJson(doc, output);

    ws.textAll(output);
}

// ==================================================================================================================================================
// SEND STATUS TO ONE CLIENT
// ==================================================================================================================================================

void sendStatus(AsyncWebSocketClient *client)
{
    StaticJsonDocument<256> doc;

    doc["type"] = "status";
    doc["temperature"] = currentTemperature;
    doc["target"] = targetTemperature;
    doc["remaining"] = remainingTime;
    doc["state"] = getStateName(fryerState);
    doc["product"] = selectedProduct;
    doc["icon"] = selectedIcon;
    doc["upper"] = upperLimitReached();
    if (lastFault.length() > 0)
    {
        doc["fault"] = lastFault;
    }

    String output;

    serializeJson(doc, output);

    client->text(output);
}

// ==================================================================================================================================================
// SAVE RECIPE
// ==================================================================================================================================================

void saveRecipe(const String &product, const String &icon, float temperature, int duration)
{
    selectedProduct = product;
    selectedIcon = icon;

    targetTemperature = temperature;
    selectedDuration = duration;

    remainingTime = 0;

    Serial.println();
    Serial.println("===========================================");
    Serial.println("[RECIPE] RECIPE SAVED");
    Serial.printf("[RECIPE] Product: %s\n", selectedProduct.c_str());
    Serial.printf("[RECIPE] Icon: %s\n", selectedIcon.c_str());
    Serial.printf("[RECIPE] Temperature: %.1f C\n", targetTemperature);
    Serial.printf("[RECIPE] Duration: %d seconds\n", selectedDuration);
    Serial.println("===========================================");
    Serial.println();

    setFryerState(PREHEATING);
    // Ensure any pending start/homing attempts are cancelled when a
    // new recipe is selected. Selection should not move the basket.
    pendingStart = false;
    lastFault = "";

    broadcastStatus();
}

// ==================================================================================================================================================
// CLEAR RECIPE
// ==================================================================================================================================================

void clearRecipe()
{
    selectedProduct = "";
    selectedIcon = "";

    targetTemperature = IDLE_TEMPERATURE;

    selectedDuration = 0;
    remainingTime = 0;
}

// ==================================================================================================================================================
// START FRYING
// ==================================================================================================================================================

void startFrying()
{
    if (selectedProduct.length() == 0)
    {
        Serial.println("[START] IGNORED - NO PRODUCT SELECTED");
        return;
    }

    if (selectedDuration <= 0)
    {
        Serial.println("[START] IGNORED - INVALID DURATION");
        return;
    }

    if (fryerState != READY)
    {
        Serial.printf("[START] IGNORED - CURRENT STATE: %s\n", getStateName(fryerState));
        return;
    }

    if (!temperatureValid)
    {
        Serial.println("[START] IGNORED - INVALID TEMPERATURE");
        heaterRelay.off();
        // PT100 faults are reported silently and do not trigger any modal or pause
        broadcastStatus();
        return;
    }

    // Safe-start: ensure basket is homed (upper limit). If not, raise first and
    // continue automatically once homed.
    if (!upperLimitReached())
    {
        Serial.println("[START] BASKET NOT RAISED - RAISING TO HOME BEFORE START");
        pendingStart = true;
        // warn operator before motion, then raise after PREMOVE tone finishes
        buzzer.mode(BUZZER_PREMOVE);
        pendingRaiseAfterPremove = true;
        broadcastStatus();
        return;
    }

    Serial.println();
    Serial.println("===========================================");
    Serial.println("[START] FRYING CYCLE STARTED");
    Serial.printf("[START] Product: %s\n", selectedProduct.c_str());
    Serial.printf("[START] Temperature: %.1f C\n", targetTemperature);
    Serial.printf("[START] Duration: %d seconds\n", selectedDuration);
    Serial.println("===========================================");
    Serial.println();

    remainingTime = selectedDuration;

    lastFrySecondMillis = millis();

    // warn operator before lowering, then lower after PREMOVE tone finishes
    buzzer.mode(BUZZER_PREMOVE);
    pendingLowerAfterPremove = true;

    broadcastStatus();
}

// ==================================================================================================================================================
// STOP FRYER
// ==================================================================================================================================================

void stopFryer()
{
    Serial.println();
    Serial.println("[STOP] STOP COMMAND RECEIVED");

    remainingTime = 0;

    heaterRelay.off();

    if (upperLimitReached())
    {
        Serial.println("[STOP] BASKET ALREADY AT UPPER LIMIT");

        clearRecipe();

        setFryerState(IDLE);
    }
    else
    {
        Serial.println("[STOP] RAISING BASKET");
        // warn operator before motion, then raise after PREMOVE tone finishes
        buzzer.mode(BUZZER_PREMOVE);
        pendingRaiseAfterPremove = true;
    }

    broadcastStatus();
}

// ==================================================================================================================================================
// READ PT100
// ==================================================================================================================================================

void readTemperature()
{

    if (millis() - lastTemperatureMillis < 250)
        return;

    // Ignore PT100 readings while the basket drive motor is active because relay
    // switching noise can generate false faults or invalid temperature samples.
    if (upwardRelay.getState() || downwardRelay.getState())
    {
        lastTemperatureMillis = millis();
        return;
    }

    lastTemperatureMillis = millis();

    uint8_t fault = pt100.readFault();

    if (fault)
    {
        Serial.printf("[PT100] FAULT: 0x%02X\n", fault);
        pt100.clearFault();
        // Ignore faulty PT100 readings while the system is otherwise running.
        temperatureValid = false;
        heaterRelay.off();
        broadcastStatus();
        return;
    }

    float temperature = pt100.temperature(RNOMINAL, RREF);

    if (isnan(temperature))
    {
        Serial.println("[PT100] INVALID TEMPERATURE");
        temperatureValid = false;
        heaterRelay.off();
        // PT100 invalid readings are silent and do not raise a fault modal
        broadcastStatus();
        return;
    }

    currentTemperature = temperature;
    temperatureValid = true;

    // Idle/ready states are not thermal-fault states. If the oil is hotter than the
    // selected product target, simply stop heating and wait for it to cool to a safe
    // margin before allowing a new fry cycle.
    if (fryerState == IDLE || fryerState == READY)
    {
        heaterRelay.off();

        if (currentTemperature >= ABSOLUTE_MAX_TEMP)
        {
            Serial.printf("[OVERTEMP] ABSOLUTE MAX REACHED: %.1f C\n", currentTemperature);
            heaterRelay.off();
            lastFault = "ABS_OVERTEMP";
            setFryerState(FAULT);
            broadcastStatus();
            return;
        }

        // Do not consider a warm-but-not-dangerous oil temperature a fault while
        // waiting for the user to start. The Start button will remain disabled until
        // temperature falls close enough to the target to be suitable for frying.
        return;
    }

    if (fryerState == FAULT)
    {
        heaterRelay.off();
        return;
    }

    // -----------------------------
    // Over-temperature absolute limit
    // -----------------------------
    if (currentTemperature >= ABSOLUTE_MAX_TEMP)
    {
        Serial.printf("[OVERTEMP] ABSOLUTE MAX REACHED: %.1f C\n", currentTemperature);
        heaterRelay.off();
        lastFault = "ABS_OVERTEMP";
        setFryerState(FAULT);
        broadcastStatus();
        return;
    }

    // -----------------------------
    // Over-target offset (user requested +20C)
    // -----------------------------
    if (currentTemperature > targetTemperature + OVERTEMP_OFFSET)
    {
        Serial.printf("[OVERTEMP] TARGET OVERSHOOT: %.1f C (target %.1f + offset %.1f)\n", currentTemperature, targetTemperature, OVERTEMP_OFFSET);
        heaterRelay.off();
        lastFault = "OVERTEMP";
        setFryerState(FAULT);
        broadcastStatus();
        return;
    }

    // -----------------------------
    // Runaway detection (rate-of-rise)
    // -----------------------------
    unsigned long now = millis();

    if (tempWindowStartMillis == 0)
    {
        tempWindowStartMillis = now;
        tempWindowStartTemp = currentTemperature;
    }
    else if (now - tempWindowStartMillis >= RUNAWAY_WINDOW_MS)
    {
        float delta = currentTemperature - tempWindowStartTemp;

        if (delta >= RUNAWAY_DELTA_C)
        {
            Serial.printf("[RUNAWAY] Temperature rose %.2f C in %lu ms\n", delta, now - tempWindowStartMillis);

            // Treat as thermal runaway: stop heating and raise basket automatically
            heaterRelay.off();
            lastFault = "RUNAWAY_TEMP";

            // If not already raising, start raising so basket leaves oil
            if (!upperLimitReached())
            {
                Serial.println("[RUNAWAY] Auto-raising basket to remove product from oil");
                // set state to RAISING (this turns on upward relay via setFryerState)
                setFryerState(RAISING);
            }
            else
            {
                // already raised — just set fault state
                setFryerState(FAULT);
            }

            broadcastStatus();

            // reset window start so we don't repeatedly trigger
            tempWindowStartMillis = 0;
            tempWindowStartTemp = 0.0;

            return;
        }

        // slide the window forward
        tempWindowStartMillis = now;
        tempWindowStartTemp = currentTemperature;
    }
}

// ==================================================================================================================================================
// HEATER CONTROL
// ==================================================================================================================================================

void updateHeater()
{
    if (!temperatureValid)
    {
        heaterRelay.off();
        return;
    }

    if (!(fryerState == PREHEATING || fryerState == READY || fryerState == LOWERING || fryerState == FRYING))
    {
        heaterRelay.off();
        return;
    }

    if (currentTemperature <= targetTemperature - HEATER_ON_OFFSET)
    {
        heaterRelay.on();
        return;
    }

    if (currentTemperature >= targetTemperature)
    {
        heaterRelay.off();
        return;
    }

    heaterRelay.off();
}

// ==================================================================================================================================================
// BASKET CONTROL
// ==================================================================================================================================================

void updateBasket()
{
    if (fryerState == LOWERING)
    {
        upwardRelay.off();

        if (lowerLimitReached())
        {
            downwardRelay.off();

            Serial.println("[BASKET] LOWER LIMIT REACHED");
            Serial.println("[BASKET] STARTING FRY TIMER");

            remainingTime = selectedDuration;
            lastFrySecondMillis = millis();

            setFryerState(FRYING);

            // play lowered confirmation tone
            buzzer.mode(BUZZER_LOWERED);

            broadcastStatus();
        }
        else if (millis() - stateStartMillis >= BASKET_LOWERING_TIMEOUT_MS)
        {
            Serial.printf("[BASKET] LOWERING TIMEOUT AFTER %lu SECONDS\n", BASKET_LOWERING_TIMEOUT_SECONDS);

            remainingTime = 0;
            lastFault = "LOWERING_TIMEOUT";
            setFryerState(FAULT);

            broadcastStatus();
        }
        else
        {
            downwardRelay.on();
        }

        return;
    }

    if (fryerState == RAISING)
    {
        downwardRelay.off();

        if (upperLimitReached())
        {
            upwardRelay.off();

            Serial.println("[BASKET] UPPER LIMIT REACHED");

            if (pendingStart)
            {
                // We were homing to start the cycle: clear the flag and lower immediately
                Serial.println("[BASKET] UPPER LIMIT REACHED (for pending start). Lowering now.");
                pendingStart = false;
                setFryerState(LOWERING);
                broadcastStatus();
            }
            else
            {
                Serial.println("[CYCLE] COMPLETE");
                Serial.println("[CYCLE] RETURNING TO IDLE");

                remainingTime = 0;

                setFryerState(IDLE);

                // play raised confirmation tone (override IDLE tone)
                buzzer.mode(BUZZER_RAISED);

                clearRecipe();

                broadcastStatus();
            }
        }
        else if (millis() - stateStartMillis >= BASKET_RAISING_TIMEOUT_MS)
        {
            Serial.printf("[BASKET] RAISING TIMEOUT AFTER %lu SECONDS\n", BASKET_RAISING_TIMEOUT_SECONDS);

            remainingTime = 0;
            lastFault = "RAISING_TIMEOUT";
            setFryerState(FAULT);

            broadcastStatus();
        }
        else
        {
            upwardRelay.on();
        }

        return;
    }

    stopBasketMotor();
}

// ==================================================================================================================================================
// FRYER UPDATE
// ==================================================================================================================================================

void updateFryer()
{
    unsigned long now = millis();

    readTemperature();

    updateHeater();

    updateBasket();

    if (fryerState == PREHEATING)
    {
        if (temperatureValid && currentTemperature >= targetTemperature)
        {
            currentTemperature = targetTemperature;

            heaterRelay.off();

            Serial.println("[TEMP] TARGET TEMPERATURE REACHED");
            Serial.printf("[TEMP] %.1f C\n", currentTemperature);
            Serial.println("[STATE] READY - PRESS START");

            remainingTime = 0;

            setFryerState(READY);

            broadcastStatus();
        }
    }

    if (fryerState == READY)
    {
        if (temperatureValid)
        {
            if (currentTemperature < targetTemperature - 0.5)
            {
                setFryerState(PREHEATING);

                broadcastStatus();
            }
        }
    }

    if (fryerState == FRYING)
    {
        // Calculate elapsed whole seconds since last tick and decrement accordingly.
        if (now > lastFrySecondMillis)
        {
            unsigned long elapsedMs = now - lastFrySecondMillis;
            unsigned long elapsedSec = elapsedMs / 1000UL;

            if (elapsedSec >= 1UL)
            {
                // advance the last tick by the elapsed whole seconds
                lastFrySecondMillis += elapsedSec * 1000UL;

                if (remainingTime > 0)
                {
                    // don't underflow remainingTime
                    int dec = (elapsedSec > (unsigned long)remainingTime) ? remainingTime : (int)elapsedSec;
                    remainingTime -= dec;

                    Serial.printf("[FRYING] -%d sec | %s | %.1f C | %02d:%02d\n", dec, selectedProduct.c_str(), currentTemperature, remainingTime / 60, remainingTime % 60);

                    broadcastStatus();
                }

                if (remainingTime <= 0)
                {
                    remainingTime = 0;

                    Serial.println("[FRYING] TIME COMPLETE");
                    Serial.println("[BASKET] RAISING BASKET (after DONE tone)");

                    // Play DONE pattern, then raise when the buzzer finishes
                    buzzer.mode(BUZZER_DONE);
                    pendingRaiseAfterDone = true;

                    broadcastStatus();
                }
            }
        }
    }

    // ========================================================
    // PERIODIC STATUS UPDATE
    // ========================================================

    if (now - lastStatusMillis >= 500)
    {
        lastStatusMillis = now;

        broadcastStatus();
    }

    // If we finished the DONE buzzer and a raise was requested, start raising now
    if (pendingRaiseAfterDone && !buzzer.isPlaying())
    {
        pendingRaiseAfterDone = false;
        Serial.println("[FRYING] DONE tone complete — raising basket now");
        setFryerState(RAISING);
        broadcastStatus();
    }

    // If a PREMOVE was requested, start the requested motion after the tone
    if (pendingLowerAfterPremove && !buzzer.isPlaying())
    {
        pendingLowerAfterPremove = false;
        Serial.println("[PREMOVE] PREMOVE complete — lowering basket now");
        setFryerState(LOWERING);
        broadcastStatus();
    }

    if (pendingRaiseAfterPremove && !buzzer.isPlaying())
    {
        pendingRaiseAfterPremove = false;
        Serial.println("[PREMOVE] PREMOVE complete — raising basket now");
        setFryerState(RAISING);
        broadcastStatus();
    }
}

// ==================================================================================================================================================
// WEBSOCKET MESSAGE HANDLER
// ==================================================================================================================================================

void handleWebSocketMessage(AsyncWebSocketClient *client, uint8_t *data, size_t len)
{
    StaticJsonDocument<256> doc;

    DeserializationError error = deserializeJson(doc, data, len);

    if (error)
    {
        Serial.printf("[WS] JSON ERROR: %s\n", error.c_str());
        return;
    }

    String command = doc["command"] | "";

    Serial.printf("[WS] COMMAND: %s\n", command.c_str());

    // ========================================================
    // STATUS
    // ========================================================

    if (command == "status")
    {
        sendStatus(client);
        return;
    }

    // ========================================================
    // RAISE (homing) - UI can request the basket raise to home
    // ========================================================
    if (command == "raise")
    {
        Serial.println("[WS] RAISE (homing) COMMAND RECEIVED");

        // Only trigger raising if we're not already at the upper limit
        if (!upperLimitReached())
        {
            // ensure we're not in a FAULT state before attempting to raise
            if (fryerState == FAULT)
            {
                Serial.println("[RAISE] IGNORED - CURRENT STATE: FAULT");
                sendStatus(client);
                return;
            }

            setFryerState(RAISING);
            broadcastStatus();
        }
        else
        {
            // already homed
            sendStatus(client);
        }

        return;
    }

    // ========================================================
    // SELECT PRODUCT
    // ========================================================

    if (command == "select")
    {
        if (fryerState != IDLE)
        {
            Serial.printf("[SELECT] IGNORED - CURRENT STATE: %s\n", getStateName(fryerState));

            sendStatus(client);

            return;
        }

        String product = doc["product"] | "";
        String icon = doc["icon"] | "";

        float temperature = doc["temperature"] | 120.0;
        int duration = doc["duration"] | 0;

        if (product.length() == 0)
        {
            Serial.println("[SELECT] IGNORED - EMPTY PRODUCT");
            return;
        }

        if (temperature <= 0)
        {
            Serial.println("[SELECT] IGNORED - INVALID TEMPERATURE");
            return;
        }

        if (duration <= 0)
        {
            Serial.println("[SELECT] IGNORED - INVALID DURATION");
            return;
        }

        saveRecipe(product, icon, temperature, duration);

        return;
    }

    // ========================================================
    // START
    // ========================================================

    if (command == "start")
    {
        Serial.println("[WS] START COMMAND RECEIVED");

        startFrying();

        return;
    }

    // ========================================================
    // STOP
    // ========================================================

    if (command == "stop")
    {
        Serial.println("[WS] STOP COMMAND RECEIVED");

        stopFryer();

        return;
    }

    // ========================================================
    // REBOOT (soft)
    // ========================================================
    if (command == "reboot")
    {
        Serial.println("[WS] REBOOT COMMAND RECEIVED");
        // attempt a soft restart
        ESP.restart();
        return;
    }

    // ========================================================
    // UNKNOWN COMMAND
    // ========================================================

    Serial.printf("[WS] UNKNOWN COMMAND: %s\n", command.c_str());
}

// ==================================================================================================================================================
// WEBSOCKET EVENT
// ==================================================================================================================================================

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("[WS] Client #%u connected\n", client->id());

        sendStatus(client);

        // operator feedback: client connected
        buzzer.mode(BUZZER_CONNECTED);

        break;

    case WS_EVT_DISCONNECT:
        Serial.printf("[WS] Client #%u disconnected\n", client->id());

        break;

    case WS_EVT_DATA:
    {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;

        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
        {
            handleWebSocketMessage(client, data, len);
        }

        break;
    }

    case WS_EVT_PONG:
        break;

    case WS_EVT_ERROR:
        Serial.printf("[WS] Client #%u error\n", client->id());

        break;
    }
}

// ==================================================================================================================================================
// SETUP
// ==================================================================================================================================================

void setup()
{
    Serial.begin(115200);
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector

    delay(500);

    Serial.println();
    Serial.println();
    Serial.println("===========================================");
    Serial.println("       SMART DEEP FRYER");
    Serial.println("===========================================");

    // ========================================================
    // GPIO
    // ========================================================

    pinMode(UPPER_LIMIT_PIN, INPUT_PULLUP);
    pinMode(LOWER_LIMIT_PIN, INPUT_PULLUP);

    // Sample each physical switch before control logic begins.
    upperLimitSwitch.begin();
    lowerLimitSwitch.begin();

    // ========================================================
    // PT100
    // ========================================================

    pt100.begin(MAX31865_3WIRE);

    Serial.println("[PT100] MAX31865 INITIALIZED");

    // ========================================================
    // RELAYS
    // ========================================================

    heaterRelay.begin();
    upwardRelay.begin();
    downwardRelay.begin();

    heaterRelay.off();
    upwardRelay.off();
    downwardRelay.off();

    Serial.println("[RELAY] RELAYS INITIALIZED");

    // ========================================================
    // BUZZER
    // ========================================================

    buzzer.begin();

    Serial.println("[BUZZER] INITIALIZED");

    // ========================================================
    // WIFI ACCESS POINT
    // ========================================================

    WiFi.mode(WIFI_AP);

    WiFi.softAPConfig(AP_IP, AP_GATEWAY, AP_SUBNET);

    WiFi.softAP(AP_SSID, AP_PASSWORD);

    Serial.println();
    Serial.println("[WIFI] ACCESS POINT STARTED");
    Serial.printf("[WIFI] SSID: %s\n", AP_SSID);
    Serial.printf("[WIFI] PASSWORD: %s\n", AP_PASSWORD);
    Serial.printf("[WIFI] IP: %s\n", WiFi.softAPIP().toString().c_str());

    // ========================================================
    // WEBSOCKET
    // ========================================================

    ws.onEvent(onWebSocketEvent);

    server.addHandler(&ws);

    // ========================================================
    // WEBPAGE
    // ========================================================

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", webpage); });

    // ========================================================
    // START SERVER
    // ========================================================

    server.begin();

    Serial.println();
    Serial.println("[HTTP] SERVER STARTED");
    Serial.println("[HTTP] DASHBOARD: http://192.168.4.1");
    Serial.println("[WS] WEBSOCKET: ws://192.168.4.1/ws");

    // ========================================================
    // INITIAL STATE
    // ========================================================

    clearRecipe();

    currentTemperature = IDLE_TEMPERATURE;
    targetTemperature = IDLE_TEMPERATURE;

    temperatureValid = false;

    setFryerState(IDLE);

    // Ensure the basket is homed on power-up. If the upper limit is
    // not reached, start raising immediately so the UI shows the
    // homing action and buttons remain disabled until complete.
    pendingStart = false; // ensure we are not auto-starting a fry
    lastFault = "";

    if (!upperLimitReached())
    {
        Serial.println("[BOOT] Basket not at upper limit - raising to home (power-on homing)");
        setFryerState(RAISING);
        broadcastStatus();
    }

    Serial.println();
    Serial.println("[SYSTEM] READY");
    Serial.println("===========================================");
    Serial.println();

    // startup tone
    buzzer.mode(BUZZER_STARTUP);
}

// ==================================================================================================================================================
// LOOP
// ==================================================================================================================================================

void loop()
{
    buzzer.update();

    updateFryer();

    ws.cleanupClients();

    delay(5);
}
