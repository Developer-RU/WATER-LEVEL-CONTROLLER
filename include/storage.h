#pragma once

#include <Arduino.h>
#include <Preferences.h>

#include "app_types.h"

/**
 * @brief Persistent storage facade over ESP32 NVS Preferences.
 */
class StorageManager {
public:
  /** @brief Returns compile-time default settings from config constants. */
  static AppSettings factorySettings();
  /** @brief Returns zeroed default statistics object. */
  static AppStatistics factoryStatistics();

  /** @brief Loads and validates settings from NVS. */
  bool loadSettings(AppSettings &settings);
  /** @brief Saves validated settings to NVS. */
  bool saveSettings(const AppSettings &settings);
  /** @brief Loads cumulative statistics from NVS. */
  bool loadStatistics(AppStatistics &statistics);
  /** @brief Saves cumulative statistics to NVS. */
  bool saveStatistics(const AppStatistics &statistics);
  /** @brief Resets statistics to defaults and persists them. */
  bool resetStatistics(AppStatistics &statistics);
  /** @brief Restores both settings and statistics to factory defaults. */
  bool restoreFactorySettings(AppSettings &settings, AppStatistics &statistics);

private:
  static void sanitizeSettings(AppSettings &settings);
};