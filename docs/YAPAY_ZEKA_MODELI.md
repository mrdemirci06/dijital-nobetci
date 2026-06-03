# Yapay Zeka Modeli - Gözlük Algılama

Bu doküman, Dijital Nöbetçi projesinde kullanılan **gözlük algılama yapay zeka modelini** detaylı olarak açıklar.

## 1. Algoritma ve Mimari

### Kullanılan Teknoloji
- **Eğitim Platformu:** Google Teachable Machine
- **Model Mimarisi:** MobileNet (transfer learning)
- **Format:** TensorFlow Lite (`.tflite`)
- **Çalışma Ortamı:** Python 3.13 + ai-edge-litert + OpenCV

### Neden MobileNet?
MobileNet, **hafif ve hızlı** bir görüntü sınıflandırma mimarisidir:
- Düşük güç tüketimi
- Gerçek zamanlı çalışma (CPU üzerinde 30 FPS)
- Küçük model boyutu (~2 MB)
- Mobil ve gömülü sistemler için optimize

### Transfer Learning Yaklaşımı
- ImageNet üzerinde önceden eğitilmiş MobileNet temel alındı
- Son katman bizim sınıflarımız için yeniden eğitildi
- Bu sayede **az veri ile yüksek doğruluk** elde edildi

## 2. Veri Seti

### Sınıflar
| Sınıf | Açıklama |
|-------|----------|
| `glasses` | Koruyucu gözlük takılı işçi yüzü |
| `no_glasses` | Gözlük takmayan işçi yüzü |

### Veri Çeşitliliği
Her sınıf için **150-200 aralığında fotoğraf** çekildi. Çeşitlilik sağlamak için:

- **Farklı açılar:** Sol, sağ, yukarı, aşağı bakış
- **Farklı mesafeler:** Yakın (30 cm) - Uzak (1.5 m)
- **Farklı ışık koşulları:** Doğal ışık, lamba altı, karışık
- **Farklı arka planlar:** Atölye, beyaz duvar, karışık zemin
- **Negatif örnekler:** Siyah telefon, kupa, kıyafet (yüze yakın) → `no_glasses` sınıfına eklendi

### Gözlük Üzerinde Özelleştirme
Modelin **siyah herhangi bir nesneyi gözlük olarak algılamasını önlemek** için:
- Gözlüğe **belirgin süslemeler** eklendi (renkli bant, reflektör)
- Bu sayede model "gözlük şekli + benzersiz desen" öğrenir
- Yanlış pozitif (false positive) oranı düştü

## 3. Eğitim Parametreleri

```
Epochs: 50
Batch Size: 16
Learning Rate: 0.001
```

Bu varsayılan değerler, MobileNet transfer learning için yeterli olarak belirlendi.

## 4. Model Performansı

### Test Doğruluğu
- Eğitim doğruluğu: ~%95+ (tipik)
- Test doğruluğu: ~%90+ (gerçek kullanımda)
- **Güven eşiği: %80** (bu eşiğin altındaki tahminler "kararsız" olarak yorumlanır)

### Yanlış Pozitif Önleme
```python
GUVEN_ESIGI = 0.80  # %80 üzeri güven gerekli
gozluk_var = ('no_glasses' not in sinif_adi.lower()) and (guven >= GUVEN_ESIGI)
```

Bu yaklaşım sayesinde:
- Model emin değilse "gözlük yok" kabul edilir (güvenli taraf)
- Yanlışlıkla motor çalıştırma riski azaltılır

## 5. Model Çalışma Akışı

```
┌─────────────────┐
│  USB Kamera     │
│  (Görüntü Akışı)│
└────────┬────────┘
         │ Her frame (33ms ~ 30 FPS)
         ▼
┌─────────────────┐
│  OpenCV         │
│  - Resize 224x224│
│  - BGR → RGB    │
│  - Normalize    │
│    [-1, +1]     │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  TFLite Modeli  │
│  (MobileNet)    │
│  Inference      │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Çıktı:         │
│  [skor1, skor2] │
│  argmax + güven │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Karar:         │
│  güven >= 0.80? │
│  sınıf?         │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Arduino'ya     │
│  Seri Port:     │
│  '1' veya '0'   │
└─────────────────┘
```

