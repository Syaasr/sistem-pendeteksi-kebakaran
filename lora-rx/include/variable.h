#pragma once

// LCD I2C (lewat level converter: LV1=SDA, LV2=SCL)
#define SDA_PIN 21
#define SCL_PIN 22
#define LCD_ADDR 0x27

// LoRa Ra-02 SX1278
#define LORA_SCK 18
#define LORA_MISO 19
#define LORA_MOSI 23
#define LORA_SS 5
#define LORA_RST 4
#define LORA_DIO0 26
#define LORA_FREQ 433E6
#define LORA_SYNC 0xF3

// Link
#define LINK_TIMEOUT_MS 5000

// Buzzer
#define BUZZER_PIN 13
#define BUZZER_FREQ_ALARM 2700   // Hz, sekitar resonansi buzzer pasif
#define BUZZER_FREQ_WARN 2000
#define BUZZER_FREQ_FAULT 1000

// LED Indikator Internal ESP32 (sesuai VIDEO-TUTORIAL-BLYNK.md)
#define LED_PIN 2

// Konfigurasi Blynk IoT & Wi-Fi (isi sesuai perangkat di Blynk Console)
#define BLYNK_TEMPLATE_ID   "TMPL6wIYQtypH"
#define BLYNK_TEMPLATE_NAME "WMC"
#define BLYNK_AUTH_TOKEN    "m5XhDj5WlxFIhaksd2c6HfJGy8wtf5y6"

#define WIFI_SSID           "ISI_NAMA_WIFI"
#define WIFI_PASS           "ISI_PASSWORD_WIFI"

#define WIFI_RECONNECT_MS   10000
#define BLYNK_RECONNECT_MS  10000

// Pemetaan Virtual Pin Blynk (Free Plan - 6 DataStream)
#define VPIN_STATUS_LEVEL   V0   // Integer (0=AMAN, 1=WASPADA, 2=BAHAYA, 3=LINK PUTUS)
#define VPIN_TEMP           V1   // Double  (Suhu °C)
#define VPIN_HUM            V2   // Double  (Kelembapan %)
#define VPIN_GAS_AO         V3   // Integer (ADC MQ-2 0-4095)
#define VPIN_FLAME          V4   // Integer (0=Tidak ada api, 1=Api terdeteksi)
#define VPIN_MUTE_SWITCH    V5   // Integer (0=Normal, 1=Mute Buzzer / Kontrol LED GPIO 2)