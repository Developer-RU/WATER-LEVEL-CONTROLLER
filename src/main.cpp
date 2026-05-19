#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>

#include "config.h"
#include "ota.h"
#include "relays.h"
#include "sensors.h"
#include "statistics.h"
#include "storage.h"
#include "webserver.h"

static StorageManager gStorage;
static RelayManager gRelays;
static SensorManager gSensors;
static StatisticsManager gStatisticsManager;
static WebServerManager gWebServer;
static OtaManager gOta;

static AppSettings gSettings;
static AppStatistics gStatistics;
static RuntimeState gRuntime;

static uint32_t gBootMs = 0;
static uint32_t gSetupApStartAtMs = 0;
static uint32_t gLastStatsSaveMs = 0;
static bool gSetupButtonHeld = false;

static uint32_t configuredTopOffSeconds() {
  uint32_t minutes = gSettings.fillTimeoutMinutes;
  if (minutes < MIN_FILL_TIMEOUT_MINUTES) {
    minutes = MIN_FILL_TIMEOUT_MINUTES;
  }
  if (minutes > MAX_FILL_TIMEOUT_MINUTES) {
    minutes = MAX_FILL_TIMEOUT_MINUTES;
  }
  return minutes * 60UL;
}

static void printSettings(const AppSettings &settings) {
  Serial.printf("Settings: timeout=%u min, flow=%.2f L/min, pumpRelay=%u, alarmRelay=%u, lower=%s, upper=%s\n",
                settings.fillTimeoutMinutes,
                settings.pumpFlowLpm,
                settings.pumpRelay,
                settings.alarmRelay,
                sensorLogicToText(settings.lowerSensorLogic),
                sensorLogicToText(settings.upperSensorLogic));
}

static void ensureFilesystem() {
  if (LittleFS.begin(true)) {
    Serial.println("LittleFS mounted successfully");
  } else {
    Serial.println("LittleFS mount failed, continuing without file storage");
  }
}

static bool loadPersistentState() {
  const bool settingsLoaded = gStorage.loadSettings(gSettings);
  const bool statisticsLoaded = gStorage.loadStatistics(gStatistics);

  if (!settingsLoaded || !statisticsLoaded) {
    Serial.println("NVS data missing or incomplete, restoring factory defaults");
    if (!settingsLoaded) {
      gSettings = StorageManager::factorySettings();
      gStorage.saveSettings(gSettings);
    }
    if (!statisticsLoaded) {
      gStatistics = StorageManager::factoryStatistics();
      gStorage.saveStatistics(gStatistics);
    }
  }

  return settingsLoaded && statisticsLoaded;
}

static void updateHardwareFromSettings() {
  gSensors.applySettings(gSettings);
  printSettings(gSettings);
}

static void enterSetupMode() {
  gRuntime.systemMode = SystemMode::Setup;
  gRuntime.setupModeActive = true;
  gRuntime.testRunActive = false;
  gRuntime.testRunStartedAtMs = 0;
  gRuntime.testRunTimeoutSeconds = 0;
  gRuntime.testRunRemainingSeconds = 0;
  gRuntime.controlState = ControlState::Standby;
  gRuntime.pumpActive = false;
  gRuntime.alarmActive = false;
  gRuntime.lastPumpStopReason = PumpStopReason::SetupFreeze;
  gRuntime.lowerWaitForClear = false;
  gRuntime.fillRemainingSeconds = 0;
  gRuntime.fillDurationSeconds = 0;
  gRuntime.fillDeadlineMs = 0;
  gRuntime.topOffStartedAtMs = 0;
  gRelays.allOff();
  Serial.println("Setup mode entered. All relays off and automation frozen.");
}

static void stopSetupTestRun(const char *reason) {
  if (!gRuntime.testRunActive) {
    return;
  }

  gRuntime.testRunActive = false;
  gRuntime.testRunStartedAtMs = 0;
  gRuntime.testRunTimeoutSeconds = 0;
  gRuntime.testRunRemainingSeconds = 0;
  gRuntime.controlState = ControlState::Standby;
  gRuntime.pumpActive = false;
  gRuntime.alarmActive = false;
  gRuntime.lastPumpStopReason =
      (strcmp(reason, "timeout reached") == 0) ? PumpStopReason::TestRunTimeout : PumpStopReason::TestRunStopped;
  gRuntime.lowerSensorActive = false;
  gRuntime.upperSensorActive = false;
  gRuntime.fillRemainingSeconds = 0;
  gRuntime.fillDeadlineMs = 0;
  gRuntime.topOffStartedAtMs = 0;
  gRelays.allOff();
  Serial.printf("Setup mode test run stopped: %s\n", reason);
}

