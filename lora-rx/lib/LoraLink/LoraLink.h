#pragma once
#include <Arduino.h>

// Level: 0 = AMAN, 1 = WASPADA, 2 = BAHAYA
struct SensorPacket {
  uint8_t  level    = 0;
  bool     flame    = false;
  bool     gas      = false;
  bool     dhtOk    = false;
  uint16_t flameAO  = 0;
  uint16_t gasAO    = 0;
  float    temp     = 0;
  float    hum      = 0;
};

class LoraLink {
public:
  LoraLink(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t ss, uint8_t rst, uint8_t dio0, long freq, uint8_t sync);
  bool begin();
  void send(const SensorPacket &p);
  bool receive(SensorPacket &p);   // true jika ada paket valid
  int  rssi();

private:
  uint8_t _sck, _miso, _mosi, _ss, _rst, _dio0, _sync;
  long _freq;
};