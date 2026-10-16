#include "FlameSensor.h"

FlameSensor::FlameSensor(uint8_t doPin, uint8_t aoPin, bool activeLow)
  : _doPin(doPin), _aoPin(aoPin), _activeLow(activeLow) {}

void FlameSensor::ensureInit() {
  if (_ready) return;
  pinMode(_doPin, INPUT);
  _ready = true;
}

int FlameSensor::readDigital() {
  ensureInit();
  return digitalRead(_doPin);
}

bool FlameSensor::isFlame() {
  return (readDigital() == (_activeLow ? LOW : HIGH));
}

int FlameSensor::readAnalog() {
  ensureInit();
  return analogRead(_aoPin);
}