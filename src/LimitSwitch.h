#ifndef LIMIT_SWITCH_H
#define LIMIT_SWITCH_H

#include <Arduino.h>

class DebouncedLimitSwitch
{
public:
    DebouncedLimitSwitch(uint8_t pin, const char *name, uint8_t activeLevel, unsigned long debounceMs)
        : pin(pin), name(name), activeLevel(activeLevel), debounceMs(debounceMs) {}

    void begin()
    {
        lastRaw = digitalRead(pin);
        stableState = (lastRaw == activeLevel);
        lastChangeMillis = millis();
    }

    bool isActive()
    {
        const int raw = digitalRead(pin);
        const unsigned long now = millis();

        if (raw != lastRaw)
        {
            lastRaw = raw;
            lastChangeMillis = now;
        }
        else if (now - lastChangeMillis >= debounceMs)
        {
            const bool newStableState = (raw == activeLevel);
            if (newStableState != stableState)
            {
                stableState = newStableState;
                Serial.printf("[LIMIT] %s debounced -> %s\n", name, stableState ? "ACTIVE" : "INACTIVE");
            }
        }

        return stableState;
    }

private:
    uint8_t pin;
    const char *name;
    uint8_t activeLevel;
    unsigned long debounceMs;
    int lastRaw = HIGH;
    unsigned long lastChangeMillis = 0;
    bool stableState = false;
};

#endif
