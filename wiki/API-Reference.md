# API Reference

All endpoints are implemented in [src/webserver.cpp](../src/webserver.cpp).

## Status and Settings

- `GET /api/status`
  - Returns runtime mode, FSM state, sensor flags, relay activity, stop reason, and countdowns.
- `GET /api/settings`
  - Returns current persisted settings.
- `POST /api/settings`
  - Updates and persists settings.
- `POST /api/settings/restore-defaults`
  - Restores factory settings and applies them.
- `GET /api/defaults`
  - Returns compile-time factory defaults.

## Statistics

- `GET /api/stats`
  - Returns cumulative telemetry counters.
- `POST /api/system/reset-stats`
  - Resets statistics counters to zero defaults.

## System Control

- `POST /api/system/reboot`
  - Schedules controlled reboot.
- `POST /api/system/factory-reset`
  - Restores settings and stats to defaults.
- `POST /api/system/test-run`
  - Starts/stops setup-mode timed diagnostic automation.

## OTA

- `POST /api/ota/update`
  - Upload firmware binary for OTA update.

## Error Handling

Common response patterns:
- `200`: successful operation
- `400`: invalid request payload or parameters
- `409`: operation not allowed in current mode
- `500`: persistence or internal runtime error

## Security Boundary

The HTTP API is designed for trusted local network usage in setup/service scenarios.
No built-in authentication is currently enforced by firmware.
