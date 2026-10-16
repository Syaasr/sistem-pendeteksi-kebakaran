#include "Buzzer.h"

// Setiap pola = durasi (ms) bergantian: indeks genap = bunyi, ganjil = diam.
// BAHAYA : Temporal-3 (ISO 8201) -> 3x (0,5 s bunyi, 0,5 s diam), jeda 1,5 s
static const uint16_t P_BAHAYA[]  = {500, 500, 500, 500, 500, 1500};
// WASPADA: 1 bip pendek tiap 3 detik
static const uint16_t P_WASPADA[] = {300, 2700};
// LINK PUTUS: kicauan sangat pendek tiap 5 detik
static const uint16_t P_FAULT[]   = {100, 4900};

Buzzer::Buzzer(uint8_t pin, uint16_t fAlarm, uint16_t fWarn, uint16_t fFault)
  : _pin(pin), _fAlarm(fAlarm), _fWarn(fWarn), _fFault(fFault) {}

void Buzzer::ensureInit() {
  if (_ready) return;
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  _ready = true;
}

void Buzzer::silence() {
  ensureInit();
  if (_toneOn) { noTone(_pin); _toneOn = false; }
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);   // jangan biarkan DC mengalir ke buzzer
}

BuzzerMode Buzzer::fromLevel(uint8_t level) {
  if (level >= 2) return BuzzerMode::BAHAYA;
  if (level == 1) return BuzzerMode::WASPADA;
  return BuzzerMode::OFF;
}

void Buzzer::setMode(BuzzerMode m) {
  if (m == _mode) return;
  _mode = m;
  _idx = 0;
  _started = false;
  silence();
}

void Buzzer::update() {
  if (_mode == BuzzerMode::OFF) return;

  const uint16_t *p;
  uint8_t n;
  uint16_t f;
  switch (_mode) {
    case BuzzerMode::BAHAYA:     p = P_BAHAYA;  n = 6; f = _fAlarm; break;
    case BuzzerMode::WASPADA:    p = P_WASPADA; n = 2; f = _fWarn;  break;
    default:                     p = P_FAULT;   n = 2; f = _fFault; break;
  }

  uint32_t now = millis();
  if (_started && now - _stepStart < p[_idx]) return;

  if (_started) _idx = (_idx + 1) % n;
  _started = true;
  _stepStart = now;

  if (_idx % 2 == 0) {
    tone(_pin, f);
    _toneOn = true;
  } else {
    silence();
  }
}

void Buzzer::beep(uint16_t freq, uint16_t ms) {
  ensureInit();
  tone(_pin, freq);
  _toneOn = true;
  delay(ms);
  silence();
}