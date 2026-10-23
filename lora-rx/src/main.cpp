#include <Arduino.h>
#include "variable.h"
#include "LcdHelper.h"
#include "LoraLink.h"
#include "FireDisplay.h"
#include "Buzzer.h"
#include "BlynkLink.h"

LcdHelper lcd(LCD_ADDR, 16, 2, SDA_PIN, SCL_PIN);
LoraLink  lora(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS, LORA_RST, LORA_DIO0, (long)LORA_FREQ, LORA_SYNC);
Buzzer    buzzer(BUZZER_PIN, BUZZER_FREQ_ALARM, BUZZER_FREQ_WARN, BUZZER_FREQ_FAULT);
BlynkLink blynk(WIFI_SSID, WIFI_PASS, BLYNK_AUTH_TOKEN, LED_PIN, WIFI_RECONNECT_MS, BLYNK_RECONNECT_MS);

SensorPacket last;
uint32_t lastRx = 0;
bool     linkOk = true;

static void applyBuzzerState() {
  if (blynk.isMuted()) {
    buzzer.setMode(BuzzerMode::OFF);
    return;
  }
  if (!linkOk && lastRx != 0) {
    buzzer.setMode(last.level == 2 ? BuzzerMode::BAHAYA : BuzzerMode::LINK_PUTUS);
    return;
  }
  buzzer.setMode(Buzzer::fromLevel(last.level));
}

void setup() {
  Serial.begin(115200);
  buzzer.beep(BUZZER_FREQ_WARN, 100);

  if (!lora.begin()) {
    Serial.println("LoRa gagal!");
    lcd.printRow(0, 0, "LoRa GAGAL!");
    lcd.printRow(0, 1, "Cek wiring");
    while (1) delay(1000);
  }

  blynk.begin();

  lcd.printRow(0, 0, "Menunggu data...");
  lcd.printRow(0, 1, "");
}

void loop() {
  static uint32_t lastPage = 0;
  static uint8_t page = 0;

  // Pastikan komunikasi Blynk dan pola suara Buzzer selalu di-update secara non-blocking
  blynk.update();
  applyBuzzerState();
  buzzer.update();

  uint8_t prevLevel = last.level;
  bool newData = lora.receive(last);
  if (newData) {
    lastRx = millis();
    linkOk = true;
    if (last.level != prevLevel) {
      blynk.onLevelChanged(prevLevel, last.level);
    }
    applyBuzzerState();
    blynk.publishTelemetry(last);
    Serial.printf("RX lvl=%d api=%d gas=%d(AO=%d) fAO=%d T=%.1f H=%.1f RSSI=%d dBm blynk=%d mute=%d\n",
                  last.level, last.flame, last.gas, last.gasAO, last.flameAO,
                  last.temp, last.hum, lora.rssi(), blynk.isConnected(), blynk.isMuted());
  }

  if (linkOk && lastRx != 0 && millis() - lastRx > LINK_TIMEOUT_MS) {
    linkOk = false;
    lcd.printRow(0, 0, "Link putus");
    lcd.printRow(0, 1, "Cek TX");
    applyBuzzerState();
    blynk.publishLinkFault(last.level);
  }

  if (!linkOk || lastRx == 0) return;

  bool pageChanged = false;
  if (millis() - lastPage >= 2000) {
    lastPage = millis();
    page = (page + 1) % FireDisplay::PAGES;
    pageChanged = true;
  }

  if (newData || pageChanged) FireDisplay::render(lcd, last, page);
}
