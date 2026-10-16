#pragma once
#include <Arduino.h>

class FireLogic {
public:
  explicit FireLogic(float tempAlarmC) : _tempAlarm(tempAlarmC) {}
  // 0 = AMAN, 1 = WASPADA, 2 = BAHAYA
  uint8_t evaluate(bool flame, bool gas, bool dhtOk, float temp) const;

private:
  float _tempAlarm;
};