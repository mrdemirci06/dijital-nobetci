# Sorun Giderme Kılavuzu

Bu projenin geliştirilmesi sırasında karşılaşılan ve çözülen sorunlar.

## Twin / ESP32-S3 Sorunları

### 1. Hangi ESP32 board'unu seçmeliyim?

**Doğru cevap:** `ESP32S3 Dev Module`

Twin AIoT içinde **ESP32-S3-MINI-1** çipi var. Karıştırılmaması gereken seçenekler:
- ❌ ESP32 Dev Module (klasik ESP32, farklı çip)
- ❌ ESP32C3 Dev Module
- ❌ ESP32C6 Dev Module
- ✅ **ESP32S3 Dev Module**

### 2. Kod yüklenmiyor / COM port görünmüyor

**Kontrol listesi:**
- USB-C kablosu **veri taşıyan** olmalı (şarj kablosu çoğu zaman çalışmaz)
- **Tools → USB CDC On Boot: Enabled** olmalı
- Boot moduna alma: **BOOT butonunu basılı tutun, RESET'e basın, BOOT'u bırakın**
- Başka programın (Twin Code Lab) portu kullanmadığından emin olun

### 3. "Sketch uses X bytes" mesajı = HATA değil, başarı

Bu mesaj **kod başarıyla derlendi** demek. Sonrasında yükleme satırları gelmeli:
```
Connecting......
Hash of data verified.
Hard resetting via RTS pin...
```

Bu mesajları görürseniz kod **başarıyla yüklenmiş** demektir.

### 4. PIR sensörü ters çalışıyor (LOW=hareket VAR)

Bu Twin PIR'ının normal davranışı. Kodda buna göre düzenleme yapıldı:
```cpp
if (digitalRead(PIR) == LOW) {  // LOW = hareket VAR
    sonHareket = millis();
}
```

### 5. PIR sensörü mode switch konumu

Mode switch **"b" konumunda** olmalı (sürekli algılama modu).
"a" konumu darbe modu, alarm mantığında düzgün çalışmaz.

### 6. PIR sürekli HIGH veriyor

PIR'ların ısınma süresi 30 saniye - 2 dakika arası. İlk açılışta hatalı tetikleme normaldir. Hiç hareket etmeden bekleyin.

## Arduino Sorunları

### 1. Arduino regülatörü çok ısınıyor

**Sebep:** Twin Wire kablosunun **VCC teli (siyah)** yanlış bir yere bağlandığında, Twin 3.3V'u Arduino'ya akar ve regülatör ısınır.

**Çözüm:**
- Wire kablosunun **siyah tel ucunu izole edin** (elektrik bandı, makaron)
- Sadece **beyaz (sinyal)** ve **kırmızı (GND)** telleri kullanın
- Multimetre ile teleri doğrulayın

### 2. Röle hangi mantıkta çalışıyor?

**Songle SRD-5VDC-SL-C röle: Active LOW**
- `digitalWrite(ROLE_PIN, LOW)` → röle çekili → fan çalışır
- `digitalWrite(ROLE_PIN, HIGH)` → röle serbest → fan durur

Bu standart Arduino mantığının tersidir.

### 3. Fan ters çalışıyor

Eğer kodu yüklediğinizde fan/röle ters davranıyorsa:
1. Önce röle mantığını test edin (Active HIGH mı Active LOW mu)
2. Sonra Python tarafındaki gözlük tanıma mantığını düzeltin
3. Son olarak ekrandaki yazıyı düzeltin

## Wire Kablosu Sorunu (En Zor Olan)

### Tel Sıralaması Belirsiz

Twin Wire kablosunda 3 tel var: beyaz, kırmızı, siyah. Ancak **renk konvansiyonu Twin'de standart değil**.

**Multimetre ile tespit yöntemi:**

1. Twin'i açın (Wire modülü D6'ya takılı)
2. Multimetre **20V DC** modunda
3. **Siyah probu** Powerbank USB kasasının metalik kısmına dokundurun (sabit GND)
4. **Kırmızı probu** 3 tele sırayla dokundurun:
   - **5-6V** veren → VCC (kullanmayın)
   - **0V** veren → GND veya sinyal

**Bu projedeki sonuç:**
- **Beyaz tel:** Sinyal → Arduino D2
- **Kırmızı tel:** GND → Arduino GND
- **Siyah tel:** VCC → ⚠️ İZOLE EDİN

