#include "FireDisplay.h"

void FireDisplay::render(LcdHelper &lcd, const SensorPacket &p, uint8_t page) {
  // Baris 0: status
  switch (p.level) {
    case 2:  lcd.printRow(0, 0, "!! KEBAKARAN !!"); break;
    case 1:  lcd.printRow(0, 0, "Status: WASPADA"); break;
    default: lcd.printRow(0, 0, "Status: AMAN");    break;
  }

  // Baris 1: bergilir
  switch (page % PAGES) {
    case 0:
      lcd.printRow(0, 1, String("Api:") + (p.flame ? "ADA" : "Tdk") + " Gas:" + (p.gas ? "ADA" : "Tdk"));
      break;
    case 1:
      if (p.dhtOk)
        lcd.printRow(0, 1, "T:" + String(p.temp, 1) + String((char)0xDF) + "C H:" + String((int)p.hum) + "%");
      else
        lcd.printRow(0, 1, "DHT22 error");
      break;
    default:
      lcd.printRow(0, 1, "G:" + String(p.gasAO) + " F:" + String(p.flameAO));
      break;
  }
}