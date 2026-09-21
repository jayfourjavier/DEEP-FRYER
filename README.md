# Smart Deep Fryer — Firmware + Web UI

## Overview

This repository implements an ESP32-based automatic deep-fryer controller with a single-file web dashboard served over Wi‑Fi Access Point mode. The firmware manages temperature (PT100 via Adafruit MAX31865), basket motion (up/down relays + limit switches), a heater relay, and a buzzer. The UI (embedded in `include/webpage.h`) communicates over WebSocket to display status and control the fryer.

Important safety features implemented:

- Safe-start (auto-raise to home before starting a fry)
- Client watchdog for motion timeouts
- Firmware timeouts for lowering/raising paths
- Overtemperature detection (absolute max and target+20°C overshoot)
- Runaway detection (rapid temperature rise -> auto-raise + fault)
- Descriptive fault reporting to the UI (fault codes surfaced in status JSON)

## Components & Pins

- ESP32 (PlatformIO project)
- PT100 RTD via Adafruit MAX31865 (CS pin: `PT100_CS_PIN`)
- Relays:
  - Heater: `HEATER_RELAY_PIN` (33)
  - Upward motor: `UPWARD_RELAY_PIN` (26)
  - Downward motor: `DOWNWARD_RELAY_PIN` (25)
- Buzzer: `BUZZER_PIN` (4)
- Limit switches:
  - Upper limit: `UPPER_LIMIT_PIN` (16)
  - Lower limit: `LOWER_LIMIT_PIN` (17)
- `LIMIT_ACTIVE` is defined as `LOW` (active-low switches)

Hardware helpers are in `Relay.h` and `Buzzer.h`.

## How the UI & Firmware Communicate

- WebSocket endpoint: `/ws`
- Status message (sent by firmware periodically and on events):

```json
{
  "type":"status",
  "temperature": 120.0,
  "target": 175,
  "remaining": 180,
  "state": "FRYING",
  "product": "French Fries",
  "icon": "🍟",
  "upper": true,      // NEW: whether upper limit switch is reached (home)
  "fault":"RUNAWAY_TEMP" // optional
}
```

- Commands from UI to firmware (WebSocket JSON): `status`, `select`, `start`, `stop`, `raise`, `reboot`.
  - `raise` triggers a homing (RAISING) action if not already at upper limit.

## UI Modals & Buttons (current)

- Manual Recipe modal: `manualSave`, `manualCancel` (preview: `manualDurationPreview`)
- Confirm Recipe modal: `confirmSelectConfirm`, `confirmSelectCancel`, `confirmSelectHint`
  - Auto-requests `raise` if `upper:false` and disables Confirm until homed
- Confirm Start modal: `confirmStartConfirm`, `confirmStartCancel`, `confirmStartHint`
  - Same homing behavior as select-confirm
- Completion modal: `confirmDoneOk`
- Error modal: `errorReboot` (reboot-only — other dismiss removed per request)

## Algorithm / State Machine (high-level)

1. IDLE: waiting for product selection.
2. PREHEATING: target set, heater enabled until target reached.
3. READY: temperature reached — user may `Start`.
   - On `Start`: check `upper` (homed). If not homed, `pendingStart=true` and set state to RAISING; once homed, lowering begins automatically.
4. LOWERING: move basket down until lower limit; on timeout -> `LOWERING_TIMEOUT` fault.
5. FRYING: heater managed to maintain temperature; countdown decremented per elapsed seconds; when remaining==0 -> RAISING.
6. RAISING: move basket up until upper limit; on timeout -> `RAISING_TIMEOUT` fault.
7. FAULT: heater off, buzzer alarm; requires user intervention (reboot currently).

Mermaid flowchart:

```mermaid
flowchart TD
  IDLE -->|select| PREHEATING
  PREHEATING -->|target reached| READY
  READY -->|start| CHECK_HOME
  CHECK_HOME -- home? yes --> LOWERING
  CHECK_HOME -- home? no --> RAISING[RAISING (homing)] --> LOWERING
  LOWERING -->|lower reached| FRYING
  LOWERING -->|timeout| FAULT
  FRYING -->|timer done| RAISING
  RAISING -->|upper reached| IDLE
  RAISING -->|timeout| FAULT
  FAULT -->|reboot| IDLE
```

## Error & Fault Handling (list of codes)

