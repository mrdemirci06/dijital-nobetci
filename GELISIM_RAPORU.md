# Dijital Nöbetçi — Geliştirme Süreci Raporu

Bu doküman, Dijital Nöbetçi projesinin geliştirilme sürecinde karşılaşılan problemleri, bulunan çözümleri ve yarı final sürecinde projeye eklenen yeni özellikleri özetler.

---

## 1. Karşılaşılan Problemler ve Çözümleri

### 1.1 Donanım Sorunları

- **ESP32 board seçimi karışıklığı** → Doğru kart "ESP32S3 Dev Module" olarak belirlendi (Twin AIoT içinde ESP32-S3-MINI-1 çipi var).
- **Twin pin haritası belirsizdi** → Test edilerek çıkarıldı:
  - PIR okuma: GPIO 8 (D4 girişi), seçim pini GPIO 37 (HIGH)
  - Buzzer: GPIO 10 (D10)
  - DUR sinyali: GPIO 35 (D6, Wire modülü üzerinden)
- **PIR sensörün ters mantığı** keşfedildi: LOW = hareket VAR, HIGH = hareket YOK.
- **PIR mode switch** "b" konumunda olması gerektiği tespit edildi.
- **Röle modülünün Active LOW** olduğu belirlendi: LOW = fan çalışır, HIGH = fan durur.
- **En zorlu sorun — Wire kablosu tel sıralaması belirsizdi** → Multimetre ile ölçülerek çözüldü:
  - Beyaz tel = Sinyal (Arduino D2)
  - Kırmızı tel = GND (Arduino GND)
  - Siyah tel = VCC (izole edildi, bağlanmadı)
- **LED Bargraph iptal edildi**, Wire modülü D6'ya taşındı (daha sade ve güvenli bağlantı).
- **10kΩ pull-down direnci** Arduino D2 ile GND arasına eklendi (floating pin önlendi).

### 1.2 Yazılım Sorunları

- **Python 3.13 + TensorFlow uyumsuzluğu** → TFLite formatı + `ai-edge-litert` kütüphanesi kullanılarak çözüldü.
- **Linux kamera erişim sorunu** (`/dev/video0`) → Kullanıcı `video` grubuna eklenerek giderildi.
- **Gözlük tanıma mantığı** netleştirildi: `no_glasses` sınıfı dışındaki tahminler ve %80+ güven şartı ile motor çalışması sağlandı.
- **Güç yönetimi** doğru kurgulandı: Twin, Arduino ve fan ayrı güç kaynaklarından beslendi, yalnızca GND'ler ortaklaştırıldı.

---

## 2. Yarı Final Sürecinde Eklenen ve Geliştirilenler

### 2.1 Jüri Eleştirilerine Yanıtlar

- **Maliyet analizi dokümanı** hazırlandı (`docs/MALIYET_ANALIZI.md`) — bileşen bazlı tahmini TL fiyatlar, eğitim ve endüstriyel versiyon projeksiyonları, maliyet/fayda analizi.
- **Yapay zeka model dokümantasyonu** eklendi (`docs/YAPAY_ZEKA_MODELI.md`) — MobileNet mimarisi, transfer learning, veri seti detayları, adım adım eğitim kılavuzu, performans metrikleri.
- **AI kodu ve model dosyaları** paylaşıma hazırlandı (model_unquant.tflite + labels.txt).

### 2.2 Dokümantasyon ve Paketleme

- **Profesyonel GitHub paketi** oluşturuldu: README, LICENSE (MIT), kurulum kılavuzu, bağlantı şeması, sorun giderme dokümanı.
- **README** güncellendi: yarı final başarısı ve durum rozetleri (badge) eklendi.
- **3 dakikalık tanıtım videosu metni** hazırlandı (`docs/VIDEO_METNI.md`) — iki konuşmacı diyaloğu, problem→çözüm→demo→sonuç akışı, çekim notları ve 90 saniyelik kısa versiyon.

### 2.3 Yeni Özellik: Canlı Web Kontrol Paneli

- **Flask + WebSocket** tabanlı gerçek zamanlı dashboard geliştirildi.
- **Cyber temalı modern arayüz**: canlı kamera akışı, durum kartları (gözlük/hareket/motor), istatistikler, renk kodlu olay günlüğü, animasyonlu arka plan.
- Sistemle gerçek haberleşme: kameradan AI tahmini, Arduino seri haberleşmesi, anlık veri akışı.

---

## 3. Devam Eden ve Planlanan Çalışmalar

- **Arduino → Twin geçişi**: Sistemi sadeleştirmek için ikinci bir Twin ile paralel sistem kurulması (mevcut çalışan sistem bozulmadan, yedek olarak korunarak).
- **Web panelinde Twin PIR durumunu canlı gösterme**: Arduino kodunda Twin DUR sinyalini PC'ye iletecek küçük bir güncelleme gerekiyor.
- **AI model iyileştirme**: Gözlüklere belirgin süslemeler eklenerek ve veri seti çeşitlendirilerek yanlış pozitiflerin (false positive) azaltılması.
- **Donanım iyileştirmeleri**: 3D baskı kutu, kompakt tasarım, powerbank yerine pil tutucu.

---

## 4. Özet

Proje, başlangıçtaki donanım belirsizlikleri ve yazılım uyumsuzluklarına rağmen sistematik test ve sorun giderme yaklaşımıyla çalışır hale getirildi. Yarı final sürecinde hem jüri eleştirileri karşılandı hem de projeye canlı web kontrol paneli gibi yeni özellikler eklenerek sistem daha güçlü ve sunulabilir bir hale getirildi.

**Proje Durumu:** ✅ Çalışıyor — Yarı Final aşamasında

**Son Güncelleme:** Haziran 2026
