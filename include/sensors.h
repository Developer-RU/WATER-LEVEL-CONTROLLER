#pragma once

#include <Arduino.h>

#include "app_types.h"

/**
 * @brief Debounced sampling for lower/upper dry-contact level sensors.
 */
class SensorManager {
public:
  /** @brief Configures sensor pins and applies logic from settings. */
  void begin(const AppSettings &settings);
  /** @brief Updates trigger logic without reinitializing GPIOs. */
  void applySettings(const AppSettings &settings);
  /** @brief Samples and debounces both channels. */
  void update(uint32_t nowMs);
  /** @brief Returns debounced trigger state of lower sensor. */
  bool lowerTriggered() const;
  /** @brief Returns debounced trigger state of upper sensor. */
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