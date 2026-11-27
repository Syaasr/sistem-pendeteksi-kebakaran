#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "../LoraLink/LoraLink.h"

class FirebaseGateway {
public:
  FirebaseGateway();
  void begin(const char* ssid, const char* password, const char* host);
  void update();
  bool pushMonitoring(const SensorPacket &p, int rssi);
  bool pushIncidentLog(const SensorPacket &p, int rssi, const char* note);
  bool isConnected() const;

private:
  const char* _ssid;
  const char* _pass;
  String _host;
  uint32_t _lastReconnect = 0;
  uint32_t _lastLogPush = 0;
  uint8_t  _lastLevel = 0;
};

