#pragma once

#include <Arduino.h>

class RelayManager {
public:
  void begin();
  void setRelay(uint8_t relayNumber, bool enabled);
  bool relayState(uint8_t relayNumber) const;
  void allOff();

private:
  bool relays_[4] = {false, false, false, false};
  bool isValidRelay(uint8_t relayNumber) const;
  int relayPin(uint8_t relayNumber) const;
  bool activeLevel() const;
};