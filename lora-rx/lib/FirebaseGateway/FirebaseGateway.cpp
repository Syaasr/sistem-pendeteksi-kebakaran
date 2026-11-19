#include "FirebaseGateway.h"

FirebaseGateway::FirebaseGateway() : _ssid(nullptr), _pass(nullptr) {}

void FirebaseGateway::begin(const char* ssid, const char* password, const char* host) {
  _ssid = ssid;
  _pass = password;
  _host = host;

  if (_host.endsWith("/")) {
    _host.remove(_host.length() - 1);
  }

  Serial.println("\n[Gateway] Memulai koneksi Wi-Fi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid, _pass);

  // Tunggu koneksi sebentar (maksimal 5 detik agar tidak memblokir LoRa jika Wi-Fi belum siap)
  uint32_t startMs = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startMs < 5000) {
    delay(200);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\n[Gateway] Wi-Fi Terhubung! IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\n[Gateway] Wi-Fi belum tersambung, akan terus mencoba di background.");
  }
}

void FirebaseGateway::update() {
  // Auto-reconnect non-blocking di background setiap 10 detik jika Wi-Fi terputus
  if (WiFi.status() != WL_CONNECTED && millis() - _lastReconnect > 10000) {
    _lastReconnect = millis();
    Serial.println("[Gateway] Mencoba menghubungkan kembali ke Wi-Fi...");
    WiFi.disconnect();
    WiFi.reconnect();
  }
}

bool FirebaseGateway::isConnected() const {
  return (WiFi.status() == WL_CONNECTED);
}

bool FirebaseGateway::pushMonitoring(const SensorPacket &p, int rssi) {
  if (!isConnected()) return false;

  WiFiClientSecure client;
  client.setInsecure(); // Bypass verifikasi sertifikat SSL agar cepat dan stabil

  HTTPClient https;
  String url = _host + "/monitoring.json";

  if (!https.begin(client, url)) {
    Serial.println("[Firebase] Gagal memulai koneksi HTTPS");
    return false;
  }

  https.addHeader("Content-Type", "application/json");

  // Format JSON payload
  char payload[300];
  snprintf(payload, sizeof(payload),
    "{\"nodeId\":\"TX-01\",\"level\":%u,\"flame\":%d,\"gas\":%d,\"dhtOk\":%d,"
    "\"flameAO\":%u,\"gasAO\":%u,\"temp\":%.1f,\"hum\":%.1f,\"rssi\":%d,\"lastUpdate\":\"%lu ms\"}",
    p.level,
    p.flame ? 1 : 0,
    p.gas ? 1 : 0,
    p.dhtOk ? 1 : 0,
    p.flameAO,
    p.gasAO,
    p.temp,
    p.hum,
    rssi,
    millis()
  );

  int httpCode = https.PATCH(payload);
  bool success = (httpCode == 200 || httpCode == 204);

  if (success) {
    Serial.printf("[Firebase] Data live terkirim! (HTTP %d)\n", httpCode);
  } else {
    Serial.printf("[Firebase] Gagal kirim data live: HTTP %d\n", httpCode);
  }

  https.end();

  // Jika terdeteksi kondisi bahaya atau waspada, kirim log insiden ke /logs.json
  if (p.level > 0 && (p.level != _lastLevel || millis() - _lastLogPush > 15000)) {
    const char* note = (p.level == 2) ? "Bahaya: Api Terdeteksi / Asap Ekstrem" : "Waspada: Peningkatan Suhu/Asap";
    pushIncidentLog(p, rssi, note);
    _lastLogPush = millis();
  }

  _lastLevel = p.level;
  return success;
}

bool FirebaseGateway::pushIncidentLog(const SensorPacket &p, int rssi, const char* note) {
  if (!isConnected()) return false;

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient https;
  String url = _host + "/logs.json";

  if (!https.begin(client, url)) return false;

  https.addHeader("Content-Type", "application/json");

  char logPayload[350];
  snprintf(logPayload, sizeof(logPayload),
    "{\"level\":%u,\"flame\":%d,\"temp\":%.1f,\"hum\":%.1f,\"gasAO\":%u,\"rssi\":%d,\"note\":\"%s\",\"timestamp\":\"Waktu TX ms: %lu\"}",
    p.level,
    p.flame ? 1 : 0,
    p.temp,
    p.hum,
    p.gasAO,
    rssi,
    note,
    millis()
  );

  int httpCode = https.POST(logPayload);
  bool success = (httpCode == 200 || httpCode == 204);

  if (success) {
    Serial.printf("[Firebase] Catatan insiden /logs tersimpan! (HTTP %d)\n", httpCode);
  }

  https.end();
  return success;
}

