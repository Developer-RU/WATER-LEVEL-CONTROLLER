#include "storage.h"

#include "config.h"

static constexpr const char *SETTINGS_NAMESPACE = "wlc-settings";
static constexpr const char *STATS_NAMESPACE = "wlc-stats";

// Builds default settings object from compile-time constants.
AppSettings StorageManager::factorySettings() {
  AppSettings settings;
  settings.fillTimeoutMinutes = DEFAULT_FILL_TIMEOUT_MINUTES;
  settings.pumpFlowLpm = DEFAULT_PUMP_FLOW_LPM;
  settings.pumpRelay = DEFAULT_PUMP_RELAY;
  settings.alarmRelay = DEFAULT_ALARM_RELAY;
  settings.lowerSensorLogic = static_cast<SensorLogic>(DEFAULT_SENSOR_TRIGGER_MODE);
  settings.upperSensorLogic = static_cast<SensorLogic>(DEFAULT_SENSOR_TRIGGER_MODE);
  return settings;
}

AppStatistics StorageManager::factoryStatistics() {
  return AppStatistics{};
}

// Ensures loaded settings are safe and within supported limits.
void StorageManager::sanitizeSettings(AppSettings &settings) {
  if (settings.fillTimeoutMinutes < MIN_FILL_TIMEOUT_MINUTES) {
    settings.fillTimeoutMinutes = MIN_FILL_TIMEOUT_MINUTES;
  }

  if (settings.fillTimeoutMinutes > MAX_FILL_TIMEOUT_MINUTES) {
    settings.fillTimeoutMinutes = MAX_FILL_TIMEOUT_MINUTES;
  }

  if (settings.pumpFlowLpm < MIN_PUMP_FLOW_LPM) {
    settings.pumpFlowLpm = MIN_PUMP_FLOW_LPM;
  }

  if (settings.pumpFlowLpm > MAX_PUMP_FLOW_LPM) {
    settings.pumpFlowLpm = MAX_PUMP_FLOW_LPM;
  }

  if (settings.pumpRelay < 1 || settings.pumpRelay > 4) {
    settings.pumpRelay = DEFAULT_PUMP_RELAY;
  }

  if (settings.alarmRelay < 1 || settings.alarmRelay > 4) {
    settings.alarmRelay = DEFAULT_ALARM_RELAY;
  }

  if (settings.pumpRelay == settings.alarmRelay) {
    settings.alarmRelay = (settings.pumpRelay == 4) ? 3 : static_cast<uint8_t>(settings.pumpRelay + 1);
  }

  if (settings.lowerSensorLogic != SensorLogic::Closed && settings.lowerSensorLogic != SensorLogic::Open) {
    settings.lowerSensorLogic = SensorLogic::Closed;
  }

  if (settings.upperSensorLogic != SensorLogic::Closed && settings.upperSensorLogic != SensorLogic::Open) {
    settings.upperSensorLogic = SensorLogic::Closed;
  }
}

// Loads settings and falls back to defaults if required keys are missing.
bool StorageManager::loadSettings(AppSettings &settings) {
  Preferences prefs;
  if (!prefs.begin(SETTINGS_NAMESPACE, true)) {
    return false;
  }

  if (!prefs.isKey("fillTimeout") || !prefs.isKey("pumpFlow") || !prefs.isKey("pumpRelay") || !prefs.isKey("alarmRelay")) {
    prefs.end();
    settings = factorySettings();
    return false;
  }

  settings.fillTimeoutMinutes = prefs.getUShort("fillTimeout", DEFAULT_FILL_TIMEOUT_MINUTES);
  settings.pumpFlowLpm = prefs.getFloat("pumpFlow", DEFAULT_PUMP_FLOW_LPM);
  settings.pumpRelay = prefs.getUChar("pumpRelay", DEFAULT_PUMP_RELAY);
  settings.alarmRelay = prefs.getUChar("alarmRelay", DEFAULT_ALARM_RELAY);
  settings.lowerSensorLogic = static_cast<SensorLogic>(prefs.getUChar("lowerLogic", DEFAULT_SENSOR_TRIGGER_MODE));
  settings.upperSensorLogic = static_cast<SensorLogic>(prefs.getUChar("upperLogic", DEFAULT_SENSOR_TRIGGER_MODE));
  prefs.end();

  sanitizeSettings(settings);
  return true;
}

// Stores validated settings values in NVS.
bool StorageManager::saveSettings(const AppSettings &settings) {
  AppSettings sanitized = settings;
  sanitizeSettings(sanitized);

  Preferences prefs;
  if (!prefs.begin(SETTINGS_NAMESPACE, false)) {
    return false;
  }

  const bool ok = prefs.putUShort("fillTimeout", sanitized.fillTimeoutMinutes) > 0 &&
                  prefs.putFloat("pumpFlow", sanitized.pumpFlowLpm) > 0 &&
                  prefs.putUChar("pumpRelay", sanitized.pumpRelay) > 0 &&
                  prefs.putUChar("alarmRelay", sanitized.alarmRelay) > 0 &&
                  prefs.putUChar("lowerLogic", static_cast<uint8_t>(sanitized.lowerSensorLogic)) > 0 &&
                  prefs.putUChar("upperLogic", static_cast<uint8_t>(sanitized.upperSensorLogic)) > 0;
  prefs.end();
  return ok;
}

// Loads persisted runtime statistics from NVS.
bool StorageManager::loadStatistics(AppStatistics &statistics) {
  Preferences prefs;
  if (!prefs.begin(STATS_NAMESPACE, true)) {
    return false;
  }

  if (!prefs.isKey("runtimeSec") || !prefs.isKey("waterLiters") || !prefs.isKey("cycles") || !prefs.isKey("alarms")) {
    prefs.end();
    statistics = factoryStatistics();
    return false;
  }

  statistics.totalPumpRuntimeSeconds = prefs.getUInt("runtimeSec", 0);
  statistics.totalWaterLiters = prefs.getDouble("waterLiters", 0.0);
  statistics.fillCycles = prefs.getUInt("cycles", 0);
  statistics.alarmCount = prefs.getUInt("alarms", 0);
  prefs.end();
  return true;
}

// Saves runtime statistics to NVS.
bool StorageManager::saveStatistics(const AppStatistics &statistics) {
  Preferences prefs;
  if (!prefs.begin(STATS_NAMESPACE, false)) {
    return false;
  }

  const bool ok = prefs.putUInt("runtimeSec", statistics.totalPumpRuntimeSeconds) > 0 &&
                  prefs.putDouble("waterLiters", statistics.totalWaterLiters) > 0 &&
                  prefs.putUInt("cycles", statistics.fillCycles) > 0 &&
                  prefs.putUInt("alarms", statistics.alarmCount) > 0;
  prefs.end();
  return ok;
}

bool StorageManager::resetStatistics(AppStatistics &statistics) {
  statistics = factoryStatistics();
  return saveStatistics(statistics);
}

bool StorageManager::restoreFactorySettings(AppSettings &settings, AppStatistics &statistics) {
  settings = factorySettings();
  statistics = factoryStatistics();
  return saveSettings(settings) && saveStatistics(statistics);
}