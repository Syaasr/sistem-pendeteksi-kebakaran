#pragma once
#include <Arduino.h>

class Mq2Sensor {
public:
  Mq2Sensor(uint8_t aoPin, int threshold, uint32_t warmupMs);
  int  readAnalog(); // rata-rata 8 sampel
  bool isWarm(); // true setelah pemanasan selesai
  bool isGas(); // false selama pemanasan

private:
  void ensureInit();
  uint8_t _aoPin;
  int _threshold;
  uint32_t _warmupMs;
  bool _ready = false;
};