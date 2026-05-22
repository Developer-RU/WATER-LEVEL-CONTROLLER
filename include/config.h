#pragma once

#include <Arduino.h>

// Human-readable firmware identifier exposed through API/UI.
#define DEVICE_NAME "WATER-LEVEL-CONTROLLER"

// Relay GPIO mapping (ESP32 pins).
#define RELAY_1_PIN 25
#define RELAY_2_PIN 26
#define RELAY_3_PIN 32
#define RELAY_4_PIN 33

// Dry-contact level sensor GPIOs.
#define LOWER_LEVEL_SENSOR_PIN 22
#define UPPER_LEVEL_SENSOR_PIN 18

// Setup-mode entry button GPIO.
#define SETUP_BUTTON_PIN 23

// Relay and input electrical polarity.
#define RELAY_ACTIVE_LOW 1
#define BUTTON_ACTIVE_LEVEL LOW

// Access point used for provisioning/setup mode.
#define SETUP_AP_SSID "WLC-SETUP"
#define SETUP_AP_PASSWORD "wlc-setup"

// Factory defaults and limits for runtime settings.
#define DEFAULT_FILL_TIMEOUT_MINUTES 10
#define DEFAULT_PUMP_FLOW_LPM 10.0f
#define DEFAULT_PUMP_RELAY 1
#define DEFAULT_ALARM_RELAY 2
#define MAX_FILL_TIMEOUT_MINUTES 60
#define MIN_FILL_TIMEOUT_MINUTES 1
#define MIN_PUMP_FLOW_LPM 0.1f
#define MAX_PUMP_FLOW_LPM 999.9f
#define WIFI_AP_START_DELAY_MS 5000UL
#define SETUP_TEST_RUN_TIMEOUT_MINUTES 10

// Loop watchdog and diagnostics cadence.
#define WATCHDOG_ENABLED 1
#define WATCHDOG_FEED_INTERVAL_MS 1000UL
#define HEARTBEAT_LOG_INTERVAL_MS 5000UL

// Default trigger mode for both sensors in factory settings.
#define DEFAULT_SENSOR_TRIGGER_MODE 0