⚠️ **Her Wire kablosunda sıralama farklı olabilir!** Multimetre ile mutlaka kontrol edin.

## Python / Yapay Zeka Sorunları

### 1. TensorFlow modeli yüklenmiyor

**Hata:** `Unrecognized keyword arguments passed to DepthwiseConv2D: {'groups': 1}`

**Sebep:** Python 3.13 + yeni TensorFlow versiyonu, eski Keras formatını okuyamaz.

**Çözüm:** TensorFlow Lite formatı kullanın:
- Teachable Machine'de **Tensorflow Lite** sekmesini seçin (Keras değil)
- **Floating point** seçin
- İndirilen `model_unquant.tflite` dosyasını kullanın

### 2. tflite-runtime kurulamıyor (Python 3.13)

**Hata:** `Could not find a version that satisfies the requirement tflite-runtime`

**Çözüm:** Alternatifini kurun:
```bash
pip install ai-edge-litert
```

Python kodunda:
```python
from ai_edge_litert.interpreter import Interpreter
```

### 3. Kamera açılmıyor (Linux)

**Hata:** `can't open camera by index`

**Çözüm:**

1. Kameraları listele:
   ```bash
   v4l2-ctl --list-devices
   ```

2. Eğer `v4l2-ctl` yoksa:
   ```bash
   sudo apt install v4l-utils
   ```

3. Kullanıcıyı `video` grubuna ekle:
   ```bash
   sudo usermod -a -G video $USER
   ```
   (Logout/login gerekir)

4. Cheese ile kamerayı test edin:
   ```bash
   sudo apt install cheese
   cheese
   ```

5. Python kodunda kamera indexini değiştirin:
   ```python
   cap = cv2.VideoCapture(1)  # 0 yerine 1
   ```

### 4. Model "siyah" nesneleri gözlük olarak algılıyor

**Sebep:** Model siyah çerçeveli gözlüğü, siyah renk olarak öğrenmiş.

**Çözüm:**
- Gözlüğe **belirgin süslemeler** ekleyin (neon bant, reflektör, renkli desen)
- Modeli **daha çeşitli verilerle yeniden eğitin**
- **Negatif örnekler** ekleyin: siyah telefon, kupa, kıyafet vb. yüze yakın tutarak `no_glasses` sınıfına ekleyin
- Güven skoru eşiğini **%80**'e yükseltin

### 5. Gözlük tanıma mantığı ters

Eğer ekranda yazı doğru ama fan ters çalışıyorsa, kontrol noktaları:

1. **Python `gozluk_var` mantığı:**
   ```python
   gozluk_var = ('no_glasses' not in sinif_adi.lower()) and (guven >= GUVEN_ESIGI)
   ```

2. **Arduino röle mantığı:**
   ```cpp
   void motorCalistir() { digitalWrite(ROLE_PIN, LOW); }  // Active LOW
   void motorDurdur() { digitalWrite(ROLE_PIN, HIGH); }
   ```

3. **Ekran yazısı:**
   ```python
   durum = "GOZLUK VAR" if gozluk_var else "GOZLUK YOK"
   ```

## Genel İpuçları

### Sistematik Teşhis Yöntemi

1. **Tek tek test edin:**
   - Önce Twin tek başına (Arduino bağlı değil)
   - Sonra Arduino tek başına (Python bağlı değil)
   - En son tam sistem

2. **Seri Monitör'ü kullanın:**
   - Twin için 115200 baud
   - Arduino için 9600 baud
   - Sistemin ne yaptığını görmek için

3. **Multimetre kullanın:**
   - Voltaj seviyeleri (3.3V mı, 5V mı, 0V mı)
   - Süreklilik kontrolü
   - Kısa devre kontrolü

4. **USB güç kaynaklarını ayırın:**
   - Twin ve Arduino farklı USB'lerden beslenmeli
   - GND'ler ortak, VCC'ler asla bağlı değil

### Sık Yapılan Hatalar

- ❌ Twin VCC telini Arduino'ya bağlamak
- ❌ Wire kablosunun siyah telini GND sanmak (her zaman GND değil!)
- ❌ Arduino ve Twin'i aynı güç kaynağından beslemek
- ❌ ESP32 (klasik) board seçmek (S3 olmalı)
- ❌ USB CDC On Boot ayarını Disabled bırakmak
- ❌ PIR mode switch'i "a" konumunda bırakmak
- ❌ Keras formatında model indirip TensorFlow ile çalıştırmaya çalışmak (TFLite kullanın)
