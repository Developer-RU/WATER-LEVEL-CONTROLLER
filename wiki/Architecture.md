# Architecture

## Runtime Layers

The firmware runs as a cooperative loop with clear module boundaries:

- Control orchestration: [src/main.cpp](../src/main.cpp)
- Sensor debounce and logic mapping: [src/sensors.cpp](../src/sensors.cpp)
- Relay abstraction: [src/relays.cpp](../src/relays.cpp)
- Statistics accumulation: [src/statistics.cpp](../src/statistics.cpp)
- NVS persistence: [src/storage.cpp](../src/storage.cpp)
- HTTP dashboard and API: [src/webserver.cpp](../src/webserver.cpp)
- OTA upload and deferred restart: [src/ota.cpp](../src/ota.cpp)

## Hardware Model

- MCU: ESP32
- Inputs: lower and upper float sensors (dry contact)
- Outputs: relay channel for pump and relay channel for alarm
- Optional setup button for setup mode arming at boot

GPIO defaults are defined in [include/config.h](../include/config.h).

## State Ownership

Persisted state:
- App settings (timeouts, flow, relay mapping, sensor logic)
- Cumulative statistics (runtime, liters, cycles, alarms)

Volatile runtime state:
- Current mode (`Work` or `Setup`)
- Current control state (`Standby`, `FillingUntilLowerClears`, `TopOffCountdown`, `Alarm`)
- Live sensor and output flags
- Countdown and test-run timers

See [include/app_types.h](../include/app_types.h) for the data model.

## Setup Mode Portal

In setup mode, the firmware can start a local access point and serve dashboard/API endpoints.

Behavior guarantees:
- Automation is frozen in setup mode by default.
- Automation can be enabled only through a timed test run endpoint.
- Test run stops automatically on timeout or explicit stop request.
