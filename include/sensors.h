#pragma once

#include <Arduino.h>

#include "app_types.h"

class SensorManager {
public:
  void begin(const AppSettings &settings);
  void applySettings(const AppSettings &settings);
  void update(uint32_t nowMs);
  bool lowerTriggered() const;
  bool upperTriggered() const;

private:
  static constexpr uint32_t debounceMs_ = 50;

  struct SensorChannel {
    uint8_t pin = 0;
    SensorLogic logic = SensorLogic::Closed;
    bool stableTriggered = false;
    bool lastRawTriggered = false;
    uint32_t lastChangeMs = 0;
  };

  SensorChannel lower_;
  SensorChannel upper_;

  bool sampleTriggered(uint8_t pin, SensorLogic logic) const;
  void configureChannel(SensorChannel &channel, uint8_t pin, SensorLogic logic);
  void updateChannel(SensorChannel &channel, uint32_t nowMs);
};