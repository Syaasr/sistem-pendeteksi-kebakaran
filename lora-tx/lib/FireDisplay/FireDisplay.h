#pragma once
#include <Arduino.h>
#include "LcdHelper.h"
#include "LoraLink.h"

namespace FireDisplay {
  constexpr uint8_t PAGES = 3;
  void render(LcdHelper &lcd, const SensorPacket &p, uint8_t page);
}