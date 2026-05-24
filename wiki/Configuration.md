# Configuration

## Build and Upload

Project configuration is in [platformio.ini](../platformio.ini).

Typical commands:

```bash
pio run
pio run -t upload
pio device monitor -b 115200
```

## Compile-Time Settings

Main constants are in [include/config.h](../include/config.h):

- Relay pins: `RELAY_1_PIN` to `RELAY_4_PIN`
- Sensor pins: `LOWER_LEVEL_SENSOR_PIN`, `UPPER_LEVEL_SENSOR_PIN`
- Setup button pin: `SETUP_BUTTON_PIN`
- Relay polarity: `RELAY_ACTIVE_LOW`
- Setup AP credentials: `SETUP_AP_SSID`, `SETUP_AP_PASSWORD`
- Timeout and flow limits

## Runtime Settings (Persisted)

Runtime-editable settings are stored in NVS and represented by `AppSettings` in [include/app_types.h](../include/app_types.h):

- `fillTimeoutMinutes`
- `pumpFlowLpm`
- `pumpRelay`
- `alarmRelay`
- `lowerSensorLogic`
- `upperSensorLogic`

Sanitization rules are enforced in [src/storage.cpp](../src/storage.cpp).

## Sensor Logic Modes

Each sensor can be interpreted as:

- `Closed`: trigger when input reads LOW (pull-up topology)
- `Open`: trigger when input reads HIGH

This allows adapting to different dry-contact wiring conventions.
