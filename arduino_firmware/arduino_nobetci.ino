/*
 * Dijital Nöbetçi - Arduino Uno Firmware
 * 
 * Board: Arduino Uno
 * 
 * Bu kod Arduino Uno üzerinde çalışır:
 * - PC'den (Teachable Machine) gözlük bilgisini seri port ile alır
 * - Twin'den hareketsizlik (DUR) sinyalini D2 üzerinden okur
 * - Her iki şart sağlanırsa röleyi (motoru) çalıştırır
 * 
 * Bağlantılar:
 * - D2 (INPUT): Twin Wire kablo beyaz tel (DUR sinyali)
 * - GND: Twin Wire kablo kırmızı tel (ortak GND)
 * - D3 (OUTPUT): Röle IN pini
 * - 5V: Röle VCC
 * 
 * Röle: Songle SRD-5VDC-SL-C (Active LOW)
 *   - LOW = röle çekili = fan çalışır
 *   - HIGH = röle serbest = fan durur
 * 
 * Motor şartı: Gözlük VAR + Twin DUR DEMİYOR
 */

// Pin tanımları
const int ROLE_PIN = 3;          // D3 - Röle kontrolü
const int TWIN_DUR_PIN = 2;      // D2 - Twin'den gelen DUR sinyali

// Sistem durumu
bool gozlukVar = false;

void setup() {
  Serial.begin(9600);             // PC ile haberlesme (Python script)
  
  pinMode(ROLE_PIN, OUTPUT);
  pinMode(TWIN_DUR_PIN, INPUT);
  
  // Başlangıçta motor kapalı (güvenli durum)
  motorDurdur();
  
  Serial.println("Arduino hazir - Dijital Nobetci");
}

void loop() {
  // 1. PC'den (Teachable Machine) gözlük bilgisini al
  if (Serial.available() > 0) {
    char gelen = Serial.read();
    if (gelen == '1') {
      gozlukVar = true;
    } else if (gelen == '0') {
      gozlukVar = false;
    }
  }
  
  // 2. Twin'den DUR sinyali kontrolü
  bool twinDurDiyor = digitalRead(TWIN_DUR_PIN);
  
  // 3. Karar: gözlük VAR ve Twin DUR DEMİYOR ise motor çalışır
  if (gozlukVar && !twinDurDiyor) {
    motorCalistir();
  } else {
    motorDurdur();
  }
  
  delay(50);
}

// Röle kontrol fonksiyonları (Active LOW röle)
void motorCalistir() {
  digitalWrite(ROLE_PIN, LOW);   // LOW = röle çekili = fan çalışır
}

void motorDurdur() {
  digitalWrite(ROLE_PIN, HIGH);  // HIGH = röle serbest = fan durur
}
