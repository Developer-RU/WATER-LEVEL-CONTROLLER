#pragma once

#include <Arduino.h>

class OtaManager {
public:
  void begin();
  bool handleUploadStart(size_t contentLength, const String &filename);
  bool handleUploadChunk(uint8_t *data, size_t len);
  bool handleUploadEnd();
  void scheduleRestart(uint32_t delayMs);
  void loop();
  bool hasError() const;
  const String &errorMessage() const;

private:
  bool started_ = false;
  bool restartScheduled_ = false;
  uint32_t restartAtMs_ = 0;
  String errorMessage_;
};