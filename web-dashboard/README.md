# 🚒 FireGuard LoRa - Web Dashboard Monitoring

Dashboard antarmuka web untuk sistem pendeteksi kebakaran berbasis **ESP32 + Modul LoRa (SX1278 433MHz)** untuk mata kuliah Wireless & Mobile Computing (WMC).

---

## 📁 Struktur Folder
- `index.html` : Single-page dashboard mandiri (Tailwind CSS, FontAwesome, Chart.js, Web Audio API).

---

## 🚀 Cara Membuka & Mencoba (Mode Konsultasi / Demo)

1. **Buka Langsung di Browser**:
   - Double-click file `index.html` di File Explorer, atau
   - Klik kanan `index.html` di VS Code -> **Open with Live Server** (jika ada ekstensi Live Server), atau drag file tersebut ke Google Chrome / Edge.

2. **Fitur Uji Coba Cepat (Toolbar Simulasi)**:
   Di bagian atas dashboard terdapat tombol simulasi untuk konsultasi kelompok dan demo ke dosen:
   - **Normal (Aman)**: Mengembalikan kondisi suhu normal (~28°C), gas rendah, api padam (Level 0 - Hijau).
   - **Waspada (Gas/Suhu)**: Mensimulasikan kenaikan konsentrasi gas MQ-2 & suhu naik (Level 1 - Kuning).
   - **Bahaya (Api Terdeteksi!)**: Mensimulasikan Flame Sensor terpicu api, gas tinggi, sirine alarm browser berbunyi & banner berkedip merah (Level 2 - Merah).
   - **Auto Acak**: Menggerakkan grafik secara otomatis secara dinamis.
   - **Export CSV**: Mengunduh rekaman log riwayat dalam format Excel / CSV.

---

## 🔗 Rencana Integrasi Real-Time dengan Firebase
Saat ESP32 RX sudah diprogram mengirim data ke Firebase Realtime Database:
1. Buat project gratis di [Firebase Console](https://console.firebase.google.com/).
2. Buat **Realtime Database** dengan rules read/write `true` (test mode).
3. Salin `firebaseConfig` ke bagian bawah `<script>` di `index.html`.
4. Web otomatis tersinkronisasi detik-ke-detik dengan data asli dari hardware LoRa!
