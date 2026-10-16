#include "Dht22Sensor.h"

Dht22Sensor::Dht22Sensor(uint8_t pin) : _dht(pin, DHT22) {}

void Dht22Sensor::ensureInit() {
  if (_ready) return;
  _dht.begin();
  _ready = true;
}

void Dht22Sensor::update() {
  ensureInit();
  if (_last != 0 && millis() - _last < 2000) return;
  _last = millis();

  float t = _dht.readTemperature();
  float h = _dht.readHumidity();
  _ok = !(isnan(t) || isnan(h));
  if (_ok) { _t = t; _h = h; }
}