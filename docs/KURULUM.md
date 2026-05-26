# Detaylı Kurulum Kılavuzu

## 1. Donanım Hazırlığı

### Gerekli Malzemeler

**Twin Seti:**
- Twin AIoT Modülü (ML18)
- Motion Sensor (MG15)
- Buzzer (MC17)
- Wire Modülü
- USB Power Modülü
- Powerbank veya USB güç kaynağı

**Arduino Tarafı:**
- Arduino Uno
- Songle SRD-5VDC-SL-C Röle (veya benzeri tek kanal röle)
- DC motorlu masaüstü fan
- Fan için ayrı güç kaynağı (pil, adaptör)
- Breadboard
- 10kΩ direnç (pull-down)
- Jumper kabloları

**PC Tarafı:**
- Bilgisayar (Linux, Windows veya Mac)
- USB webcam
- USB kabloları (Arduino ve Twin için ayrı)

## 2. Yazılım Kurulumu

### 2.1 Arduino IDE Kurulumu

1. https://www.arduino.cc/en/software adresinden indirin
2. Kurun

### 2.2 ESP32 Board Paketi (Twin için)

1. **Arduino IDE → File → Preferences**
2. "Additional Boards Manager URLs" kısmına ekleyin:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. **Tools → Board → Boards Manager**
4. "esp32" aratın
5. **"esp32 by Espressif Systems"** paketini yükleyin

### 2.3 Twin AIoT Kütüphanesi (Opsiyonel)

GitHub: https://github.com/twin-science-robotics/hw_twin_aiot_module_arduino_library

1. ZIP olarak indirin
2. Arduino IDE → Sketch → Include Library → Add .ZIP Library
3. İndirdiğiniz ZIP'i seçin

### 2.4 Python Kurulumu

Python 3.10+ önerilir.

```bash
# Linux/Mac
python3 --version

# Windows
python --version
```

Gerekli paketleri kurun:

```bash
cd python_ai
pip install -r requirements.txt
```

**Not:** `tflite-runtime` çalışmazsa `ai-edge-litert` kullanın (Python 3.13 için).

## 3. Kod Yükleme

### 3.1 Twin Kartına Kod Yükleme

1. Twin'i USB-C kablo ile bilgisayara bağlayın
2. Arduino IDE'yi açın
3. **Tools menüsü ayarları:**
   - Board: **ESP32S3 Dev Module**
   - USB CDC On Boot: **Enabled** (ÖNEMLİ!)
   - CPU Frequency: 240MHz
   - Flash Size: 4MB
   - Port: Bağlanan COM/ttyACM/ttyUSB portu
4. `twin_firmware/twin_nobetci.ino` dosyasını açın
5. **Upload** (→) butonuna basın

### 3.2 Arduino'ya Kod Yükleme

1. Arduino Uno'yu USB ile bilgisayara bağlayın
2. **Tools menüsü:**
   - Board: **Arduino Uno**
   - Port: Bağlanan COM/ttyUSB portu (Twin'den farklı olmalı)
3. `arduino_firmware/arduino_nobetci.ino` dosyasını açın
4. **Upload** butonuna basın

### 3.3 Teachable Machine Modeli

1. https://teachablemachine.withgoogle.com/ adresine gidin
2. **Image Project → Standard image model** seçin
3. İki sınıf oluşturun: `glasses` ve `no_glasses`
4. Her sınıf için 50-100 fotoğraf çekin
5. **Train Model** butonuna basın (varsayılan ayarlar yeterli)
6. Eğitim bitince **Export Model** → **Tensorflow Lite** → **Floating point** → **Download**
7. ZIP'i açın
8. `model_unquant.tflite` ve `labels.txt` dosyalarını `python_ai/` klasörüne kopyalayın

## 4. Donanım Bağlantısı

### Twin Tarafı (Mıknatıslı)

Modülleri ok yönünde mıknatıslarla birleştirin:

```
[USB Power] ⟷ [PIR Sensor] ⟷ [Twin AIoT D4]
                                      │
                          D6 ⟷ [Wire Modülü]
                          D10 ⟷ [Buzzer]
```

PIR sensörün **mode switch'i "b" konumunda** olmalı.

### Arduino - Twin Bağlantısı

Wire kablosunun JST ucundaki tellerden:

| Wire Tel | Arduino Pini | Not |
|----------|--------------|-----|
| Beyaz | D2 | Sinyal (DUR komutu) |
| Kırmızı | GND | Ortak GND |
| Siyah | **BOŞTA** | ⚠️ İzole edin, bağlamayın |

**10kΩ pull-down direnci:** Arduino D2 ile GND arasına bağlanır.

### Arduino - Röle Bağlantısı

| Röle Pini | Arduino Pini |
|-----------|--------------|
| VCC | 5V |
| GND | GND |
| IN | D3 |

### Röle - Fan Bağlantısı

```
Fan (+) → Pil/Güç Kaynağı (+)
Fan (-) → Röle COM
Röle NO → Pil/Güç Kaynağı (-)
```

⚠️ **Fan ve Arduino güç kaynakları AYRI olmalı!**

## 5. Çalıştırma

### 5.1 Sistemleri Açın

1. Powerbank'i Twin USB Power'a bağlayın → Twin açılır
2. 30 saniye bekleyin (PIR ısınması)
3. Arduino'yu PC'ye USB ile bağlayın
4. Fan güç kaynağını açın

### 5.2 Python Scriptini Çalıştırın

```bash
cd python_ai
python nobetci.py
```

### 5.3 Test Senaryoları

| Test | Beklenen Davranış |
|------|-------------------|
| Gözlük tak, hareket et | Fan çalışır |
| Gözlük çıkar | Fan durur |
| Gözlük tak, 5sn hareketsiz dur | Fan durur + buzzer alarm |

## 6. Arduino Port Bulma

### Linux
```bash
ls /dev/ttyUSB* /dev/ttyACM*
```

### Windows
**Aygıt Yöneticisi → Bağlantı Noktaları (COM ve LPT)**

### Mac
```bash
ls /dev/tty.usbmodem*
```

Bulduğunuz portu `nobetci.py` içindeki `ARDUINO_PORT` değişkenine yazın.
