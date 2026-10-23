#pragma once
#include <Arduino.h>
#include "LoraLink.h"

class BlynkLink {
public:
  BlynkLink(const char *ssid, const char *pass, const char *auth,
            uint8_t ledPin, uint32_t wifiRetryMs, uint32_t blynkRetryMs);

  void begin();                                      // inisialisasi Wi-Fi & Blynk non-blocking
  void update();                                     // WAJIB dipanggil tiap loop (non-blocking)
  void publishTelemetry(const SensorPacket &p);      // kirim V0-V4 saat paket LoRa diterima
  void publishLinkFault(uint8_t lastLevel);          // kirim status link putus ke V0

  bool isConnected() const;                          // true jika terhubung ke server Blynk
  bool isMuted() const { return _muted; }            // true jika Switch Mute (V5) aktif
  void onLevelChanged(uint8_t prevLevel, uint8_t newLevel); // auto-unmute saat naik ke BAHAYA / reset saat AMAN

  // Dipanggil dari callback BLYNK_WRITE(V5)
  void handleRemoteSwitch(bool state);

private:
  void syncLed();
  bool hasValidCredentials() const;

  const char *_ssid;
  const char *_pass;
  const char *_auth;
  uint8_t     _ledPin;
  uint32_t    _wifiRetryMs;
  uint32_t    _blynkRetryMs;

  uint32_t _lastWifiAttempt  = 0;
  uint32_t _lastBlynkAttempt = 0;
  uint8_t  _currentLevel     = 0;
  bool     _muted            = false;
  bool     _ready            = false;
  bool     _wasConnected     = false;
};
