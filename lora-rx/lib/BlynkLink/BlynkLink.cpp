#include "variable.h"
#include "BlynkLink.h"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <cstring>

static BlynkLink *s_instance = nullptr;

// Handler ketika tombol Switch pada dashboard Blynk (Virtual Pin V5) ditekan
BLYNK_WRITE(VPIN_MUTE_SWITCH) {
  if (s_instance) {
    bool state = param.asInt() != 0;
    s_instance->handleRemoteSwitch(state);
  }
}

// Sinkronisasi state awal saat baru tersambung ke server Blynk
BLYNK_CONNECTED() {
  Blynk.syncVirtual(VPIN_MUTE_SWITCH);
}

BlynkLink::BlynkLink(const char *ssid, const char *pass, const char *auth,
                     uint8_t ledPin, uint32_t wifiRetryMs, uint32_t blynkRetryMs)
  : _ssid(ssid), _pass(pass), _auth(auth), _ledPin(ledPin),
    _wifiRetryMs(wifiRetryMs), _blynkRetryMs(blynkRetryMs) {
  s_instance = this;
}

bool BlynkLink::hasValidCredentials() const {
  if (!_ssid || !_auth) return false;
  if (std::strlen(_ssid) == 0 || std::strcmp(_ssid, "ISI_NAMA_WIFI") == 0) return false;
  if (std::strlen(_auth) == 0 || std::strcmp(_auth, "ISI_BLYNK_AUTH_TOKEN") == 0) return false;
  return true;
}

void BlynkLink::begin() {
  pinMode(_ledPin, OUTPUT);
  digitalWrite(_ledPin, LOW);

  if (!hasValidCredentials()) {
    Serial.println("[Blynk] Kredensial Wi-Fi/Blynk masih placeholder di variable.h (mode offline lokal aktif).");
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(_ssid, _pass);
  Blynk.config(_auth);

  _lastWifiAttempt  = millis();
  _lastBlynkAttempt = millis();
  _ready = true;
  Serial.printf("[Blynk] Menghubungkan Wi-Fi ke '%s' (non-blocking)...\n", _ssid);
}

void BlynkLink::update() {
  if (!_ready) return;

  uint32_t now = millis();

  // 1. Pastikan Wi-Fi tersambung secara non-blocking
  if (WiFi.status() != WL_CONNECTED) {
    _wasConnected = false;
    if (now - _lastWifiAttempt >= _wifiRetryMs) {
      _lastWifiAttempt = now;
      Serial.println("[Blynk] Mencoba rekoneksi Wi-Fi...");
      WiFi.disconnect();
      WiFi.begin(_ssid, _pass);
    }
    return;
  }

  // 2. Jika Wi-Fi tersambung tetapi Blynk belum konek, coba konek dengan timeout singkat
  if (!Blynk.connected()) {
    _wasConnected = false;
    if (now - _lastBlynkAttempt >= _blynkRetryMs) {
      _lastBlynkAttempt = now;
      Serial.println("[Blynk] Wi-Fi OK, menghubungkan ke server Blynk...");
      if (Blynk.connect(500)) {
        _wasConnected = true;
        Serial.println("[Blynk] Terhubung ke Blynk Cloud!");
      }
    }
    return;
  }

  if (!_wasConnected) {
    _wasConnected = true;
    Serial.println("[Blynk] Terhubung ke Blynk Cloud!");
  }

  Blynk.run();
}

void BlynkLink::publishTelemetry(const SensorPacket &p) {
  _currentLevel = p.level;
  syncLed();

  if (!_ready || !Blynk.connected()) return;

  Blynk.virtualWrite(VPIN_STATUS_LEVEL, p.level);
  if (p.dhtOk) {
    Blynk.virtualWrite(VPIN_TEMP, p.temp);
    Blynk.virtualWrite(VPIN_HUM, p.hum);
  }
  Blynk.virtualWrite(VPIN_GAS_AO, p.gasAO);
  Blynk.virtualWrite(VPIN_FLAME, p.flame ? 1 : 0);
}

void BlynkLink::publishLinkFault(uint8_t lastLevel) {
  _currentLevel = lastLevel;
  syncLed();

  if (!_ready || !Blynk.connected()) return;
  // Nilai 3 menandakan LINK PUTUS jika sebelumnya tidak sedang BAHAYA (2)
  Blynk.virtualWrite(VPIN_STATUS_LEVEL, lastLevel == 2 ? 2 : 3);
}

void BlynkLink::onLevelChanged(uint8_t prevLevel, uint8_t newLevel) {
  _currentLevel = newLevel;

  // Demi keselamatan: jika eskalasi naik ke BAHAYA (2), batalkan mute otomatis
  if (newLevel == 2 && prevLevel < 2 && _muted) {
    _muted = false;
    Serial.println("[Blynk] Eskalasi ke BAHAYA -> Auto-unmute buzzer!");
    if (_ready && Blynk.connected()) {
      Blynk.virtualWrite(VPIN_MUTE_SWITCH, 0);
    }
  }
  // Jika kondisi sudah kembali AMAN (0), reset status mute ke 0
  else if (newLevel == 0 && prevLevel > 0 && _muted) {
    _muted = false;
    if (_ready && Blynk.connected()) {
      Blynk.virtualWrite(VPIN_MUTE_SWITCH, 0);
    }
  }

  syncLed();
}

void BlynkLink::handleRemoteSwitch(bool state) {
  _muted = state;
  syncLed();
  Serial.printf("[Blynk] Switch V5 diterima: %s\n", _muted ? "ON (Mute Buzzer / LED ON)" : "OFF (Normal)");
}

void BlynkLink::syncLed() {
  // LED GPIO 2 menyala jika ada bahaya/waspada ATAU jika diaktifkan lewat Switch V5 di Blynk
  bool ledOn = (_currentLevel > 0) || _muted;
  digitalWrite(_ledPin, ledOn ? HIGH : LOW);
}

bool BlynkLink::isConnected() const {
  return _ready && Blynk.connected();
}
