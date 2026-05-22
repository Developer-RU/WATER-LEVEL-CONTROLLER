#pragma once

#include <Arduino.h>

/**
 * @brief Handles OTA firmware upload lifecycle and deferred reboot.
 */
class OtaManager {
public:
  /** @brief Initializes OTA state machine. */
  void begin();
  /** @brief Starts OTA upload session before first chunk arrives. */
  bool handleUploadStart(size_t contentLength, const String &filename);
  /** @brief Writes a chunk of binary data to flash. */
  bool handleUploadChunk(uint8_t *data, size_t len);
  /** @brief Finalizes update image and validates partition switch. */
  bool handleUploadEnd();
  /** @brief Schedules MCU restart after a delay in milliseconds. */
  void scheduleRestart(uint32_t delayMs);
  /** @brief Executes deferred reboot logic in main loop context. */
  void loop();
  /** @brief Returns true when last upload operation failed. */
  bool hasError() const;
  /** @brief Returns current OTA error message, if any. */
  const String &errorMessage() const;

private:
  bool started_ = false;
  bool restartScheduled_ = false;
  uint32_t restartAtMs_ = 0;
  String errorMessage_;
};