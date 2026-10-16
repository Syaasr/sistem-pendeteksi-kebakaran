#pragma once
#include <Arduino.h>

enum class BuzzerMode : uint8_t { OFF, WASPADA, BAHAYA, LINK_PUTUS };

class Buzzer {
public:
  Buzzer(uint8_t pin, uint16_t fAlarm, uint16_t fWarn, uint16_t fFault);

  void setMode(BuzzerMode m);          // ganti pola (aman dipanggil berulang)
  void update();                       // WAJIB dipanggil tiap loop (non-blocking)
  void beep(uint16_t freq, uint16_t ms); // blocking, hanya untuk self-test di setup

  static BuzzerMode fromLevel(uint8_t level);  // 0=OFF, 1=WASPADA, 2=BAHAYA

private:
  void ensureInit();
  void silence();

  uint8_t  _pin;
  uint16_t _fAlarm, _fWarn, _fFault;
  BuzzerMode _mode = BuzzerMode::OFF;
  uint8_t  _idx = 0;
  uint32_t _stepStart = 0;
  bool _started = false, _toneOn = false, _ready = false;
};