- `PT100_FAULT` — PT100/Max31865 reported a fault. Action: heater off, go to FAULT.
- `PT100_INVALID` — NaN reading from sensor. Action: heater off, go to FAULT.
- `LOWERING_TIMEOUT` — Lowering did not reach lower limit within configured timeout.
- `RAISING_TIMEOUT` — Raising did not reach upper limit within configured timeout.
- `ABS_OVERTEMP` — Absolute temperature exceeded safe maximum (e.g., 240°C). Heater off + FAULT.
- `OVERTEMP` — Temperature exceeded (target + OVERTEMP_OFFSET, currently +20°C). Heater off + FAULT.
- `RUNAWAY_TEMP` — Temperature rose rapidly (RUNAWAY_DELTA_C within RUNAWAY_WINDOW_MS), treated as thermal runaway; heater off and auto-raise attempted.

UI: firmware fault codes are shown in the `errorModal` with user-friendly text. The error modal currently offers a `Reboot Device` button only.

## Recommended Fixes & Improvements (priority sorted)

1. JSON allocation safety (HIGH):
   - Currently the firmware uses `JsonDocument doc;` without specifying capacity. Replace with `StaticJsonDocument<...>` or `DynamicJsonDocument` sized appropriately to avoid heap issues or runtime allocation failures. Example:

     ```cpp
     StaticJsonDocument<256> doc; // tune capacity
     ```

   - Do this in `broadcastStatus()`, `sendStatus()`, and WebSocket handlers.
2. Limit switch debouncing (HIGH):
   - Add simple software debouncing (require stable reading for 20–50 ms) or sample multiple reads to avoid false triggers from noisy mechanical switches.
3. Rate-limited broadcasting (MEDIUM):
   - `broadcastStatus()` is called periodically and on events; currently status is sent at 500ms cadence. Consider lowering frequency to 1s or only broadcasting on state changes to reduce airtime and CPU usage.
4. Fault clearing (MEDIUM):
   - Add a `clearFault` WebSocket command or secure handshake so that not every fault requires a full reboot. Only allow clearing when system is safe (e.g., temperature below thresholds and basket homed).
5. Webpage delivery / PROGMEM (LOW):
   - `request->send_P` usage was flagged as deprecated in analyzer. Switch to the recommended `request->send()` or use `AsyncWebServerResponse` with `send_P` depending on AsyncWebServer version. Test that the full webpage still fits in flash.
6. Improve safety logging & guidance (LOW):
   - Expand fault message text to include clear remediation steps (e.g., `RUNAWAY_TEMP`: "Raise basket, disable heater, check element and oil level"). Also log events with timestamps to SPIFFS for post-mortem.
7. WebSocket input validation (MEDIUM):
   - Validate incoming JSON fields and bounds (temperature range, duration) in `handleWebSocketMessage` more defensively.
8. Persistent configuration (OPT):
   - Consider storing last-good recipe or calibration data to SPIFFS or Preferences so restart preserves useful state.

## Build & Flash

Requirements:

- PlatformIO (recommended) with ESP32 environment.

Common commands:

```bash
# build
platformio run
# upload (use configured environment)
platformio run --target upload
# monitor serial
platformio device monitor --port COM7 --baud 115200
```

## Testing Checklist (safety first)

- Verify limit switches behave as expected and test manual homing with no oil in basket.
- Confirm heater turns off on `PT100` faults and overtemp conditions; verify `RUNAWAY_TEMP` causes auto-raise.
- Test UI Confirm/Start flows both when basket is already homed and when not homed.
- Test lowering/raising timeouts by disabling a limit switch to ensure meaningful fault messages.

## Where to Start Next

- Replace anonymous `JsonDocument` instances with sized `StaticJsonDocument` in `src/main.cpp` (urgent).
- Add switch debounce logic (short software filter) in `upperLimitReached()` and `lowerLimitReached()` helpers.
- Optional: implement a `clearFault` command and a more granular recovery workflow.

## Files of Interest

- `src/main.cpp` — main state machine, WebSocket handlers, sensor reading, safety logic.
- `include/webpage.h` — inlined web dashboard (HTML/CSS/JS) served from flash.
- `lib/` — libraries including `Adafruit_MAX31865`, `ArduinoJson`, and helper libraries.

## Safety Notice

This firmware controls heaters and moving mechanical parts. Verify behavior on a test bench without hot oil and with personnel safety in mind. Validate limit switches and emergency stop behavior before full-scale tests.

---
If you want, I can:

- Open PR-style patches to replace `JsonDocument` with `StaticJsonDocument` (I can propose sizes),
- Add debounce code for limit switches and a `clearFault` handler, or
- Create unit-test harness scripts (host-side simulation) for the state transitions.

Which follow-up should I implement first? (I recommend fixing `JsonDocument` allocations now.)
