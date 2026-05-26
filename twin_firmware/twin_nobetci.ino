/*
 * Dijital Nöbetçi - Twin AIoT Firmware
 * 
 * Board: ESP32S3 Dev Module
 * 
 * Bu kod Twin AIoT modülü üzerinde çalışır:
 * - PIR sensörü hareket izler
 * - 5 saniye hareketsizlikte alarm verir (buzzer)
 * - Arduino'ya "motoru durdur" sinyali gönderir
 * 
 * Bağlantılar:
 * - D4 girişi: PIR Sensor (Motion Sensor MG15) - mode b
 * - D10 çıkışı: Buzzer (MC17)
 * - D6 çıkışı: Wire modülü -> Arduino D2'ye sinyal
 * 
 * Önemli: PIR sensör TERS mantıkla çalışıyor:
 *   - LOW = hareket VAR
 *   - HIGH = hareket YOK
 */

// Pin tanımları
const int S1 = 37;              // D4 giriş seçim pini (dijital mod için HIGH)
const int PIR = 8;              // D4 dijital okuma pini
const int BUZZER = 10;          // D10 - Buzzer
const int DUR_SIGNAL = 35;      // D6 - Wire modülü -> Arduino'ya

// Zamanlama
unsigned long sonHareket = 0;
const unsigned long HAREKETSIZLIK_ESIGI = 5000;  // 5 saniye

void setup() {
  Serial.begin(115200);
  
  // D4 girişini dijital moda al (PIR dijital sensör)
  pinMode(S1, OUTPUT);
  digitalWrite(S1, HIGH);
  pinMode(PIR, INPUT);
  
  // Çıkış pinleri
  pinMode(BUZZER, OUTPUT);
  pinMode(DUR_SIGNAL, OUTPUT);
  
  // Başlangıçta her şey güvenli durumda
  digitalWrite(BUZZER, LOW);
  digitalWrite(DUR_SIGNAL, LOW);
  
  Serial.println("=================================");
  Serial.println("Dijital Nobetci - Twin tarafi");
  Serial.println("=================================");
  Serial.println("PIR isiniyor, 30 saniye bekleyin...");
  delay(30000);
  Serial.println("Sistem aktif!");
  
  sonHareket = millis();
}

void loop() {
  unsigned long simdi = millis();
  
  // PIR ters mantik: LOW = hareket var
  if (digitalRead(PIR) == LOW) {
    sonHareket = simdi;
    digitalWrite(BUZZER, LOW);
    digitalWrite(DUR_SIGNAL, LOW);   // Arduino'ya: normal durum
  }
  
  // 5 saniye hareketsiz kalindiysa alarm!
  if (simdi - sonHareket > HAREKETSIZLIK_ESIGI) {
    digitalWrite(DUR_SIGNAL, HIGH);   // Arduino'ya: motoru DURDUR!
    
    // Buzzer 300ms aralıklarla yanıp söner
    bool alarmDurumu = (simdi / 300) % 2;
    digitalWrite(BUZZER, alarmDurumu);
    
    // Seri monitör'e bilgi (her 1 saniyede bir)
    static unsigned long sonLog = 0;
    if (simdi - sonLog > 1000) {
      Serial.println("UYARI: 5 saniyedir hareket yok - ALARM!");
      sonLog = simdi;
    }
  }
  
  delay(50);
}
