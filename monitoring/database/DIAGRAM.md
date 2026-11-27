# 📊 Dokumentasi & Diagram Database — FireGuard LoRa

Dokumen ini memuat **Diagram Skema Database (ERD / NoSQL Schema)** dan **Kamus Data (Data Dictionary)** untuk sistem pendeteksi kebakaran berbasis LoRa IoT pada mata kuliah Wireless & Mobile Computing (WMC).

---

## 1. Diagram Skema Database (Entity-Relationship / Schema Diagram)

```mermaid
erDiagram
    FIREBASE_REALTIME_DATABASE {
        string project_id "sistem-pendeteksi-kebaka-bac09"
        string database_url "https://...asia-southeast1.firebasedatabase.app"
    }

    MONITORING_NODE {
        string nodeId "ID Transmitter (contoh: TX-01)"
        int level "0: Aman, 1: Waspada, 2: Bahaya"
        float temp "Suhu Lingkungan (°C) - DHT22"
        float hum "Kelembaban Relatif (%) - DHT22"
        int gas "Status Digital Sensor MQ-2 (0 / 1)"
        int gasAO "Nilai Analog ADC MQ-2 (0 - 4095)"
        int flame "Status Digital Api (0: Padam, 1: Api)"
        int flameAO "Nilai Analog ADC Flame (0 - 4095)"
        int rssi "Kekuatan Sinyal LoRa (dBm)"
        string lastUpdate "Waktu Paket Terakhir Masuk"
    }

    LOGS_COLLECTION {
        string log_id PK "Unique Key (Push ID Firebase)"
        string timestamp "Waktu Kejadian (YYYY-MM-DD HH:mm:ss)"
        string nodeId "ID Node Sumber Data"
        int level "Tingkat Bahaya saat Kejadian"
        float temp "Nilai Suhu saat Kejadian (°C)"
        float hum "Nilai Kelembaban saat Kejadian (%)"
        int gasAO "Kadar Gas Analog saat Kejadian"
        int flame "Status Sensor Api (0 / 1)"
        int rssi "Kekuatan Sinyal LoRa (dBm)"
        string note "Keterangan / Deskripsi Anomali"
    }

    FIREBASE_REALTIME_DATABASE ||--|| MONITORING_NODE : "Menyimpan Data Realtime (/monitoring)"
    FIREBASE_REALTIME_DATABASE ||--o{ LOGS_COLLECTION : "Menyimpan Riwayat Kejadian (/logs)"
```

---

## 2. Diagram Alur Data Sistem (Data Flow Architecture)

```mermaid
flowchart TD
    subgraph SENSOR_NODE ["1. Node Sensor (Transmitter TX)"]
        S1["Sensor DHT22<br/>(Suhu & Kelembaban)"]
        S2["Sensor MQ-2<br/>(Gas & Asap)"]
        S3["Flame Sensor<br/>(Deteksi Api)"]
        TX_MCU["ESP32 Transmitter<br/>(Klasifikasi Level Bahaya)"]
        LORA_TX["Modul LoRa SX1278<br/>(433 MHz Radio)"]
        
        S1 --> TX_MCU
        S2 --> TX_MCU
        S3 --> TX_MCU
        TX_MCU --> LORA_TX
    end

    subgraph GATEWAY_NODE ["2. Gateway (Receiver RX)"]
        LORA_RX["Modul LoRa SX1278<br/>(Penerima Radio)"]
        RX_MCU["ESP32 Gateway Receiver<br/>(LCD 16x2 + Buzzer Alarm)"]
        WIFI["Modul Wi-Fi Internal ESP32"]

        LORA_RX --> RX_MCU
        RX_MCU --> WIFI
    end

    subgraph CLOUD_DATABASE ["3. Cloud Database (Firebase Realtime DB)"]
        PATH_MONITORING["/monitoring<br/>(Data Real-Time Terkini)"]
        PATH_LOGS["/logs<br/>(Riwayat Kejadian / Audit Trail)"]
    end

    subgraph CLIENT_APPS ["4. Antarmuka Pengguna (Web Monitoring)"]
        DASHBOARD["Web Dashboard<br/>(monitoring/web/index.html)"]
        TABLE_VIEWER["Database Table Explorer<br/>(monitoring/database/table-viewer.html)"]
    end

    LORA_TX -.->|"LoRa RF 433 MHz (~1-3 km)"| LORA_RX
    WIFI ==>|"HTTPS REST / WebSocket (JSON)"| CLOUD_DATABASE
    CLOUD_DATABASE -.->|"Real-Time Push Listener"| DASHBOARD
    CLOUD_DATABASE -.->|"Real-Time Query"| TABLE_VIEWER
```

