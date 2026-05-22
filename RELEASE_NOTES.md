# Release Notes

## 1.0.0 - 2026-05-22

### Summary

First stable production release of WATER-LEVEL-CONTROLLER for ESP32-based relay control blocks used in well pump and Eurocube filling systems.

### Highlights

- Deterministic automatic refill workflow with emergency overflow protection.
- Two-sensor logic (lower operational trigger and upper emergency trigger).
- Setup mode with local AP dashboard for configuration and diagnostics.
- OTA firmware update endpoint.
- Persistent settings and runtime telemetry in ESP32 NVS.
- Responsive embedded web interface for desktop and mobile devices.

### Included Telemetry

- Total pump runtime in seconds.
- Estimated pumped water volume in liters (based on configured pump flow).
- Fill cycle counter.
- Emergency alarm shutdown counter.
- Boot diagnostics: reset reason and boot count.

### Hardware Profile

- Target MCU: ESP32.
- Relay support: 2-channel and 4-channel boards.
- Two side-mounted float sensors.

### Operational Notes

- Upper sensor has strict emergency priority and forces immediate pump stop.
- Setup mode freezes automation until a test run is explicitly started.
- Settings are sanitized before persistence.

### Known Constraints

- Liter calculation is an estimation model based on configured flow and runtime.
- Dashboard and API are designed for local trusted network usage.
