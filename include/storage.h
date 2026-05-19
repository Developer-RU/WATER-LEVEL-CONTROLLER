#pragma once

#include <Arduino.h>
#include <Preferences.h>

#include "app_types.h"

class StorageManager {
public:
  static AppSettings factorySettings();
  static AppStatistics factoryStatistics();

  bool loadSettings(AppSettings &settings);
  bool saveSettings(const AppSettings &settings);
  bool loadStatistics(AppStatistics &statistics);
  bool saveStatistics(const AppStatistics &statistics);
  bool resetStatistics(AppStatistics &statistics);
  bool restoreFactorySettings(AppSettings &settings, AppStatistics &statistics);

private:
  static void sanitizeSettings(AppSettings &settings);
};