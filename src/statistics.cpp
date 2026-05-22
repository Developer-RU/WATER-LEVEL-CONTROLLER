#include "statistics.h"

// Initializes runtime accumulation state.
void StatisticsManager::begin(uint32_t nowMs) {
  lastTickMs_ = nowMs;
  runtimeCarryMs_ = 0;
  dirty_ = false;
}

// Starts accounting for a new pump session.
void StatisticsManager::onPumpStarted(uint32_t nowMs) {
  lastTickMs_ = nowMs;
  runtimeCarryMs_ = 0;
  dirty_ = true;
}

// Finalizes accounting window on pump stop.
void StatisticsManager::onPumpStopped(uint32_t nowMs) {
  lastTickMs_ = nowMs;
  runtimeCarryMs_ = 0;
  dirty_ = true;
}

// Integrates elapsed time into runtime seconds and estimated liters.
void StatisticsManager::update(AppStatistics &statistics, bool pumpActive, float flowLpm, uint32_t nowMs) {
  if (!pumpActive) {
    lastTickMs_ = nowMs;
    return;
  }

  const uint32_t elapsedMs = nowMs - lastTickMs_;
  if (elapsedMs == 0) {
    return;
  }

  lastTickMs_ = nowMs;
  runtimeCarryMs_ += elapsedMs;

  while (runtimeCarryMs_ >= 1000UL) {
    statistics.totalPumpRuntimeSeconds++;
    runtimeCarryMs_ -= 1000UL;
  }

  statistics.totalWaterLiters += (static_cast<double>(flowLpm) * static_cast<double>(elapsedMs)) / 60000.0;
  dirty_ = true;
}

bool StatisticsManager::dirty() const {
  return dirty_;
}

void StatisticsManager::clearDirty() {
  dirty_ = false;
}