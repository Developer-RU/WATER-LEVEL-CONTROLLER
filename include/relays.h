#pragma once

#include <Arduino.h>

/**
 * @brief Abstraction over up to four hardware relay channels.
 */
class RelayManager {
public:
  /** @brief Configures relay GPIOs and forces all relays to OFF state. */
  void begin();
  /** @brief Enables or disables relay channel in range 1..4. */
  void setRelay(uint8_t relayNumber, bool enabled);
  /** @brief Returns cached relay state for a channel in range 1..4. */
  bool relayState(uint8_t relayNumber) const;
  /** @brief Turns all relay channels OFF. */
  void allOff();

private:
  bool relays_[4] = {false, false, false, false};
  bool isValidRelay(uint8_t relayNumber) const;
  int relayPin(uint8_t relayNumber) const;
  bool activeLevel() const;
};