static void startSetupPortal() {
  if (gRuntime.webServerActive) {
    return;
  }

  WiFi.mode(WIFI_AP);
  const bool apStarted = WiFi.softAP(SETUP_AP_SSID, SETUP_AP_PASSWORD);
  if (!apStarted) {
    Serial.println("Failed to start access point");
    return;
  }

  gWebServer.start();
  gRuntime.wifiActive = true;
  gRuntime.webServerActive = true;
  enterSetupMode();
  Serial.printf("Setup AP started. SSID=%s IP=%s\n", SETUP_AP_SSID, WiFi.softAPIP().toString().c_str());
}

static void triggerAlarm(const char *reason, uint32_t nowMs) {
  if (!gRuntime.alarmActive) {
    gStatistics.alarmCount++;
  }

  gRuntime.controlState = ControlState::Alarm;
  gRuntime.pumpActive = false;
  gRuntime.alarmActive = true;
  gRuntime.lastPumpStopReason = PumpStopReason::UpperAlarm;
  gRuntime.lowerWaitForClear = false;
  gRuntime.fillRemainingSeconds = 0;
  gRuntime.fillDurationSeconds = 0;
  gRelays.setRelay(gSettings.pumpRelay, false);
  gRelays.setRelay(gSettings.alarmRelay, true);
  gStatisticsManager.onPumpStopped(nowMs);
  Serial.printf("Alarm triggered: %s\n", reason);
}

static void startFill(uint32_t nowMs) {
  gRuntime.pumpActive = true;
  gRuntime.alarmActive = false;
  gRuntime.lastPumpStopReason = PumpStopReason::None;
  gRuntime.lowerWaitForClear = true;
  gRuntime.controlState = ControlState::FillingUntilLowerClears;
  gRuntime.fillDurationSeconds = configuredTopOffSeconds();
  gRuntime.fillRemainingSeconds = 0;
  gRuntime.fillDeadlineMs = 0;
  gRuntime.topOffStartedAtMs = 0;
  gRelays.setRelay(gSettings.alarmRelay, false);
  gRelays.setRelay(gSettings.pumpRelay, true);
  gStatistics.fillCycles++;
  gStatisticsManager.onPumpStarted(nowMs);
  Serial.printf("Pump started. Waiting for lower sensor to clear before top-off timer. Top-off=%lu sec\n",
                static_cast<unsigned long>(gRuntime.fillDurationSeconds));
}

static void startTopOffCountdown(uint32_t nowMs) {
  if (gRuntime.fillDurationSeconds == 0) {
    gRuntime.fillDurationSeconds = configuredTopOffSeconds();
  }

  gRuntime.controlState = ControlState::TopOffCountdown;
  gRuntime.fillRemainingSeconds = gRuntime.fillDurationSeconds;
  gRuntime.fillDeadlineMs = nowMs + (gRuntime.fillDurationSeconds * 1000UL);
  gRuntime.topOffStartedAtMs = nowMs;
  Serial.printf("Lower sensor returned to normal. Top-off countdown started: %lu sec\n",
                static_cast<unsigned long>(gRuntime.fillDurationSeconds));
}

static void resetTopOffCountdown(const char *reason) {
  if (gRuntime.controlState == ControlState::TopOffCountdown) {
    Serial.printf("Top-off countdown reset: %s\n", reason);
  }
  gRuntime.controlState = ControlState::FillingUntilLowerClears;
  gRuntime.fillRemainingSeconds = 0;
  gRuntime.fillDeadlineMs = 0;
  gRuntime.topOffStartedAtMs = 0;
}

static void processControlLoop(uint32_t nowMs) {
  if (gRuntime.systemMode == SystemMode::Setup && !gRuntime.testRunActive) {
    return;
  }

  const bool lower = gRuntime.lowerSensorActive;
  const bool upper = gRuntime.upperSensorActive;

  if (gRuntime.alarmActive) {
    gRelays.setRelay(gSettings.pumpRelay, false);
    gRelays.setRelay(gSettings.alarmRelay, true);

    if (!lower && !upper) {
      gRuntime.alarmActive = false;
      gRuntime.controlState = ControlState::Standby;
      gRelays.setRelay(gSettings.alarmRelay, false);
      Serial.println("Alarm cleared, returning to standby");
    }
    return;
  }

  if (upper) {
    triggerAlarm("upper sensor active", nowMs);
    return;
  }

  if (!gRuntime.pumpActive) {
    gRelays.setRelay(gSettings.alarmRelay, false);
    gRelays.setRelay(gSettings.pumpRelay, false);
    gRuntime.fillRemainingSeconds = 0;
    gRuntime.fillDeadlineMs = 0;
    gRuntime.fillDurationSeconds = configuredTopOffSeconds();
    gRuntime.controlState = ControlState::Standby;

    if (lower) {
      startFill(nowMs);
    }
    return;
  }

  gRelays.setRelay(gSettings.pumpRelay, true);

  if (gRuntime.controlState == ControlState::FillingUntilLowerClears) {
    if (!lower) {
      startTopOffCountdown(nowMs);
    }
    return;
  }

  if (gRuntime.controlState == ControlState::TopOffCountdown) {
    if (lower) {
      resetTopOffCountdown("lower sensor became active again");
      return;
    }

    if (nowMs >= gRuntime.fillDeadlineMs) {
      gRuntime.pumpActive = false;
      gRuntime.controlState = ControlState::Standby;
      gRuntime.fillRemainingSeconds = 0;
      gRuntime.fillDeadlineMs = 0;
      gRuntime.lowerWaitForClear = false;
      gRuntime.lastPumpStopReason = PumpStopReason::TopOffDone;
      gRelays.setRelay(gSettings.pumpRelay, false);
      gStatisticsManager.onPumpStopped(nowMs);
      Serial.println("Pump stopped: top-off countdown completed.");
      return;
    }

    const uint32_t remainingMs = gRuntime.fillDeadlineMs - nowMs;
    gRuntime.fillRemainingSeconds = (remainingMs + 999UL) / 1000UL;
    return;
  }
}

