# Maliyet Analizi

Dijital Nöbetçi projesinin **gerçek malzeme maliyeti** (Mayıs 2026 tahmini fiyatlarla).

## Donanım Bileşen Listesi ve Fiyatlar

### Twin Seti (Mevcut Kit İçeriği)

Twin AI & Kodlama Seti hazır bir set olarak satılmaktadır. Set fiyatı yaklaşık:

| Set | Tahmini Fiyat |
|-----|---------------|
| Twin AI & Kodlama Seti | ~3.500 - 4.500 TL |

Bu setin içinde projemizde kullandığımız bileşenler:
- Twin AIoT Modülü (ESP32-S3)
- Motion Sensor (PIR)
- Buzzer
- LED Bargraph
- USB Power Modülü
- Wire Modülü
- Powerbank
- Diğer bileşenler (LED, fan, servo motor, sensörler vb.)

**Tek tek almak isteyenler için yaklaşık birim fiyatlar:**

| Bileşen | Tahmini Fiyat |
|---------|---------------|
| Twin AIoT Modülü (ML18) | ~800 - 1.000 TL |
| Motion Sensor (MG15) | ~150 - 200 TL |
| Buzzer Modülü (MC17) | ~80 - 120 TL |
| USB Power Modülü | ~120 - 180 TL |
| Wire Modülü | ~50 - 80 TL |
| Powerbank | ~200 - 300 TL |
| **Twin Bileşenleri Alt Toplamı** | **~1.400 - 1.880 TL** |

### Arduino Tarafı

| Bileşen | Tahmini Fiyat |
|---------|---------------|
| Arduino Uno (orijinal) | ~600 - 800 TL |
| Arduino Uno (klon) | ~150 - 250 TL |
| Songle SRD-5VDC-SL-C Röle Modülü | ~30 - 50 TL |
| Masaüstü Fan (12V DC) | ~80 - 150 TL |
| Fan Güç Kaynağı (Pil/Adaptör) | ~50 - 100 TL |
| Breadboard (yarım boy) | ~30 - 50 TL |
| 10kΩ Direnç | ~1 TL |
| Jumper Kabloları (10'lu paket) | ~20 - 30 TL |
| USB Kabloları (2 adet) | ~30 - 50 TL |
| **Arduino Bileşenleri Alt Toplamı** | **~241 - 631 TL** (klon ile) |
| **Arduino Bileşenleri Alt Toplamı** | **~841 - 1.231 TL** (orijinal ile) |

### Genel Toplam

| Senaryo | Toplam Maliyet |
|---------|----------------|
| **Minimum (Twin tek tek + Arduino klon)** | ~1.641 - 2.511 TL |
| **Orta (Twin seti + Arduino klon)** | ~3.741 - 5.131 TL |
| **Maksimum (Twin seti + Arduino orijinal)** | ~4.341 - 5.731 TL |

## Maliyet Analizi Yorumu

### Avantajlar
✅ **Eğitim amaçlı kullanım için makul** bir maliyet
✅ **Twin seti** birden çok proje için kullanılabilir (yeniden kullanılabilir)
✅ **Modüler tasarım** sayesinde bileşenler diğer projelerde de kullanılır
✅ Klon Arduino kullanılarak maliyet **%50'den fazla** azaltılabilir

### Dezavantajlar
❌ Endüstriyel ortamda yaygın kullanım için **daha sağlam donanım** gerekebilir
❌ Twin seti **prototipleme için tasarlanmış**, son ürün için değil
❌ Mevcut tasarım **akademik/eğitim odaklı**

## Endüstriyel Üretim İçin Maliyet Projeksiyonu

Eğer bu sistem **gerçek bir fabrika ortamında** kullanılacaksa:

| Bileşen | Endüstriyel Versiyon | Tahmini Fiyat |
|---------|---------------------|---------------|
| Ana kontrolcü | Endüstriyel PLC veya ESP32 + custom PCB | ~500 - 2.000 TL |
| PIR sensör | IP65 endüstriyel PIR | ~300 - 600 TL |
| Röle | Solid state röle (SSR) | ~150 - 400 TL |
| Buzzer | Endüstriyel siren (90 dB+) | ~200 - 500 TL |
| Kamera | IP kamera (HD, gece görüş) | ~800 - 2.500 TL |
| Bilgisayar | Edge computing cihazı (Jetson Nano vb.) | ~3.000 - 8.000 TL |
| Muhafaza | IP65 kutu, kablo tavası vb. | ~500 - 1.500 TL |
| Kurulum işçiliği | - | ~2.000 - 5.000 TL |
| **Endüstriyel Toplam** | | **~7.450 - 20.500 TL** |

## Maliyet/Fayda Analizi

### Önlenebilir Maliyetler (Bir İş Kazası Senaryosu)
Bir endüstriyel iş kazası şu maliyetlere neden olabilir:
- **Tıbbi tedavi:** 10.000 - 500.000 TL
- **İş gücü kaybı:** 5.000 - 100.000 TL/ay
- **SGK ve hukuki maliyetler:** 50.000 - 2.000.000 TL
- **İtibar kaybı:** Ölçülemez

### Yatırım Geri Dönüşü (ROI)
- **Sistem maliyeti:** ~10.000 - 20.000 TL (endüstriyel versiyon)
- **Önlediği potansiyel zarar:** **Milyonlar TL**
- **ROI:** Tek bir önlenen kaza ile **100x+** geri ödenir

## Eğitim Versiyonu Maliyeti (Bu Proje)

Bu projedeki **toplam donanım maliyeti yaklaşık 3.500 - 5.000 TL** seviyesindedir. Tüm bileşenler eğitim setinin parçası olduğu için, **proje sonrası diğer çalışmalarda da kullanılabilir**. Bu da maliyetin **eğitim açısından oldukça verimli** olduğunu gösterir.

## Notlar

- Fiyatlar **Mayıs 2026** itibariyle Türkiye piyasası için **tahmini** değerlerdir
- Döviz kurları, üretici politikaları ve stok durumuna göre değişiklik gösterebilir
- Klon vs orijinal Arduino seçimi performansı etkilemez, kalite farkı vardır
- Twin seti **sadece bu proje için değil**, birden fazla eğitim projesi için kullanılır

## Fiyat Araştırma Önerisi

Güncel fiyatlar için şu kaynakları kullanabilirsiniz:
- **Robotistan** (robotistan.com)
- **Direnç.net** (direnc.net)
- **Twin Science** resmi web sitesi (twinscience.com)
- **Hepsiburada / Trendyol** (genel ticari platformlar)
- **AliExpress** (uluslararası, fiyat karşılaştırma için)