## 6. Görüntü Ön İşleme

Modelin doğru çalışması için her görüntü standartlaştırılır:

```python
# 1. Boyutlandırma (MobileNet 224x224 bekler)
img = cv2.resize(frame, (224, 224))

# 2. Renk kanalı dönüşümü (OpenCV BGR, model RGB bekler)
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

# 3. Numpy array dönüşümü
img = np.asarray(img, dtype=np.float32).reshape(1, 224, 224, 3)

# 4. Normalizasyon ([-1, +1] aralığına)
img = (img / 127.5) - 1
```

## 7. TFLite Interpreter Kullanımı

```python
from ai_edge_litert.interpreter import Interpreter

# Modeli yükle
interpreter = Interpreter(model_path='model_unquant.tflite')
interpreter.allocate_tensors()

# Giriş/çıkış bilgilerini al
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

# Tahmin yap
interpreter.set_tensor(input_details[0]['index'], img)
interpreter.invoke()
tahmin = interpreter.get_tensor(output_details[0]['index'])

# Sonuç: [skor_no_glasses, skor_glasses] formatında
sinif_idx = np.argmax(tahmin)
guven = tahmin[0][sinif_idx]
```

## 8. Model Eğitim Adımları (Tekrarlanabilirlik)

Birisi modeli yeniden eğitmek isterse:

### Adım 1: Teachable Machine'i Aç
1. https://teachablemachine.withgoogle.com/
2. **Get Started** → **Image Project** → **Standard image model**

### Adım 2: Sınıfları Oluştur
1. İlk sınıfı `glasses` olarak adlandır
2. İkinci sınıfı `no_glasses` olarak adlandır

### Adım 3: Veri Topla
- Her sınıf için **Webcam** veya **Upload** ile fotoğraf ekle
- Minimum 50, ideal 100+ fotoğraf
- Yukarıda belirtilen çeşitliliğe dikkat et

### Adım 4: Modeli Eğit
- **Train Model** butonuna bas
- Varsayılan ayarları kullan (Epochs: 50, Batch Size: 16, LR: 0.001)
- Eğitim 1-3 dakika sürer

### Adım 5: Test Et
- Sağ taraftaki **Preview** alanında modeli test et
- Hatalı tahminler varsa, o senaryolar için **daha fazla veri ekle**
- Tekrar eğit

### Adım 6: Modeli İndir
1. **Export Model** → **Tensorflow Lite** sekmesi (Keras değil!)
2. Model conversion type: **Floating point** seç
3. **Download my model** → ZIP indirilir
4. ZIP içinden `model_unquant.tflite` ve `labels.txt` çıkar
5. Bu dosyaları `python_ai/` klasörüne koy

## 9. Model Dosyaları

Proje deposunda mevcut dosyalar:
- `python_ai/model_unquant.tflite` - Eğitilmiş model
- `python_ai/labels.txt` - Sınıf etiketleri
- `python_ai/nobetci.py` - Ana çalıştırma kodu

## 10. Gelecek İyileştirmeler

### Veri Seti Genişletme
- Daha fazla işçi/birey çeşitliliği
- Endüstriyel ortam görüntüleri
- Düşük ışık koşullarında daha çok veri

### Model Mimarisi
- **EfficientNet** denemesi (daha yüksek doğruluk)
- **YOLO** entegrasyonu (yüz tespit + sınıflandırma)
- **Çoklu sınıf:** Maske takılı/takılı değil, eldiven vb. de algılama

### Performans Optimizasyonu
- **Edge TPU** uyumluluğu (Coral USB Accelerator)
- **Quantized model** kullanımı (int8, daha hızlı)
- **GPU desteği** (eğer mevcutsa)

### Algoritma İyileştirme
- **Temporal smoothing:** Tek frame yerine son N frame'in çoğunluğu
- **Yüz tespit ön katmanı:** Kameraya bakan ile bakmayan yüzü ayırt etme
- **Birden fazla işçi tanıma:** Yüz tanıma + gözlük tanıma kombinasyonu
