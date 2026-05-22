# Code Reference

This document provides expanded technical documentation for the current implementation without changing runtime behavior.

## Runtime Model

The firmware runs as a cooperative loop on ESP32 with the following pillars:
- Sensor sampling and debounce.
- Control FSM execution.
- Statistics accumulation.
- Periodic NVS persistence.
- Web API and dashboard service in setup mode.
- OTA restart scheduling.

## Module Map

## Core Types

Defined in [include/app_types.h](../include/app_types.h):
- `SensorLogic`: input interpretation model.
- `ControlState`: runtime finite states.
- `SystemMode`: work/setup mode split.
- `PumpStopReason`: normalized reason code for pump stop.
- `AppSettings`: persistent user-controlled settings.
- `AppStatistics`: cumulative telemetry counters.
- `RuntimeState`: volatile state exposed to control loop and API.

## Settings And Limits

Defined in [include/config.h](../include/config.h):
- Hardware pin mapping.
- AP credentials.
- Timeout and flow boundaries.
- Watchdog and heartbeat cadence.
- Default sensor trigger mode.

## Control Loop Ownership

Implemented in [src/main.cpp](../src/main.cpp):
- Initializes all managers and persistence.
- Handles setup-mode arming and delayed AP startup.
- Runs normal work mode FSM.
- Enforces emergency shutdown on upper sensor trigger.
- Freezes automation in setup mode unless test run is active.

## FSM Behavior Details

### Standby
- Pump and alarm outputs are OFF.
- Waits for lower sensor trigger.

### FillingUntilLowerClears
- Pump ON.
- Waits until lower sensor returns to normal.

### TopOffCountdown
- Pump ON.
- Countdown starts when lower sensor clears.
- Returns to previous state if lower sensor retriggers.
- Stops pump at timeout and records stop reason.

### Alarm
- Pump OFF immediately.
- Alarm relay ON.
- State clears only when both sensors are normal.

## Statistics Model

Implemented in [src/statistics.cpp](../src/statistics.cpp):
- Runtime seconds are accumulated from elapsed milliseconds while pump is active.
- Liters are estimated from configured flow and elapsed active runtime.
- Dirty flag indicates pending NVS write.

Formula:

```text
liters_delta = flow_lpm * elapsed_ms / 60000
```

## Persistence Model

Implemented in [src/storage.cpp](../src/storage.cpp):
- Settings namespace: `wlc-settings`.
- Statistics namespace: `wlc-stats`.
- Input sanitization on load/save protects against invalid values.
- Factory restore resets both settings and statistics.

## Sensor Model

Implemented in [src/sensors.cpp](../src/sensors.cpp):
- Debounce window is 50 ms.
- Inputs are sampled with INPUT_PULLUP topology.
- Logic can be switched between Closed and Open trigger modes.

## Relay Model

Implemented in [src/relays.cpp](../src/relays.cpp):
- Supports channels 1..4.
- Startup initializes relays to OFF to avoid output pulse.
- Relay active polarity is controlled by compile-time flag.

## Web Dashboard And API

Implemented in [src/webserver.cpp](../src/webserver.cpp):
- Embedded HTML/CSS/JS dashboard served from firmware image.
- Responsive behavior via CSS media breakpoints.
- Runtime polling endpoint for live status updates.
- Settings, maintenance, and OTA control endpoints.

## OTA Flow

Implemented in [src/ota.cpp](../src/ota.cpp):
- Upload start initializes Update subsystem.
- Chunks are written incrementally.
- End call validates and finalizes image.
- Restart is deferred and triggered from main loop.

## Diagnostics And Reliability

- Boot diagnostics are persisted using dedicated namespace.
- Heartbeat log outputs current state and free heap periodically.
- Loop watchdog can be enabled for runtime safety.

## Safety Invariants

- Upper sensor alarm path has strict stop priority.
- Pump and alarm relay channels cannot share same relay in sanitized settings.
- Setup mode freezes automation by default.
- Factory defaults are restored when persistence is missing or invalid.
