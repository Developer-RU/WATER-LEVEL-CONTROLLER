#include "relays.h"

#include "config.h"

void RelayManager::begin() {
  const int pins[4] = {RELAY_1_PIN, RELAY_2_PIN, RELAY_3_PIN, RELAY_4_PIN};
  for (uint8_t index = 0; index < 4; ++index) {
    if (pins[index] >= 0) {
      pinMode(pins[index], OUTPUT);
      // Initialize in OFF state to avoid startup relay pulse.
      digitalWrite(pins[index], activeLevel() ? LOW : HIGH);
    }
    relays_[index] = false;
  }
}

void RelayManager::setRelay(uint8_t relayNumber, bool enabled) {
  if (!isValidRelay(relayNumber)) {
    return;
  }

  relays_[relayNumber - 1] = enabled;
  const int pin = relayPin(relayNumber);
  if (pin >= 0) {
    const bool highLevel = enabled ? activeLevel() : !activeLevel();
    digitalWrite(pin, highLevel ? HIGH : LOW);
  }
}

bool RelayManager::relayState(uint8_t relayNumber) const {
  if (!isValidRelay(relayNumber)) {
    return false;
  }
  return relays_[relayNumber - 1];
}

void RelayManager::allOff() {
  for (uint8_t relayNumber = 1; relayNumber <= 4; ++relayNumber) {
    setRelay(relayNumber, false);
  }
}

bool RelayManager::isValidRelay(uint8_t relayNumber) const {
  return relayNumber >= 1 && relayNumber <= 4;
}

int RelayManager::relayPin(uint8_t relayNumber) const {
  switch (relayNumber) {
    case 1:
      return RELAY_1_PIN;
    case 2:
      return RELAY_2_PIN;
    case 3:
      return RELAY_3_PIN;
    case 4:
      return RELAY_4_PIN;
    default:
      return -1;
  }
}

bool RelayManager::activeLevel() const {
#if RELAY_ACTIVE_LOW
  return false;
#else
  return true;
#endif
}