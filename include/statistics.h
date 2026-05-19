#pragma once

#include <Arduino.h>

#include "app_types.h"

class StatisticsManager {
public:
  void begin(uint32_t nowMs);
  void onPumpStarted(uint32_t nowMs);
  void onPumpStopped(uint32_t nowMs);
  void update(AppStatistics &statistics, bool pumpActive, float flowLpm, uint32_t nowMs);
  bool dirty() const;
  void clearDirty();

private:
  uint32_t lastTickMs_ = 0;
  uint32_t runtimeCarryMs_ = 0;
  bool dirty_ = false;
};