#include "sensors.h"

#include "config.h"

// Initializes both sensor channels from active settings.
void SensorManager::begin(const AppSettings &settings) {
  configureChannel(lower_, LOWER_LEVEL_SENSOR_PIN, settings.lowerSensorLogic);
  configureChannel(upper_, UPPER_LEVEL_SENSOR_PIN, settings.upperSensorLogic);
}

// Updates electrical logic mapping without touching pin configuration.
void SensorManager::applySettings(const AppSettings &settings) {
  lower_.logic = settings.lowerSensorLogic;
  upper_.logic = settings.upperSensorLogic;
}

// Refreshes debounced state for lower and upper channels.
void SensorManager::update(uint32_t nowMs) {
  updateChannel(lower_, nowMs);
  updateChannel(upper_, nowMs);
}

bool SensorManager::lowerTriggered() const {
  return lower_.stableTriggered;
}

bool SensorManager::upperTriggered() const {
  return upper_.stableTriggered;
}

// Converts raw GPIO level to logical trigger based on configured contact mode.
bool SensorManager::sampleTriggered(uint8_t pin, SensorLogic logic) const {
  const bool rawHigh = digitalRead(pin) == HIGH;
  if (logic == SensorLogic::Closed) {
    return !rawHigh;
  }
  return rawHigh;
}

// Configures one channel and seeds debounce state from current input value.
void SensorManager::configureChannel(SensorChannel &channel, uint8_t pin, SensorLogic logic) {
  channel.pin = pin;
  channel.logic = logic;
  pinMode(pin, INPUT_PULLUP);
  channel.lastRawTriggered = sampleTriggered(pin, logic);
  channel.stableTriggered = channel.lastRawTriggered;
  channel.lastChangeMs = millis();
}

// Applies debounce window and updates stable trigger state.
void SensorManager::updateChannel(SensorChannel &channel, uint32_t nowMs) {
  const bool rawTriggered = sampleTriggered(channel.pin, channel.logic);

  if (rawTriggered != channel.lastRawTriggered) {
    channel.lastRawTriggered = rawTriggered;
    channel.lastChangeMs = nowMs;
  }

  if ((nowMs - channel.lastChangeMs) >= debounceMs_ && channel.stableTriggered != rawTriggered) {
    channel.stableTriggered = rawTriggered;
  }
}