# 🚀 Panduan untuk Teman Pemegang Hardware (ESP32 RX)

Kodingan untuk menghubungkan **ESP32 Gateway (RX)** ke **Cloud Firebase & Web Monitoring** sudah selesai dibuat dan siap di-flash!

Node Sensor (TX) **TIDAK PERLU DIUBAH SAMA SEKALI**. Kamu hanya perlu meng-upload kodingan di folder **`lora-rx`** ini.

---

## 🛠️ Langkah-Langkah (Hanya 3 Langkah Mudah):

### 1. Buka File Pengaturan Wi-Fi
Buka file: **`lora-rx/include/variable.h`** di VS Code.
Cari baris berikut di bagian paling bawah:

```cpp
// Ganti dengan Nama & Password Hotspot HP kamu:
#define WIFI_SSID       "NamaHotspotKamu"
#define WIFI_PASSWORD   "PasswordHotspotKamu"
```
Ganti `"NamaHotspotKamu"` dan `"PasswordHotspotKamu"` sesuai Hotspot HP kamu (pastikan frekuensi hotspot 2.4 GHz aktif).

---

### 2. Upload Kodingan ke ESP32 RX
1. Colokkan board **ESP32 RX** menggunakan kabel data USB ke laptop.
2. Di VS Code, buka menu **PlatformIO** (icon semut di sidebar kiri atau tombol panah `→` di baris status bawah).
3. Klik tombol **Upload** (atau `PlatformIO: Upload`).
4. Tunggu hingga proses kompilasi selesai bertuliskan **[SUCCESS]**.

---

### 3. Selesai! Uji Coba Langsung di Web
1. Nyalakan Hotspot HP kamu.
2. Buka Serial Monitor di VS Code (kecepatan baud `115200`). Kamu akan melihat tulisan:
   ```text
   [Gateway] Memulai koneksi Wi-Fi...
   [Gateway] Wi-Fi Terhubung! IP: 192.168.xxx.xxx
   ```
3. Nyalakan node sensor **ESP32 TX**.
4. Buka Web Dashboard di browser: **`monitoring/web/index.html`**.
5. Sulut pemantik/korek api di depan Flame Sensor atau tiup gas ke sensor MQ-2 pada node TX.
6. **Hasilnya:**
   - Sinyal terkirim via LoRa 433 MHz ke RX.
   - RX langsung meneruskan data ke Cloud Firebase via Wi-Fi.
   - Web Dashboard di laptop langsung berkedip merah dan memutar sirine alarm secara real-time!

