#pragma once
#include <Arduino.h>
#include <DHT.h>

class Dht22Sensor {
public:
  explicit Dht22Sensor(uint8_t pin);
  void  update();          // aman dipanggil tiap loop (baca max tiap 2 detik)
  float temperature() const { return _t; }
  float humidity() const    { return _h; }
  bool  isValid() const     { return _ok; }

private:
  void ensureInit();
  DHT _dht;
  float _t = 0, _h = 0;
  bool _ok = false, _ready = false;
  uint32_t _last = 0;
};