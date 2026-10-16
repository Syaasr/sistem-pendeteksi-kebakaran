#include "FireLogic.h"

uint8_t FireLogic::evaluate(bool flame, bool gas, bool dhtOk, float temp) const {
  bool panas = dhtOk && temp >= _tempAlarm;

  if (flame)        return 2; // api terlihat langsung
  if (gas && panas) return 2; // asap + panas
  if (gas || panas) return 1; // salah satu saja
  return 0;
}