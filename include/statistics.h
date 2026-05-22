#pragma once

#include <Arduino.h>

#include "app_types.h"

/**
 * @brief Accumulates pump runtime and estimated delivered water volume.
 */
class StatisticsManager {
public:
  /** @brief Resets internal timers and dirty flag. */
  void begin(uint32_t nowMs);
  /** @brief Marks start of a new pump run window. */
  void onPumpStarted(uint32_t nowMs);
  /** @brief Finalizes current pump run accounting window. */
  void onPumpStopped(uint32_t nowMs);
  /** @brief Adds elapsed runtime and volume delta while pump is active. */
  void update(AppStatistics &statistics, bool pumpActive, float flowLpm, uint32_t nowMs);
  /** @brief Returns true when statistics should be flushed to NVS. */
  bool dirty() const;
  /** @brief Clears dirty flag after successful save. */
  void clearDirty();

private:
  uint32_t lastTickMs_ = 0;
  uint32_t runtimeCarryMs_ = 0;
  bool dirty_ = false;
};