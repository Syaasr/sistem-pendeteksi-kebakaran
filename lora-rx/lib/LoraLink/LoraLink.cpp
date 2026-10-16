#include "LoraLink.h"
#include <SPI.h>
#include <LoRa.h>

// Format paket:
// K,<level>,<flame>,<gas>,<dhtOk>,<flameAO>,<gasAO>,<suhu>,<lembap>

LoraLink::LoraLink(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t ss, uint8_t rst, uint8_t dio0, long freq, uint8_t sync)
  : _sck(sck), _miso(miso), _mosi(mosi), _ss(ss), _rst(rst), _dio0(dio0),
    _sync(sync), _freq(freq) {}

bool LoraLink::begin() {
  SPI.begin(_sck, _miso, _mosi, _ss);
  LoRa.setPins(_ss, _rst, _dio0);
  if (!LoRa.begin(_freq)) return false;
  LoRa.setSyncWord(_sync);
  LoRa.enableCrc();
  return true;
}

void LoraLink::send(const SensorPacket &p) {
  LoRa.beginPacket();
  LoRa.print("K,");
  LoRa.print(p.level);            LoRa.print(',');
  LoRa.print(p.flame ? 1 : 0);    LoRa.print(',');
  LoRa.print(p.gas ? 1 : 0);      LoRa.print(',');
  LoRa.print(p.dhtOk ? 1 : 0);    LoRa.print(',');
  LoRa.print(p.flameAO);          LoRa.print(',');
  LoRa.print(p.gasAO);            LoRa.print(',');
  LoRa.print(p.temp, 1);          LoRa.print(',');
  LoRa.print(p.hum, 1);
  LoRa.endPacket();
}

bool LoraLink::receive(SensorPacket &p) {
  if (!LoRa.parsePacket()) return false;

  String msg;
  while (LoRa.available()) msg += (char)LoRa.read();
  if (!msg.startsWith("K,")) return false;

  String f[8];
  int start = 2;
  for (uint8_t i = 0; i < 8; i++) {
    int c = msg.indexOf(',', start);
    if (c < 0) {
      if (i != 7) return false; // field kurang
      c = msg.length();
    }
    f[i] = msg.substring(start, c);
    start = c + 1;
  }

  p.level   = f[0].toInt();
  p.flame   = f[1].toInt() == 1;
  p.gas     = f[2].toInt() == 1;
  p.dhtOk   = f[3].toInt() == 1;
  p.flameAO = f[4].toInt();
  p.gasAO   = f[5].toInt();
  p.temp    = f[6].toFloat();
  p.hum     = f[7].toFloat();
  return true;
}

int LoraLink::rssi() { return LoRa.packetRssi(); }