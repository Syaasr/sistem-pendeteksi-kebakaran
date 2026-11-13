# 🗄️ Database Service - Firebase Realtime Database

Folder ini berisi konfigurasi, skema, dan alat pengujian untuk **Firebase Realtime Database** pada sistem monitoring kebakaran berbasis LoRa.

---

## 📄 File di Folder Ini:
1. `firebase-config.json` : Kunci kredensial project Firebase yang aktif.
2. `database.rules.json` : Aturan izin baca/tulis (*Security Rules*) untuk Realtime Database.
3. `schema.json` : Contoh struktur JSON data sensor terkini (`monitoring`) dan riwayat (`logs`).
4. `test-simulator.html` : Halaman interaktif untuk mengetes pengiriman data langsung ke Firebase via browser.

---

## 🔒 Aturan Keamanan (*Security Rules*):
Pastikan di Firebase Console -> **Realtime Database** -> Tab **Rules** berisi:
```json
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```

---

## 🌐 Endpoint & Node Path:
- **`https://sistem-pendeteksi-kebaka-bac09-default-rtdb.asia-southeast1.firebasedatabase.app/monitoring.json`**
  Node data live terkini (di-overwrite oleh ESP32 RX setiap menerima paket LoRa).
- **`https://sistem-pendeteksi-kebaka-bac09-default-rtdb.asia-southeast1.firebasedatabase.app/logs.json`**
  Node catatan riwayat kejadian/anomali.

