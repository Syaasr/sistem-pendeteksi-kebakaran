#pragma once
#include <Arduino.h>

class FlameSensor {
public:
  FlameSensor(uint8_t doPin, uint8_t aoPin, bool activeLow = true);
  bool isFlame();
  int  readAnalog();
  int  readDigital();

private:
  void ensureInit();
  uint8_t _doPin, _aoPin;
  bool _activeLow;
  bool _ready = false;
};