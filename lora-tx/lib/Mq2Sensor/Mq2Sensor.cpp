#include "Mq2Sensor.h"

Mq2Sensor::Mq2Sensor(uint8_t aoPin, int threshold, uint32_t warmupMs) : _aoPin(aoPin), _threshold(threshold), _warmupMs(warmupMs) {}

void Mq2Sensor::ensureInit() {
  if (_ready) return;
  pinMode(_aoPin, INPUT);
  analogSetPinAttenuation(_aoPin, ADC_11db);
  _ready = true;
}

int Mq2Sensor::readAnalog() {
  ensureInit();
  long sum = 0;
  for (uint8_t i = 0; i < 8; i++) sum += analogRead(_aoPin);
  return sum / 8;
}

bool Mq2Sensor::isWarm() {
  return millis() >= _warmupMs;
}

bool Mq2Sensor::isGas() {
  int v = readAnalog();
  return isWarm() && v >= _threshold;
}