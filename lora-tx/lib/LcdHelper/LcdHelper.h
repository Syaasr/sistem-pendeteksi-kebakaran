#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class LcdHelper {
public:
  LcdHelper(uint8_t addr, uint8_t cols, uint8_t rows, uint8_t sda, uint8_t scl);
  void printRow(uint8_t kolom, uint8_t baris, const String &pesan);

private:
  void ensureInit();
  LiquidCrystal_I2C _lcd;
  uint8_t _cols, _sda, _scl;
  bool _ready = false;
};