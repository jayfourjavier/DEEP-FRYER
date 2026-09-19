#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <Adafruit_MAX31865.h>

#include "Relay.h"
#include "Buzzer.h"
#include "webpage.h"

// ============================================================
// WIFI ACCESS POINT
// ============================================================

const char *AP_SSID = "SMART-DEEP-FRYER";
const char *AP_PASSWORD = "12345678";

IPAddress AP_IP(192, 168, 4, 1);
IPAddress AP_GATEWAY(192, 168, 4, 1);
IPAddress AP_SUBNET(255, 255, 255, 0);

// ============================================================
// HARDWARE PINS
// ============================================================

#define PT100_CS_PIN 5

#define HEATER_RELAY_PIN 33
#define UPWARD_RELAY_PIN 26
#define DOWNWARD_RELAY_PIN 25

#define BUZZER_PIN 4

// CHANGE THESE TO YOUR ACTUAL LIMIT SWITCH PINS
#define UPPER_LIMIT_PIN 27
#define LOWER_LIMIT_PIN 14

#define LIMIT_ACTIVE LOW

// ============================================================
// PT100
// ============================================================

#define RREF 430.0
#define RNOMINAL 100.0

Adafruit_MAX31865 pt100(PT100_CS_PIN);

// ============================================================
// HARDWARE OBJECTS
// ============================================================

Relay heaterRelay(HEATER_RELAY_PIN, "Heater Relay", true);
Relay upwardRelay(UPWARD_RELAY_PIN, "Upward Relay", true);
Relay downwardRelay(DOWNWARD_RELAY_PIN, "Downward Relay", true);

Buzzer buzzer(BUZZER_PIN, "Buzzer", true);

// ============================================================
// WEB SERVER
// ============================================================

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// ============================================================
// FRYER STATES
// ============================================================

enum FryerState
{
    IDLE,
    PREHEATING,
    READY,
    LOWERING,
    FRYING,
    RAISING
};

FryerState fryerState = IDLE;

// ============================================================
// RECIPE
// ============================================================

String selectedProduct = "";
String selectedIcon = "";

float targetTemperature = 120.0;

int selectedDuration = 0;
int remainingTime = 0;

// ============================================================
// TEMPERATURE
// ============================================================

float currentTemperature = 120.0;
bool temperatureValid = false;

const float IDLE_TEMPERATURE = 120.0;

const float HEATER_ON_OFFSET = 2.0;

// ============================================================
// TIMERS
// ============================================================

unsigned long stateStartMillis = 0;
unsigned long lastTemperatureMillis = 0;
unsigned long lastStatusMillis = 0;
unsigned long lastFrySecondMillis = 0;

// ============================================================
// FUNCTION DECLARATIONS
// ============================================================

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

// ============================================================
// STATE NAME
// ============================================================

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
    }

    return "IDLE";
}

// ============================================================
// SET FRYER STATE
// ============================================================

void setFryerState(FryerState newState)
{
    FryerState oldState = fryerState;

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
    }
}

// ============================================================
// LIMIT SWITCHES
// ============================================================

bool upperLimitReached()
{
    return digitalRead(UPPER_LIMIT_PIN) == LIMIT_ACTIVE;
}

bool lowerLimitReached()
{
    return digitalRead(LOWER_LIMIT_PIN) == LIMIT_ACTIVE;
}

// ============================================================
// STOP BASKET MOTOR
// ============================================================

void stopBasketMotor()
{
    upwardRelay.off();
    downwardRelay.off();
}

// ============================================================
// BROADCAST STATUS
// ============================================================

void broadcastStatus()
{
    JsonDocument doc;

    doc["type"] = "status";
    doc["temperature"] = currentTemperature;
    doc["target"] = targetTemperature;
    doc["remaining"] = remainingTime;
    doc["state"] = getStateName(fryerState);
    doc["product"] = selectedProduct;
    doc["icon"] = selectedIcon;

    String output;

    serializeJson(doc, output);

    ws.textAll(output);
}

// ============================================================
// SEND STATUS TO ONE CLIENT
// ============================================================

