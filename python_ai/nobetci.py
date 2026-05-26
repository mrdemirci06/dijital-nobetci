"""
Dijital Nobetci - Python AI Scripti

Bu script:
- USB kameradan görüntü alır
- Teachable Machine modelini (TFLite) çalıştırır
- Gözlük tanıma sonucunu Arduino'ya seri port üzerinden gönderir

Gerekli dosyalar:
- model_unquant.tflite (Teachable Machine'den indirilen)
- labels.txt (sınıf isimleri)

Sınıf isimleri (labels.txt):
0 no_glasses
1 glasses

Kurulum:
    pip install opencv-python ai-edge-litert pyserial numpy

Çalıştırma:
    python nobetci.py
"""

import cv2
import numpy as np
from ai_edge_litert.interpreter import Interpreter
import serial
import time

# ============== AYARLAR ==============
ARDUINO_PORT = '/dev/ttyUSB0'    # Linux: /dev/ttyUSB0 veya /dev/ttyACM0
                                  # Windows: COM3, COM4 vb.
BAUD_RATE = 9600
MODEL_PATH = 'model_unquant.tflite'
LABELS_PATH = 'labels.txt'
GUVEN_ESIGI = 0.80               # %80 ve üzeri güven gerekli
KAMERA_INDEX = 0                 # Kamera indeksi (0, 1, 2...)

# ============== KURULUM ==============
print("Arduino baglaniyor...")
arduino = serial.Serial(ARDUINO_PORT, BAUD_RATE, timeout=1)
time.sleep(2)
print("Arduino bagli!")

print("Model yukleniyor...")
interpreter = Interpreter(model_path=MODEL_PATH)
interpreter.allocate_tensors()
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

with open(LABELS_PATH, 'r', encoding='utf-8') as f:
    labels = [line.strip() for line in f.readlines()]
print(f"Siniflar: {labels}")

# Kamera başlat
cap = cv2.VideoCapture(KAMERA_INDEX)
if not cap.isOpened():
    # Kamera 0 açılmazsa 1 dene
    print("Kamera 0 acilmadi, kamera 1 deneniyor...")
    cap = cv2.VideoCapture(1)

son_gonderim = None

print("Sistem calisiyor... (q ile cikis)")
print("=" * 50)

# ============== ANA DÖNGÜ ==============
while True:
    ret, frame = cap.read()
    if not ret:
        continue
    
    # Görüntüyü modele uygun hale getir
    img = cv2.resize(frame, (224, 224))
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    img = np.asarray(img, dtype=np.float32).reshape(1, 224, 224, 3)
    img = (img / 127.5) - 1
    
    # Tahmin yap
    interpreter.set_tensor(input_details[0]['index'], img)
    interpreter.invoke()
    tahmin = interpreter.get_tensor(output_details[0]['index'])
    
    sinif_idx = np.argmax(tahmin)
    guven = tahmin[0][sinif_idx]
    sinif_adi = labels[sinif_idx]
    
    # Gözlük var mı kararı:
    # 'no_glasses' içermiyorsa = gözlük VAR
    gozluk_var = ('no_glasses' not in sinif_adi.lower()) and (guven >= GUVEN_ESIGI)
    
    # Arduino'ya gönder (sadece durum değiştiğinde)
    sinyal = '1' if gozluk_var else '0'
    if sinyal != son_gonderim:
        arduino.write(sinyal.encode())
        son_gonderim = sinyal
        print(f"-> Arduino: {sinyal} ({sinif_adi}, guven: {guven:.0%})")
    
    # Ekrana yazdır
    durum = "GOZLUK VAR" if gozluk_var else "GOZLUK YOK"
    # Renk: gözlük varken yeşil (güvenli), yokken kırmızı (uyarı)
    renk = (0, 255, 0) if gozluk_var else (0, 0, 255)
    
    cv2.putText(frame, f"{durum} ({guven:.0%})", (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX, 1, renk, 2)
    cv2.putText(frame, f"Sinif: {sinif_adi}", (10, 70),
                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
    cv2.imshow('Dijital Nobetci', frame)
    
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# ============== TEMİZLİK ==============
cap.release()
cv2.destroyAllWindows()
arduino.close()
print("Sistem kapatildi.")
