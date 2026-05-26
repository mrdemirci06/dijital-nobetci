# Bağlantı Şeması ve Detayları

## Sistem Genel Görünümü

```
┌─────────────────────────────────────────────────────────┐
│                    PC (Bilgisayar)                       │
│  ┌─────────────┐                                         │
│  │   Kamera    │ ─→ Python (Teachable Machine TFLite)   │
│  └─────────────┘                                         │
└────────────────────────────┬────────────────────────────┘
                             │ USB (Seri Port, 9600 baud)
                             ▼
┌─────────────────────────────────────────────────────────┐
│                    Arduino Uno                           │
│  D2 ← Twin DUR sinyali (INPUT)                          │
│  D3 → Röle IN (OUTPUT, Active LOW)                      │
│  5V → Röle VCC                                          │
│  GND → Twin GND + Röle GND (ortak)                      │
└──────────┬──────────────────────────────────┬───────────┘
           │                                  │
           │ D3                               │ Wire kablo
           ▼                                  ▼
   ┌──────────────┐                  ┌─────────────────┐
   │  Röle Modülü │                  │  Twin AIoT      │
   │   (5V SRD)   │                  │  (ESP32-S3)     │
   └──────┬───────┘                  │                 │
          │ COM/NO                   │  D6 → Wire çıkış│
          ▼                          │  D10 → Buzzer   │
   ┌──────────────┐                  │  D4 ← PIR       │
   │     Fan      │                  └────────┬────────┘
   │  (DC Motor)  │                           │
   └──────┬───────┘                           ▼
          │                          ┌─────────────────┐
          ▼                          │  PIR + Buzzer   │
   ┌──────────────┐                  │   (Mıknatıslı)  │
   │  Ayrı Güç    │                  └─────────────────┘
   │   Kaynağı    │                           ▲
   └──────────────┘                           │
                                              │
                                     ┌────────┴────────┐
                                     │   Powerbank     │
                                     │  (USB Power)    │
                                     └─────────────────┘
```

## Twin Modüllerinin Yerleşimi (Mıknatıslı Zincir)

```
[USB Power Modülü (Gri)]
        │ ⟷ (mıknatıs, ok yönü)
[Motion Sensor MG15 (Sarı)]    ← mode switch'i "b"
        │ ⟷ 
[Twin AIoT (Beyaz/Kırmızı) - Ana Modül]
        │
        ├── D6 (mıknatıs çıkış) ⟷ [Wire Modülü]
        │                              │
        │                              └─ Kablo → Arduino
        │
        └── D10 (mıknatıs çıkış) ⟷ [Buzzer MC17 (Mavi)]
```

## Wire Kablosunun Detayı

Wire modülünden çıkan kabloda **3 tel** vardır:

```
       ┌─────────────────────────────┐
       │      Wire Modülü            │
       │     (JST Konnektör)         │
       └──────┬──────┬──────┬────────┘
              │      │      │
            BEYAZ KIRMIZI SİYAH
              │      │      │
              ▼      ▼      ▼
          Sinyal   GND   VCC (⚠️ KULLANMAZ)
              │      │      │
              │      │      └─→ İZOLE ET!
              │      │
              ▼      ▼
          Arduino  Arduino
            D2     GND
```

**⚠️ ÖNEMLİ:** Siyah tel (VCC) ASLA Arduino'ya bağlanmaz. Bağlanırsa Arduino regülatörü ısınır ve zarar görebilir.

## Pull-Down Direnci

Arduino D2 ile GND arasına **10kΩ direnç** bağlanır:

```
   Arduino D2 ──────┬───── Wire Beyaz Tel (Sinyal)
                    │
                  10kΩ
                    │
   Arduino GND ─────┴───── Wire Kırmızı Tel (GND)
```

**Görevi:** Twin kapalıyken Arduino D2 pininin "floating" kalmasını önler. Pin sürekli LOW seviyede tutulur, böylece sistem güvenli tarafa düşer.

## Röle Bağlantısı

### Röle Modülü Pinleri (Songle SRD-5VDC-SL-C)

**Kontrol Tarafı (DC):**
```
 ┌─────────────────┐
 │  ROLE MODÜLÜ    │
 │                 │
 │  VCC ───── 5V (Arduino)
 │  GND ───── GND (Arduino)
 │  IN  ───── D3 (Arduino)
 └─────────────────┘
```

**Yük Tarafı (Vidalı Terminal):**
```
 ┌──────┬──────┬──────┐
 │  NC  │ COM  │  NO  │
 └───┬──┴───┬──┴───┬──┘
     │      │      │
   (boş) Pil(-) Fan(-)
                Pil(+) → Fan(+)
```

**Active LOW Röle Mantığı:**
- Arduino D3 = LOW → Röle bobini enerjilenir → COM-NO kontağı kapanır → Fan çalışır
- Arduino D3 = HIGH → Röle serbest → COM-NO kontağı açılır → Fan durur

## Güç Yönetimi

### Bağımsız Güç Kaynakları (KRİTİK)

```
1. Twin → USB Power → Powerbank
2. Arduino → PC USB
3. Fan → Ayrı Pil/Adaptör
```

**⚠️ Bu üç güç kaynağı asla birbirine bağlanmaz!**

Sadece **GND'ler ortaklaştırılır:**
- Twin GND (Wire kablo kırmızı) ↔ Arduino GND
- Arduino GND ↔ Röle GND
- Fan ve Arduino devresi arasında doğrudan elektriksel bağlantı YOK (röle ile izole)

## Pin Haritası Özeti

### Twin AIoT (ESP32-S3) GPIO Tablosu

| Twin Pini | GPIO No | Tür | Kullanım |
|-----------|---------|-----|----------|
| D4 | 8 | INPUT (dijital) | PIR sensör okuma |
| D4 seçim | 37 | OUTPUT | HIGH (dijital mod) |
| D6 | 35 | OUTPUT | Wire modülü → DUR sinyali |
| D10 | 10 | OUTPUT | Buzzer kontrolü |
| D8 | 11 | - | Kullanılmıyor |
| D9 | 36 | - | Kullanılmıyor |
| D12 | 9 | - | Kullanılmıyor |

### Arduino Uno Pin Tablosu

| Pin | Tür | Kullanım |
|-----|-----|----------|
| D2 | INPUT | Twin'den DUR sinyali |
| D3 | OUTPUT | Röle kontrol (Active LOW) |
| GND | - | Ortak GND |
| 5V | - | Röle VCC |
| USB | - | PC haberleşme + güç |
