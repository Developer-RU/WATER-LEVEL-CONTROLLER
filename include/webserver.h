#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include "ota.h"
#include "relays.h"
#include "sensors.h"
#include "statistics.h"
#include "storage.h"

class WebServerManager {
public:
  void configure(StorageManager *storage,
                 RelayManager *relays,
                 SensorManager *sensors,
                 AppSettings *settings,
                 AppStatistics *statistics,
                 RuntimeState *runtime,
                 OtaManager *ota);
  void start();
  void stop();
  bool active() const;

private:
  AsyncWebServer server_{80};
  StorageManager *storage_ = nullptr;
  RelayManager *relays_ = nullptr;
  SensorManager *sensors_ = nullptr;
  AppSettings *settings_ = nullptr;
  AppStatistics *statistics_ = nullptr;
  RuntimeState *runtime_ = nullptr;
  OtaManager *ota_ = nullptr;
  bool configured_ = false;
  bool active_ = false;

  void registerRoutes();
  String buildStatusJson() const;
  String buildSettingsJson() const;
  String buildStatisticsJson() const;
  String buildDefaultsJson() const;
  bool parseSettingsRequest(AsyncWebServerRequest *request, AppSettings &settings) const;
  static String jsonEscape(const String &value);
};