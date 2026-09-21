#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

// ============================================================
// BUZZER MODES
// ============================================================

enum BuzzerMode
{
    BUZZER_BEEP,
    BUZZER_READY,
    BUZZER_START,
    BUZZER_STARTUP,
    BUZZER_CONNECTED,
    BUZZER_PREMOVE,
    BUZZER_LOWERING,
    BUZZER_LOWERED,
    BUZZER_RAISING,
    BUZZER_RAISED,
    BUZZER_DONE,
    BUZZER_ERROR,
    BUZZER_FATAL,
    BUZZER_ALARM
};

// ============================================================
// BUZZER CLASS
// ============================================================

class Buzzer
{
private:
    uint8_t pin;
    const char *name;
    bool activeLow;

    BuzzerMode currentMode = BUZZER_BEEP;

    bool playing = false;
    bool outputState = false;

    unsigned long stepStartMillis = 0;
    unsigned long lastToggleMillis = 0;

    uint8_t step = 0;

    void writeBuzzer(bool state)
    {
        outputState = state;

        if (activeLow)
            digitalWrite(pin, state ? LOW : HIGH);
        else
            digitalWrite(pin, state ? HIGH : LOW);
    }

    void stop()
    {
        writeBuzzer(false);
        playing = false;
        step = 0;
    }

    void start()
    {
        playing = true;
        step = 0;
        stepStartMillis = millis();
        lastToggleMillis = millis();
        writeBuzzer(false);
    }

public:
    Buzzer(uint8_t pin, const char *name = "Buzzer", bool activeLow = true)
    {
        this->pin = pin;
        this->name = name;
        this->activeLow = activeLow;
    }

    void begin()
    {
        pinMode(pin, OUTPUT);
        writeBuzzer(false);
        playing = false;

        Serial.printf("[BUZZER] %s initialized on GPIO %d\n", name, pin);
    }

    void mode(BuzzerMode newMode)
    {
        currentMode = newMode;
        start();

        Serial.printf("[BUZZER] Mode: %d\n", currentMode);
    }