void sendStatus(AsyncWebSocketClient *client)
{
    JsonDocument doc;

    doc["type"] = "status";
    doc["temperature"] = currentTemperature;
    doc["target"] = targetTemperature;
    doc["remaining"] = remainingTime;
    doc["state"] = getStateName(fryerState);
    doc["product"] = selectedProduct;
    doc["icon"] = selectedIcon;

    String output;

    serializeJson(doc, output);

    client->text(output);
}

// ============================================================
// SAVE RECIPE
// ============================================================

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

    broadcastStatus();
}

// ============================================================
// CLEAR RECIPE
// ============================================================

void clearRecipe()
{
    selectedProduct = "";
    selectedIcon = "";

    targetTemperature = IDLE_TEMPERATURE;

    selectedDuration = 0;
    remainingTime = 0;
}

// ============================================================
// START FRYING
// ============================================================

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

    setFryerState(LOWERING);

    broadcastStatus();
}

// ============================================================
// STOP FRYER
// ============================================================

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

        setFryerState(RAISING);
    }

    broadcastStatus();
}

// ============================================================
// READ PT100
// ============================================================

void readTemperature()
{

    if (millis() - lastTemperatureMillis < 250)
        return;
    lastTemperatureMillis = millis();

    uint8_t fault = pt100.readFault();

    if (fault)
    {
        Serial.printf("[PT100] FAULT: 0x%02X\n", fault);
        pt100.clearFault();
        temperatureValid = false;
        heaterRelay.off();

        return;
    }

    float temperature = pt100.temperature(RNOMINAL, RREF);

    if (isnan(temperature))
    {
        Serial.println("[PT100] INVALID TEMPERATURE");
        temperatureValid = false;
        heaterRelay.off();
        return;
    }

    currentTemperature = temperature;
    temperatureValid = true;
}

// ============================================================
// HEATER CONTROL
// ============================================================

void updateHeater()
{
    if (!temperatureValid)
    {
        heaterRelay.off();
        return;
    }

    if (fryerState == PREHEATING || fryerState == READY || fryerState == LOWERING || fryerState == FRYING)
    {
        if (currentTemperature <= targetTemperature - HEATER_ON_OFFSET)
        {
            heaterRelay.on();
        }

        if (currentTemperature >= targetTemperature)
        {
            heaterRelay.off();
        }

        return;
    }

    heaterRelay.off();
}

// ============================================================
// BASKET CONTROL
// ============================================================

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
            Serial.println("[CYCLE] COMPLETE");
            Serial.println("[CYCLE] RETURNING TO IDLE");

            remainingTime = 0;

            setFryerState(IDLE);

            clearRecipe();

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

// ============================================================
// FRYER UPDATE
// ============================================================

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
        if (now - lastFrySecondMillis >= 1000)
        {
            lastFrySecondMillis += 1000;

            if (remainingTime > 0)
            {
                remainingTime--;

                Serial.printf("[FRYING] %s | %.1f C | %02d:%02d\n", selectedProduct.c_str(), currentTemperature, remainingTime / 60, remainingTime % 60);

                broadcastStatus();
            }

            if (remainingTime <= 0)
            {
                remainingTime = 0;

                Serial.println("[FRYING] TIME COMPLETE");
                Serial.println("[BASKET] RAISING BASKET");

                setFryerState(RAISING);

                broadcastStatus();
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
}

// ============================================================
// WEBSOCKET MESSAGE HANDLER
// ============================================================

void handleWebSocketMessage(AsyncWebSocketClient *client, uint8_t *data, size_t len)
{
    JsonDocument doc;

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
    // UNKNOWN COMMAND
    // ========================================================

    Serial.printf("[WS] UNKNOWN COMMAND: %s\n", command.c_str());
}

// ============================================================
// WEBSOCKET EVENT
// ============================================================

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("[WS] Client #%u connected\n", client->id());

        sendStatus(client);

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

// ============================================================
// SETUP
// ============================================================

void setup()
{
    Serial.begin(115200);

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

    Serial.println();
    Serial.println("[SYSTEM] READY");
    Serial.println("===========================================");
    Serial.println();
}

// ============================================================
// LOOP
// ============================================================

void loop()
{
    buzzer.update();
    updateFryer();

    ws.cleanupClients();

    delay(5);
}