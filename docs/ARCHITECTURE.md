# Architecture

This document describes the production architecture of the WATER-LEVEL-CONTROLLER solution.

## 1. High-Level Overview

The system is an ESP32-based embedded controller for autonomous well pump operation and Eurocube tank level control.

Primary objectives:
- Maintain target fill behavior using a lower trigger sensor.
- Prevent overflow using an upper emergency sensor.
- Provide local setup and diagnostics without internet dependency.
- Persist operational metrics and settings across power cycles.

## 2. Runtime Layers

1. Hardware abstraction layer
- Sensor inputs with debounce.
- Relay output abstraction with configurable active level.

2. Control layer
- Finite state machine for refill lifecycle and alarm handling.

3. Service layer
- NVS storage service.
- Statistics accumulation service.
- OTA update service.

4. Presentation layer
- Embedded responsive web dashboard and REST API.

## 3. Core State Model

State is split into:
- Persistent configuration and counters.
- Volatile runtime state.

Persistent objects:
- AppSettings
- AppStatistics

Volatile object:
- RuntimeState

## 4. Control FSM

Core states:
- Standby
- FillingUntilLowerClears
- TopOffCountdown
- Alarm

Transition rules:
- Lower sensor triggers fill start when in standby.
- Lower sensor clear starts top-off countdown.
- Upper sensor trigger causes immediate alarm transition from any active control state.
- Alarm clears only when both sensors return to normal.

## 5. Setup Mode Model

Setup mode can be armed at boot with the setup button.

Behavior:
- Automation is frozen by default.
- Relays are forced off.
- Dashboard and API are served from local AP.
- Timed test run can temporarily enable automation for diagnostics.

## 6. Telemetry and Persistence

Tracked values:
- totalPumpRuntimeSeconds
- totalWaterLiters
- fillCycles
- alarmCount

Storage backend:
- ESP32 Preferences (NVS namespaces for settings, statistics, diagnostics).

## 7. Network and Security Posture

Designed for local trusted network usage.

Current model:
- Setup AP with local web dashboard.
- OTA upload endpoint in local access context.

Recommended deployment controls:
- Isolated control network.
- Strong AP credentials.
- Physical protection of control cabinet.

## 8. Reliability Features

- Watchdog support in main loop.
- Heartbeat diagnostics logging.
- Startup self-healing to factory defaults if persistence is invalid.
- Deterministic relay-off behavior during mode transitions.

## 9. Module Ownership

- src/main.cpp: orchestration and FSM lifecycle.
- src/sensors.cpp: sampling and debounce.
- src/relays.cpp: relay control abstraction.
- src/statistics.cpp: runtime and volume accounting.
- src/storage.cpp: settings/statistics persistence.
- src/ota.cpp: firmware update flow.
- src/webserver.cpp: dashboard and API.