static void flushStatisticsIfNeeded(uint32_t nowMs) {
  if (!gStatisticsManager.dirty()) {
    return;
  }

  if (!gRuntime.pumpActive && !gRuntime.alarmActive) {
    if (gStorage.saveStatistics(gStatistics)) {
      gStatisticsManager.clearDirty();
      gLastStatsSaveMs = nowMs;
      Serial.println("Statistics saved to NVS");
    }
    return;
  }

  if (nowMs - gLastStatsSaveMs >= 10000UL) {
    if (gStorage.saveStatistics(gStatistics)) {
      gStatisticsManager.clearDirty();
      gLastStatsSaveMs = nowMs;
      Serial.println("Periodic statistics save completed");
    }
  }
}

void setup() {
  Serial.begin(115200);
  gBootMs = millis();

  pinMode(SETUP_BUTTON_PIN, INPUT_PULLUP);
  gSetupButtonHeld = (digitalRead(SETUP_BUTTON_PIN) == BUTTON_ACTIVE_LEVEL);

  ensureFilesystem();

  loadPersistentState();
  updateHardwareFromSettings();

  gRelays.begin();
  gRelays.allOff();

  gSensors.begin(gSettings);
  gStatisticsManager.begin(gBootMs);
  gWebServer.configure(&gStorage, &gRelays, &gSensors, &gSettings, &gStatistics, &gRuntime, &gOta);
  gOta.begin();

  WiFi.mode(WIFI_OFF);
  gRuntime.setupModeArmed = gSetupButtonHeld;

  if (gSetupButtonHeld) {
    gSetupApStartAtMs = gBootMs + WIFI_AP_START_DELAY_MS;
    Serial.println("Setup button is held at boot. Setup portal will start after delay.");
  } else {
    Serial.println("Normal boot mode. Wi-Fi and web server remain disabled.");
  }
}

void loop() {
  const uint32_t nowMs = millis();

  if (gRuntime.setupModeArmed && !gRuntime.webServerActive && nowMs >= gSetupApStartAtMs) {
    startSetupPortal();
  }

  if (gRuntime.systemMode == SystemMode::Setup && gRuntime.testRunActive) {
    const uint32_t timeoutMs = gRuntime.testRunTimeoutSeconds * 1000UL;
    if (timeoutMs > 0) {
      const uint32_t elapsedMs = nowMs - gRuntime.testRunStartedAtMs;
      if (elapsedMs >= timeoutMs) {
        stopSetupTestRun("timeout reached");
      } else {
        const uint32_t remainingMs = timeoutMs - elapsedMs;
        gRuntime.testRunRemainingSeconds = (remainingMs + 999UL) / 1000UL;
      }
    }
  }

  if (gRuntime.systemMode == SystemMode::Setup && !gRuntime.testRunActive) {
    gRuntime.lowerSensorActive = false;
    gRuntime.upperSensorActive = false;
    gRuntime.pumpActive = false;
    gRuntime.alarmActive = false;
    gRuntime.controlState = ControlState::Standby;
    gRuntime.fillRemainingSeconds = 0;
    gRuntime.fillDeadlineMs = 0;
    gRuntime.topOffStartedAtMs = 0;
    gRelays.allOff();
    gOta.loop();
    return;
  }

  gSensors.update(nowMs);
  gRuntime.lowerSensorActive = gSensors.lowerTriggered();
  gRuntime.upperSensorActive = gSensors.upperTriggered();

  processControlLoop(nowMs);
  gStatisticsManager.update(gStatistics, gRuntime.pumpActive, gSettings.pumpFlowLpm, nowMs);
  flushStatisticsIfNeeded(nowMs);

  gOta.loop();

  if (gOta.hasError()) {
    Serial.printf("OTA error: %s\n", gOta.errorMessage().c_str());
  }
}