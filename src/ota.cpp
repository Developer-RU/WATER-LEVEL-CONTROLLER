#include "ota.h"

#include <Update.h>

void OtaManager::begin() {
  started_ = true;
  restartScheduled_ = false;
  restartAtMs_ = 0;
  errorMessage_.clear();
}

bool OtaManager::handleUploadStart(size_t contentLength, const String &filename) {
  (void)contentLength;
  errorMessage_.clear();

  if (!started_) {
    errorMessage_ = "OTA manager is not initialized";
    return false;
  }

  Serial.printf("OTA upload started: %s\n", filename.c_str());
  if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH)) {
    errorMessage_ = Update.errorString();
    return false;
  }

  return true;
}

bool OtaManager::handleUploadChunk(uint8_t *data, size_t len) {
  if (errorMessage_.length() > 0) {
    return false;
  }

  const size_t written = Update.write(data, len);
  if (written != len) {
    errorMessage_ = Update.errorString();
    Update.abort();
    return false;
  }

  return true;
}

bool OtaManager::handleUploadEnd() {
  if (errorMessage_.length() > 0) {
    return false;
  }

  if (!Update.end(true)) {
    errorMessage_ = Update.errorString();
    return false;
  }

  Serial.println("OTA upload completed successfully");
  return true;
}

void OtaManager::scheduleRestart(uint32_t delayMs) {
  restartScheduled_ = true;
  restartAtMs_ = millis() + delayMs;
}

void OtaManager::loop() {
  if (restartScheduled_ && millis() >= restartAtMs_) {
    Serial.println("Restarting after OTA");
    ESP.restart();
  }
}

bool OtaManager::hasError() const {
  return errorMessage_.length() > 0;
}

const String &OtaManager::errorMessage() const {
  return errorMessage_;
}