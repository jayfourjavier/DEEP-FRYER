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
    BUZZER_LOWERING,
    BUZZER_RAISING,
    BUZZER_DONE,
    BUZZER_ERROR,
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
            // ERROR
            // FAST REPEATING BEEP
            // ====================================================

        case BUZZER_ERROR:
            if (now - lastToggleMillis >= 150)
            {
                lastToggleMillis = now;
                writeBuzzer(!outputState);
            }

            if (now - stepStartMillis >= 1500)
            {
                stop();
            }
            break;

            // ====================================================
            // ALARM
            // CONTINUOUS FAST BEEP
            // ====================================================

        case BUZZER_ALARM:
            if (now - lastToggleMillis >= 100)
            {
                lastToggleMillis = now;
                writeBuzzer(!outputState);
            }
            break;
        }
    }

    bool isPlaying()
    {
        return playing;
    }
};

#endif