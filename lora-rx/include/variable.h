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