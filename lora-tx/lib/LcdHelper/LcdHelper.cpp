#include "LcdHelper.h"

LcdHelper::LcdHelper(uint8_t addr, uint8_t cols, uint8_t rows, uint8_t sda, uint8_t scl)
  : _lcd(addr, cols, rows), _cols(cols), _sda(sda), _scl(scl) {}

void LcdHelper::ensureInit() {
  if (_ready) return;
  Wire.begin(_sda, _scl);
  _lcd.init();
  _lcd.backlight();
  _ready = true;
}

void LcdHelper::printRow(uint8_t kolom, uint8_t baris, const String &pesan) {
  ensureInit();
  if (kolom >= _cols) return;

  uint8_t sisa = _cols - kolom;
  String teks = pesan.substring(0, sisa);
  while (teks.length() < sisa) teks += ' ';

  _lcd.setCursor(kolom, baris);
  _lcd.print(teks);
}