---

## 3. Kamus Data (Data Dictionary)

### A. Tabel Node Real-Time: `/monitoring`
Tabel ini bertindak sebagai *single-record live state*. Nilai di dalamnya **di-overwrite setiap detik** oleh Gateway ESP32 RX saat menerima data telemetri baru dari Node TX.

| Nama Atribut | Tipe Data | Rentang / Satuan | Sumber Sensor | Keterangan |
| :--- | :--- | :--- | :--- | :--- |
| `nodeId` | String | misal: "TX-01" | Konfigurasi TX | Identitas unik perangkat sensor di lapangan |
| `level` | Integer | `0`, `1`, atau `2` | Algoritma ESP32 | Tingkat bahaya: `0` = Aman, `1` = Waspada, `2` = Bahaya |
| `temp` | Float | 0.0 – 100.0 (°C) | DHT22 | Suhu lingkungan di sekitar node |
| `hum` | Float | 0.0 – 100.0 (% RH) | DHT22 | Kelembaban relatif udara |
| `gas` | Integer | `0` (Low) atau `1` (High) | MQ-2 Digital | Status ambang batas digital komparator MQ-2 |
| `gasAO` | Integer | 0 – 4095 (ADC 12-bit) | MQ-2 Analog | Konsentrasi partikel asap & gas mudah terbakar |
| `flame` | Integer | `0` (Padam) atau `1` (Ada Api) | Flame Digital | Deteksi radiasi inframerah dari nyala api |
| `flameAO` | Integer | 0 – 4095 (ADC 12-bit) | Flame Analog | Intensitas radiasi nyala api (makin kecil makin dekat api) |
| `rssi` | Integer | -120 s/d -30 (dBm) | SX1278 Register | Indikator kekuatan sinyal transmisi gelombang LoRa |
| `lastUpdate`| String | Format Waktu | Internal RTC/Timer | Waktu terakhir kali gateway menerima paket dari TX |

---

### B. Tabel Riwayat Kejadian: `/logs`
Tabel ini bertindak sebagai *time-series audit trail*. Setiap kali terjadi peningkatan status bahaya (`level >= 1` atau `flame == 1`), sistem akan **menambahkan (append/push)** satu baris rekaman baru yang tersimpan permanen.

| Nama Atribut | Tipe Data | Format / Contoh | Keterangan |
| :--- | :--- | :--- | :--- |
| `log_id` | String (Key) | `-Nx9aBcDe123...` | Primary Key unik yang dihasilkan otomatis oleh Firebase |
| `timestamp` | String | `2026-11-13 16:50:12` | Tanggal dan jam kejadian saat anomali terdeteksi |
| `nodeId` | String | `TX-01` | Identitas node yang memicu anomali |
| `level` | Integer | `1` atau `2` | Status bahaya saat kejadian direkam |
| `temp` | Float | `58.2` | Nilai suhu puncak saat kejadian |
| `hum` | Float | `34.0` | Nilai kelembaban udara saat kejadian |
| `gasAO` | Integer | `850` | Nilai kadar gas MQ-2 saat kejadian |
| `flame` | Integer | `1` | Status sensor api saat kejadian |
| `rssi` | Integer | `-78` | Kualitas sinyal radio LoRa saat kejadian |
| `note` | String | `Bahaya: Api terdeteksi!` | Pesan ringkas penjelasan insiden |

---

## 4. Keunggulan Arsitektur Database Ini untuk Laporan Akademik

1. **Efisiensi Bandwidth & Penyimpanan**:
   - Memisahkan antara data live (`/monitoring`) dan data historis (`/logs`) mencegah database membengkak karena data kondisi normal tidak perlu disimpan ribuan kali per hari.
2. **Real-Time Subscription (WebSockets)**:
   - Web browser tidak perlu melakukan polling (menembak server berulang-ulang), melainkan Firebase yang aktif mendorong perubahan (*push notification*) dalam hitungan milidetik.
3. **Auditability (Pelaporan UAS/UTS)**:
   - Data pada node `/logs` dapat diekspor langsung ke format CSV/Excel melalui fitur yang sudah terpasang pada halaman Table Viewer.