    void update()
    {
        if (!playing)
            return;

        unsigned long now = millis();

        switch (currentMode)
        {
            // ====================================================
            // SIMPLE BEEP
            // ====================================================

        case BUZZER_BEEP:
            if (now - stepStartMillis < 150)
            {
                writeBuzzer(true);
            }
            else
            {
                stop();
            }
            break;

            // ====================================================
            // READY
            // BEEP - PAUSE - BEEP
            // ====================================================

        case BUZZER_READY:
            switch (step)
            {
            case 0:
                writeBuzzer(true);

                if (now - stepStartMillis >= 150)
                {
                    writeBuzzer(false);
                    step = 1;
                    stepStartMillis = now;
                }
                break;

            case 1:
                if (now - stepStartMillis >= 150)
                {
                    writeBuzzer(true);
                    step = 2;
                    stepStartMillis = now;
                }
                break;

            case 2:
                if (now - stepStartMillis >= 150)
                {
                    stop();
                }
                break;
            }
            break;

        // ====================================================
        // STARTUP: two short ascending beeps
        // ON 120ms, OFF 80ms, ON 200ms
        // ====================================================
        case BUZZER_STARTUP:
            switch (step)
            {
            case 0:
                writeBuzzer(true);

                if (now - stepStartMillis >= 120)
                {
                    writeBuzzer(false);
                    step = 1;
                    stepStartMillis = now;
                }
                break;

            case 1:
                if (now - stepStartMillis >= 80)
                {
                    writeBuzzer(true);
                    step = 2;
                    stepStartMillis = now;
                }
                break;

            case 2:
                if (now - stepStartMillis >= 200)
                {
                    stop();
                }
                break;
            }
            break;

        // ====================================================
        // CONNECTED: two quick beeps
        // ON 120ms, OFF 80ms, ON 120ms
        // ====================================================
        case BUZZER_CONNECTED:
            switch (step)
            {
            case 0:
                writeBuzzer(true);

                if (now - stepStartMillis >= 120)
                {
                    writeBuzzer(false);
                    step = 1;
                    stepStartMillis = now;
                }
                break;

            case 1:
                if (now - stepStartMillis >= 80)
                {
                    writeBuzzer(true);
                    step = 2;
                    stepStartMillis = now;
                }
                break;

            case 2:
                if (now - stepStartMillis >= 120)
                {
                    stop();
                }
                break;
            }
            break;

        // ====================================================
        // PREMOVE: single alert before motion
        // ON 200ms
        // ====================================================
        case BUZZER_PREMOVE:
            if (now - stepStartMillis < 200)
            {
                writeBuzzer(true);
            }
            else
            {
                stop();
            }
            break;

            // ====================================================
            // START
            // LONG BEEP
            // ====================================================

        case BUZZER_START:
            writeBuzzer(true);

            if (now - stepStartMillis >= 500)
            {
                stop();
            }
            break;

            // ====================================================
            // LOWERING
            // THREE SHORT BEEPS
            // ====================================================

        case BUZZER_LOWERING:
            switch (step)
            {
            case 0:
                writeBuzzer(true);

                if (now - stepStartMillis >= 100)
                {
                    writeBuzzer(false);
                    step = 1;
                    stepStartMillis = now;
                }
                break;

            case 1:
                if (now - stepStartMillis >= 100)
                {
                    writeBuzzer(true);
                    step = 2;
                    stepStartMillis = now;
                }
                break;

            case 2:
                if (now - stepStartMillis >= 100)
                {
                    writeBuzzer(false);
                    step = 3;
                    stepStartMillis = now;
                }
                break;

            case 3:
                if (now - stepStartMillis >= 100)
                {
                    writeBuzzer(true);
                    step = 4;
                    stepStartMillis = now;
                }
                break;

            case 4:
                if (now - stepStartMillis >= 100)
                {
                    stop();
                }
                break;
            }
            break;

        // ====================================================
        // LOWERED: three short beeps (same as DONE)
        // ====================================================
        case BUZZER_LOWERED:
            switch (step)
            {
            case 0:
                writeBuzzer(true);

                if (now - stepStartMillis >= 100)
                {
                    writeBuzzer(false);
                    step = 1;
                    stepStartMillis = now;
                }
                break;

            case 1:
                if (now - stepStartMillis >= 100)
                {
                    writeBuzzer(true);
                    step = 2;
                    stepStartMillis = now;
                }
                break;

            case 2:
                if (now - stepStartMillis >= 100)
                {
                    writeBuzzer(false);
                    step = 3;
                    stepStartMillis = now;
                }
                break;

            case 3:
                if (now - stepStartMillis >= 100)
                {
                    writeBuzzer(true);
                    step = 4;
                    stepStartMillis = now;
                }
                break;

            case 4:
                if (now - stepStartMillis >= 100)
                {
                    stop();
                }
                break;
            }
            break;

            // ====================================================
            // RAISING
            // TWO LONGER BEEPS
            // ====================================================

        case BUZZER_RAISING:
            switch (step)
            {
            case 0:
                writeBuzzer(true);

                if (now - stepStartMillis >= 250)
                {
                    writeBuzzer(false);
                    step = 1;
                    stepStartMillis = now;
                }
                break;

            case 1:
                if (now - stepStartMillis >= 200)
                {
                    writeBuzzer(true);
                    step = 2;
                    stepStartMillis = now;
                }
                break;

            case 2:
                if (now - stepStartMillis >= 250)
                {
                    stop();
                }
                break;
            }
            break;

        // ====================================================
        // RAISED: two medium beeps
        // ON 250ms, OFF 200ms, ON 250ms
        // ====================================================
        case BUZZER_RAISED:
            switch (step)
            {
            case 0:
                writeBuzzer(true);

                if (now - stepStartMillis >= 250)
                {
                    writeBuzzer(false);
                    step = 1;
                    stepStartMillis = now;
                }
                break;

            case 1:
                if (now - stepStartMillis >= 200)
                {
                    writeBuzzer(true);
                    step = 2;
                    stepStartMillis = now;
                }
                break;

            case 2:
                if (now - stepStartMillis >= 250)
                {
                    stop();
                }
                break;
            }
            break;

            // ====================================================
            // DONE
            // THREE SHORT BEEPS
            // ====================================================

        case BUZZER_DONE:
            switch (step)
            {
            case 0:
                writeBuzzer(true);

                if (now - stepStartMillis >= 100)
                {
                    writeBuzzer(false);
                    step = 1;
                    stepStartMillis = now;
                }
                break;

            case 1:
                if (now - stepStartMillis >= 100)
                {
                    writeBuzzer(true);
                    step = 2;
                    stepStartMillis = now;
                }
                break;

            case 2:
                if (now - stepStartMillis >= 100)
                {
                    writeBuzzer(false);
                    step = 3;
                    stepStartMillis = now;
                }
                break;

            case 3:
                if (now - stepStartMillis >= 100)
                {
                    writeBuzzer(true);
                    step = 4;
                    stepStartMillis = now;
                }
                break;

            case 4:
                if (now - stepStartMillis >= 100)
                {
                    stop();
                }
                break;
            }
            break;

        // ====================================================
        // ERROR: three rapid beeps then stop
        // ON 150ms, OFF 150ms x3
        // ====================================================
        case BUZZER_ERROR:
            switch (step)
            {
            case 0:
                writeBuzzer(true);

                if (now - stepStartMillis >= 150)
                {
                    writeBuzzer(false);
                    step = 1;
                    stepStartMillis = now;
                }
                break;

            case 1:
                if (now - stepStartMillis >= 150)
                {
                    writeBuzzer(true);
                    step = 2;
                    stepStartMillis = now;
                }
                break;

            case 2:
                if (now - stepStartMillis >= 150)
                {
                    writeBuzzer(false);
                    step = 3;
                    stepStartMillis = now;
                }
                break;

            case 3:
                if (now - stepStartMillis >= 150)
                {
                    writeBuzzer(true);
                    step = 4;
                    stepStartMillis = now;
                }
                break;

            case 4:
                if (now - stepStartMillis >= 150)
                {
                    stop();
                }
                break;
            }
            break;

            // ====================================================
            // ALARM
            // CONTINUOUS FAST BEEP
            // ====================================================

        case BUZZER_ALARM:
            /* Play three long buzzes then stop. Pattern: ON 600ms, OFF 250ms (x3) */
            switch (step)
            {
            case 0:
                writeBuzzer(true);

                if (now - stepStartMillis >= 600)
                {
                    writeBuzzer(false);
                    step = 1;
                    stepStartMillis = now;
                }
                break;

            case 1:
                if (now - stepStartMillis >= 250)
                {
                    writeBuzzer(true);
                    step = 2;
                    stepStartMillis = now;
                }
                break;

            case 2:
                if (now - stepStartMillis >= 600)
                {
                    writeBuzzer(false);
                    step = 3;
                    stepStartMillis = now;
                }
                break;

            case 3:
                if (now - stepStartMillis >= 250)
                {
                    writeBuzzer(true);
                    step = 4;
                    stepStartMillis = now;
                }
                break;

            case 4:
                if (now - stepStartMillis >= 600)
                {
                    writeBuzzer(false);
                    step = 5;
                    stepStartMillis = now;
                }
                break;

            case 5:
                if (now - stepStartMillis >= 250)
                {
                    writeBuzzer(true);
                    step = 6;
                    stepStartMillis = now;
                }
                break;

            case 6:
                if (now - stepStartMillis >= 600)
                {
                    stop();
                }
                break;
            }
            break;

        // ====================================================
        // FATAL: continuous long repeating alarm until stopped
        // ON 700ms, OFF 300ms, repeat indefinitely
        // ====================================================
        case BUZZER_FATAL:
            if (outputState)
            {
                if (now - lastToggleMillis >= 700)
                {
                    lastToggleMillis = now;
                    writeBuzzer(false);
                }
            }
            else
            {
                if (now - lastToggleMillis >= 300)
                {
                    lastToggleMillis = now;
                    writeBuzzer(true);
                }
            }
            // do NOT call stop(); requires explicit stop or reboot
            break;
        }
    }

    bool isPlaying()
    {
        return playing;
    }
};

#endif