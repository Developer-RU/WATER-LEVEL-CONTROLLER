#pragma once

#include <Arduino.h>

#include "config.h"

/**
 * @brief Electrical interpretation for a level switch input.
 *
 * Closed means the contact is considered triggered when the GPIO reads LOW
 * (with INPUT_PULLUP topology). Open means triggered when GPIO reads HIGH.
 */
enum class SensorLogic : uint8_t {
  Closed = 0,
  Open = 1,
};

/**
 * @brief Main finite-state machine states for automatic fill control.
 */
enum class ControlState : uint8_t {
  Standby = 0,
  FillingUntilLowerClears = 1,
  TopOffCountdown = 2,
  Alarm = 3,
};

/**
 * @brief High-level operation mode.
 */
enum class SystemMode : uint8_t {
  Work = 0,
  Setup = 1,
};

/**
 * @brief Reason why pump output was switched OFF.
 */
enum class PumpStopReason : uint8_t {
  None = 0,
  TopOffDone = 1,
  UpperAlarm = 2,
  SetupFreeze = 3,
  TestRunStopped = 4,
  TestRunTimeout = 5,
};

/**
 * @brief Persisted user settings loaded from NVS.
 */
struct AppSettings {
  uint16_t fillTimeoutMinutes = DEFAULT_FILL_TIMEOUT_MINUTES;
  float pumpFlowLpm = DEFAULT_PUMP_FLOW_LPM;
  uint8_t pumpRelay = DEFAULT_PUMP_RELAY;
  uint8_t alarmRelay = DEFAULT_ALARM_RELAY;
  SensorLogic lowerSensorLogic = SensorLogic::Closed;
  SensorLogic upperSensorLogic = SensorLogic::Closed;
};

/**
 * @brief Persisted cumulative telemetry counters.
 */
struct AppStatistics {
  uint32_t totalPumpRuntimeSeconds = 0;
  double totalWaterLiters = 0.0;
  uint32_t fillCycles = 0;
  uint32_t alarmCount = 0;
};

/**
 * @brief Volatile runtime state consumed by control loop and web API.
 */
struct RuntimeState {
  SystemMode systemMode = SystemMode::Work;
  ControlState controlState = ControlState::Standby;
  bool lowerSensorActive = false;
  bool upperSensorActive = false;
  bool pumpActive = false;
  bool alarmActive = false;
  PumpStopReason lastPumpStopReason = PumpStopReason::None;
  bool setupModeArmed = false;
  bool wifiActive = false;
  bool webServerActive = false;
  bool setupModeActive = false;
  bool testRunActive = false;
  uint32_t testRunStartedAtMs = 0;
  uint32_t testRunTimeoutSeconds = 0;
  uint32_t testRunRemainingSeconds = 0;
  String bootResetReason = "UNKNOWN";
  String previousResetReason = "UNKNOWN";
  uint32_t bootCount = 0;
  bool lowerWaitForClear = false;
  uint32_t fillRemainingSeconds = 0;
  uint32_t fillDurationSeconds = 0;
  uint32_t fillDeadlineMs = 0;
  uint32_t topOffStartedAtMs = 0;
};

/**
 * @brief Converts sensor logic enum to stable API/UI text.
 */
inline const char *sensorLogicToText(SensorLogic logic) {
  return logic == SensorLogic::Closed ? "Closed" : "Open";
}

/**
 * @brief Converts stop reason enum to stable API/UI text.
 */
inline const char *pumpStopReasonToText(PumpStopReason reason) {
  switch (reason) {
    case PumpStopReason::TopOffDone:
      return "TOP_OFF_DONE";
    case PumpStopReason::UpperAlarm:
      return "UPPER_ALARM";
    case PumpStopReason::SetupFreeze:
      return "SETUP_FREEZE";
    case PumpStopReason::TestRunStopped:
      return "TEST_RUN_STOPPED";
    case PumpStopReason::TestRunTimeout:
      return "TEST_RUN_TIMEOUT";
    case PumpStopReason::None:
    default:
      return "NONE";
  }
}