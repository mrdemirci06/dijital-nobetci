# Dijital Nöbetçi 🛡️

> İşçi Sağlığı ve Güvenliği için Yapay Zeka Destekli Çift Katmanlı İzleme Sistemi
> Karabük Atatürk Ortaokulu
> Proje Danışmanı: Emre Demirci, Takım Lideri: Abdullah Mert Durmaz, Araştırmacı: Ömer Faruk Oral

[![Status](https://img.shields.io/badge/Status-Yar%C4%B1%20Final-success)]()
[![License](https://img.shields.io/badge/License-MIT-blue)]()
[![Platform](https://img.shields.io/badge/Platform-ESP32--S3%20%2B%20Arduino-orange)]()

## 🏆 Proje Başarıları

Bu proje **yarı finale yükselmiştir**. Jüri değerlendirmesinde özellikle:
- ✅ Güvenlik ve kişisel gizlilik testleri başarılı
- ✅ Birden fazla kontrol noktasına sahip sistem
- ✅ Özgün fikir üzerine **iyi mühendislik tasarımı**

## Proje Hakkında

Dijital Nöbetçi, motorlu araç ile çalışan işçilerin güvenliğini sağlamak amacıyla geliştirilmiş **çift katmanlı bir izleme sistemidir**. 

İşçi koruyucu gözlük takmadan motoru çalıştıramaz. Bunun yanı sıra, motor çalışırken işçi bayılırsa ya da uyuyakalırsa (5 saniye boyunca hareket algılanmazsa) motor otomatik durur ve alarm devreye girer.

## Sistem Mimarisi

İki ayrı platform birlikte çalışır:

### Birinci Sistem: Yapay Zeka Kontrolü
```
Kamera → PC (Görüntü İşleme) → Arduino Uno → Röle → DC Motor (Fan)
```
- USB kamera ile işçinin yüzü izlenir
- Teachable Machine modeli koruyucu gözlüğü tanır
- Sonuç Arduino'ya seri port ile iletilir

### İkinci Sistem: Donanımsal Güvenlik (Twin)
```
PIR Sensör → Twin AIoT (ESP32-S3) → Buzzer + Arduino'ya DUR sinyali
```
- PIR sensör hareketleri sürekli izler
- 5 saniye hareketsizlikte alarm verir
- Arduino'ya "motoru durdur" sinyali gönderir

## Çalışma Mantığı

| Gözlük | Hareket | Motor |
|--------|---------|-------|
| ✅ Var | ✅ Var | 🟢 Çalışır |
| ❌ Yok | ✅ Var | 🔴 Durur |
| ✅ Var | ❌ Yok (5sn+) | 🔴 Durur + Alarm |
| ❌ Yok | ❌ Yok | 🔴 Durur + Alarm |

## Donanım Bileşenleri

### Twin Seti
- **Twin AIoT Modülü (ML18)** - ESP32-S3 tabanlı ana kontrolcü
- **Motion Sensor (MG15)** - PIR hareket sensörü (mode b)
- **Buzzer (MC17)** - Sesli alarm
- **Wire Modülü** - Sinyal kablo çıkışı
- **USB Power Modülü** - Powerbank ile besleme

### Arduino Tarafı
- **Arduino Uno**
- **Songle SRD-5VDC-SL-C Röle Modülü** (active LOW)
- **Masaüstü Fan** (ayrı güç kaynağı)
- **10kΩ direnç** (pull-down)

### PC Tarafı
- **USB Webcam**
- **Teachable Machine modeli** (TFLite formatında)
- **Python 3.13** + ai-edge-litert + OpenCV + PySerial

## Kurulum

Detaylı kurulum talimatları için: [docs/KURULUM.md](docs/KURULUM.md)

### Hızlı Başlangıç

1. **Twin tarafına kod yükleyin:**
   - Arduino IDE → Board: ESP32S3 Dev Module
   - `twin_firmware/twin_nobetci.ino` yükleyin

2. **Arduino'ya kod yükleyin:**
   - Board: Arduino Uno
   - `arduino_firmware/arduino_nobetci.ino` yükleyin

3. **Python ortamını hazırlayın:**
   ```bash
   pip install opencv-python ai-edge-litert pyserial numpy
   ```

4. **Modeli indirin:**
   - Teachable Machine'den TFLite formatında modeli `python_ai/` klasörüne koyun

5. **Çalıştırın:**
   ```bash
   cd python_ai
   python nobetci.py
   ```

## Bağlantı Şeması

```
TWIN TARAFI:
[Powerbank] → [USB Power] ⟷ [PIR Sensor MG15] ⟷ [Twin AIoT D4]
                                                          │
                                                  D6 ⟷ [Wire Modülü]
                                                  D10 ⟷ [Buzzer MC17]

ARDUINO TARAFI:
[PC USB] → [Arduino Uno]
              │
              D2 ← Wire kablosu beyaz tel (sinyal)
              GND ← Wire kablosu kırmızı tel (GND)
              D3 → Röle IN
              5V → Röle VCC

FAN DEVRESİ:
[Ayrı Pil] → [Röle COM/NO] → [Fan]
```

**⚠️ Önemli:** Twin'in Wire kablosundan çıkan **siyah tel (VCC) ASLA bağlanmaz**, izole edilir.

## Pin Haritası

### Twin AIoT (ESP32-S3)
| Twin Pini | GPIO | Görev |
|-----------|------|-------|
| D4 (giriş, dijital) | GPIO 8 | PIR sensör okuma |
| - | GPIO 37 | D4 giriş tipi seçimi (HIGH) |
| D6 (mıknatıs çıkış) | GPIO 35 | Wire modülü → DUR sinyali |
| D10 (mıknatıs çıkış) | GPIO 10 | Buzzer kontrolü |

### Arduino Uno
| Pin | Görev |
|-----|-------|
| D2 | Twin'den DUR sinyali (INPUT) |
| D3 | Röle kontrolü (OUTPUT, active LOW) |
| GND | Twin GND ile ortak |

## Önemli Notlar

### PIR Sensörünün Mantığı
Bu projedeki Twin PIR sensörü **ters mantıkla** çalışıyor:
- **Hareket VAR → LOW (0)**
- **Hareket YOK → HIGH (1)**

Kodda buna göre düzenleme yapılmıştır.

### Sınıf İsimleri (Teachable Machine)
```
0 no_glasses
1 glasses
```

### Yapay Zeka Modelinin İyileştirmesi
Mevcut model "siyah herhangi bir nesne"yi gözlük olarak algılayabiliyor. Bu sorunu çözmek için:
- Gözlüklere belirgin süslemeler (neon renk, reflektör şerit)
- Daha çeşitli eğitim verisi (farklı ışık, açı, arka plan)
- Negatif örnekler (siyah telefon, kupa vb.)
- Güven skoru eşiği %80 ve üzeri

## Klasör Yapısı

```
dijital_nobetci/
├── README.md                      # Bu dosya
├── twin_firmware/
│   └── twin_nobetci.ino           # ESP32-S3 kodu
├── arduino_firmware/
│   └── arduino_nobetci.ino        # Arduino Uno kodu
├── python_ai/
│   ├── nobetci.py                 # Ana Python scripti
│   └── requirements.txt           # Python bağımlılıkları
├── docs/
│   ├── KURULUM.md                 # Detaylı kurulum
│   ├── BAGLANTI_SEMASI.md         # Bağlantı detayları
│   ├── SORUN_GIDERME.md           # Karşılaşılan sorunlar
│   ├── YAPAY_ZEKA_MODELI.md       # Model algoritması ve eğitim detayları
│   ├── MALIYET_ANALIZI.md         # Detaylı maliyet hesabı (Mayıs 2026)
│   └── proje_sunum.html           # Proje sunum sayfası
└── images/                        # Proje fotoğrafları
```

## Sorun Giderme

Karşılaşabileceğiniz yaygın sorunlar ve çözümleri için: [docs/SORUN_GIDERME.md](docs/SORUN_GIDERME.md)

## Geliştirme İçin İdeler

- [ ] LED Bargraph'ı uyarı seviyesi göstergesi olarak entegre et
- [ ] LCD/OLED ekran ekle (durum bilgisi)
- [ ] Olay loglama (kaç alarm, kaç gözlük çıkarma)
- [ ] Yüz tanıma + gözlük tanıma (birden fazla işçi)
- [ ] Web arayüzü (uzaktan izleme)
- [ ] Powerbank yerine pil tutucu (kompakt tasarım)
- [ ] 3D baskı kutu tasarımı

## Lisans

MIT License - Eğitim amaçlı kullanıma açıktır.

## Teşekkürler

- Twin Science Robotics (Twin AIoT kütüphanesi)
- Google Teachable Machine (Yapay zeka modeli)
- Espressif Systems (ESP32-S3)
- TensorFlow / AI Edge LiteRT topluluğu

---

**Proje Durumu:** ✅ Çalışıyor ve hazır

**Son Güncelleme:** Mayıs 2026
