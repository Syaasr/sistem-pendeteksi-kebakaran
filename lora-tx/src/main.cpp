#include <Arduino.h>
#include "variable.h"
#include "LcdHelper.h"
#include "FlameSensor.h"
#include "Mq2Sensor.h"
#include "Dht22Sensor.h"
#include "LoraLink.h"
#include "FireLogic.h"
#include "FireDisplay.h"
#include "Buzzer.h"

LcdHelper lcd(LCD_ADDR, 16, 2, SDA_PIN, SCL_PIN);
FlameSensor flame(FLAME_DO_PIN, FLAME_AO_PIN);
Mq2Sensor mq2(MQ2_AO_PIN, MQ2_THRESHOLD, MQ2_WARMUP_MS);
Dht22Sensor dht(DHT_PIN);
LoraLink lora(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS, LORA_RST, LORA_DIO0, (long)LORA_FREQ, LORA_SYNC);
FireLogic logic(TEMP_ALARM_C);
Buzzer buzzer(BUZZER_PIN, BUZZER_FREQ_ALARM, BUZZER_FREQ_WARN, BUZZER_FREQ_FAULT);

void setup() {
  Serial.begin(115200);

  lcd.printRow(0, 0, "Fire Detector TX");
  lcd.printRow(0, 1, "Memulai...");
  buzzer.beep(BUZZER_FREQ_WARN, 100);

  if (!lora.begin()) {
    Serial.println("LoRa gagal!");
    lcd.printRow(0, 0, "LoRa GAGAL!");
    lcd.printRow(0, 1, "Cek wiring");
    while (1) delay(1000);
  }
}

void loop() {
  static uint32_t lastSend = 0, lastPage = 0;
  static uint8_t page = 0;

  dht.update();

  buzzer.update();

  if (millis() - lastPage >= 2000) {
    lastPage = millis();
    page = (page + 1) % FireDisplay::PAGES;
  }

  if (millis() - lastSend < SEND_INTERVAL) return;
  lastSend = millis();

  SensorPacket p;
  p.flame   = flame.isFlame();
  p.flameAO = flame.readAnalog();
  p.gasAO   = mq2.readAnalog();
  p.gas     = mq2.isGas();
  p.dhtOk   = dht.isValid();
  p.temp    = dht.temperature();
  p.hum     = dht.humidity();
  p.level   = logic.evaluate(p.flame, p.gas, p.dhtOk, p.temp);

  buzzer.setMode(Buzzer::fromLevel(p.level));
  
  FireDisplay::render(lcd, p, page);
  lora.send(p);

  Serial.printf("TX lvl=%d api=%d gas=%d(AO=%d) fAO=%d T=%.1f H=%.1f dht=%d warm=%d\n", p.level, p.flame, p.gas, p.gasAO, p.flameAO, p.temp, p.hum, p.dhtOk, mq2.isWarm());
}