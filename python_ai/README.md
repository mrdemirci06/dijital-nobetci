# Python AI Modülü

## Dosyalar

- `nobetci.py` - Ana Python scripti
- `requirements.txt` - Python bağımlılıkları
- `model_unquant.tflite` - **Teachable Machine'den indirilecek** (bu klasöre koyun)
- `labels.txt` - **Teachable Machine'den indirilecek** (bu klasöre koyun)

## Model Dosyaları Nasıl İndirilir?

1. https://teachablemachine.withgoogle.com/ adresine gidin
2. Modelinizi eğitin (veya mevcut modeli açın)
3. **Export Model** → **Tensorflow Lite** sekmesi
4. Model conversion type: **Floating point**
5. **Download my model** → ZIP indirilir
6. ZIP'i açın, içindeki `model_unquant.tflite` ve `labels.txt` dosyalarını bu klasöre koyun

## Sınıf İsimleri Önerisi

Teachable Machine'de sınıfları **İngilizce** ve **basit** isimlendirin:
- Class 1: `glasses` (gözlük var)
- Class 2: `no_glasses` (gözlük yok)

Türkçe karakter (ş, ç, ğ, ü, ö, ı) Python'da sorun çıkarabilir.

## Eğitim İçin İpuçları

- Her sınıf için **en az 150-200 fotoğraf**
- Farklı **ışık koşulları**
- Farklı **açılar** (sol, sağ, yukarı, aşağı)
- Farklı **arka planlar**
- **Negatif örnekler:** Siyah telefon, kupa vb. nesneleri yüze yakın tutarak da çekin (no_glasses sınıfına ekleyin)

## Kurulum

```bash
pip install -r requirements.txt
```

## Çalıştırma

```bash
python nobetci.py
```

- Çıkmak için: kamera penceresinde **q** tuşuna basın
