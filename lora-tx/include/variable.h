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

// Flame sensor (VCC 3.3V)
#define FLAME_DO_PIN 27
#define FLAME_AO_PIN 34

// MQ-2 (VCC 5V, AO lewat pembagi tegangan 10k/15k)
#define MQ2_AO_PIN 35
#define MQ2_THRESHOLD 1800 // ADC 0-4095, kalibrasi sesuai udara bersih
#define MQ2_WARMUP_MS 20000

// DHT22 (VCC 3.3V)
#define DHT_PIN 25

// Logika kebakaran
#define TEMP_ALARM_C 50.0f
#define SEND_INTERVAL 1000

// Buzzer
#define BUZZER_PIN 13
#define BUZZER_FREQ_ALARM 2700
#define BUZZER_FREQ_WARN 2000
#define BUZZER_FREQ_FAULT 1000