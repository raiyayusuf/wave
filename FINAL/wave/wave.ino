// ================== WAVE - STEP 6 ==================
// Ultrasonic + Turbidity + LCD + Buzzer + Pump + ESP32 32D
// Pump nyala sampai air habis (ultrasonic deteksi > 15.8cm)

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ================== PIN ==================
#define TRIG_PIN 4
#define ECHO_PIN 5
#define TURBIDITY_PIN 34
#define BUZZER_PIN 32
#define PUMP_1 33
#define PUMP_2 27

// ================== LCD ==================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================== RANGE KALIBRASI TURBIDITY ==================
int ADC_JERNIH_MIN = 570;
int ADC_JERNIH_MAX = 660;
int ADC_KERUH_MIN = 0;
int ADC_KERUH_MAX = 390;

// ================== THRESHOLD ULTRASONIC ==================
#define JARAK_ADA_AIR 12.00     // cm — ada air
#define JARAK_KOSONG 15.80      // cm — air habis

// ================== VALIDASI ULTRASONIC ==================
#define DURASI_VALID 3000

// ================== WAKTU ==================
#define WAKTU_STABIL 3000
#define WAKTU_BACA 5000
#define DELAY_MATI 1000

// ================== FUNGSI BUZZER 3x ==================
void buzzer3x() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
  }
}

// ================== FUNGSI INTRO ==================
void intro() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Project Wave");
  delay(1000);
  
  lcd.setCursor(0, 1);
  lcd.print("SMP Kinder");
  
  buzzer3x();
  delay(300);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);
  
  delay(1000);
  lcd.clear();
}

// ================== FUNGSI ULTRASONIC ==================
float bacaJarak() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  float jarak = duration * 0.0343 / 2.0;
  
  return jarak;
}

// ================== FUNGSI VALIDASI ULTRASONIC ==================
bool cekAirValid() {
  unsigned long startTime = millis();
  
  while (millis() - startTime < DURASI_VALID) {
    float jarak = bacaJarak();
    if (jarak > JARAK_ADA_AIR) {
      return false;
    }
    delay(100);
  }
  return true;
}

// ================== FUNGSI PUMP ==================
void pump1On() {
  digitalWrite(PUMP_1, HIGH);
}

void pump1Off() {
  digitalWrite(PUMP_1, LOW);
}

void pump2On() {
  digitalWrite(PUMP_2, HIGH);
}

void pump2Off() {
  digitalWrite(PUMP_2, LOW);
}

// ================== FUNGSI TUNGGU AIR HABIS ==================
void tungguAirHabis() {
  while (true) {
    float jarak = bacaJarak();
    
    // Tampilkan jarak
    lcd.setCursor(0, 1);
    lcd.print("Jarak: ");
    lcd.print(jarak, 1);
    lcd.print("cm  ");
    
    // Cek air habis
    if (jarak >= JARAK_KOSONG) {
      delay(DELAY_MATI);  // delay 1 detik
      
      // Cek ulang — beneran kosong?
      float cek = bacaJarak();
      if (cek >= JARAK_KOSONG) {
        break;  // air habis
      }
    }
    
    delay(200);
  }
}

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PUMP_1, OUTPUT);
  pinMode(PUMP_2, OUTPUT);
  
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(PUMP_1, LOW);
  digitalWrite(PUMP_2, LOW);
  
  analogReadResolution(10);
  analogSetAttenuation(ADC_11db);
  
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  Serial.println("\nWAVE - STEP 6");
  
  intro();
}

// ================== LOOP ==================
void loop() {
  float jarak = bacaJarak();
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jarak: ");
  lcd.print(jarak, 1);
  lcd.print("cm");
  
  if (jarak <= JARAK_ADA_AIR) {
    lcd.setCursor(0, 1);
    lcd.print("Validasi...");
    
    if (cekAirValid()) {
      Serial.println("Air terdeteksi valid");
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Air terdeteksi");
      
      buzzer3x();
      delay(1000);
      
      // Stabilin
      Serial.println("Stabilin 3 detik...");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Stabilin...");
      delay(WAKTU_STABIL);
      
      // Baca turbidity
      Serial.println("Baca turbidity 5 detik...");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Baca turbidity");
      lcd.setCursor(0, 1);
      lcd.print("5 detik...");
      
      long totalADC = 0;
      int jumlahBaca = 0;
      unsigned long startTime = millis();
      
      while (millis() - startTime < WAKTU_BACA) {
        totalADC += analogRead(TURBIDITY_PIN);
        jumlahBaca++;
        delay(100);
      }
      
      int adcValue = totalADC / jumlahBaca;
      float voltage = (adcValue / 1023.0) * 3.3;
      
      // Status
      String status;
      String statusLCD;
      if (adcValue >= ADC_JERNIH_MIN && adcValue <= ADC_JERNIH_MAX) {
        status = "JERNIH";
        statusLCD = "JERNIH";
      } else if (adcValue >= ADC_KERUH_MIN && adcValue <= ADC_KERUH_MAX) {
        status = "KERUH";
        statusLCD = "KERUH";
      } else {
        status = "TIDAK VALID";
        statusLCD = "TIDAK VALID";
      }
      
      // Serial
      Serial.println("─────────────────────────────");
      Serial.println("HASIL TURBIDITY");
      Serial.println("  ADC    : " + String(adcValue));
      Serial.println("  Voltage: " + String(voltage, 3) + " V");
      Serial.println("  Status : " + status);
      Serial.println("─────────────────────────────\n");
      
      // LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ADC: ");
      lcd.print(adcValue);
      lcd.setCursor(0, 1);
      lcd.print(statusLCD);
      
      buzzer3x();
      
      // ================== PUMP ==================
      if (status == "JERNIH") {
        Serial.println("Pump 1 ON (tunggu air habis)...");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Pump 1 ON");
        
        pump1On();
        tungguAirHabis();
        pump1Off();
        
        Serial.println("Pump 1 OFF (air habis)");
      } 
      else if (status == "KERUH") {
        Serial.println("Pump 2 ON (tunggu air habis)...");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Pump 2 ON");
        
        pump2On();
        tungguAirHabis();
        pump2Off();
        
        Serial.println("Pump 2 OFF (air habis)");
      }
      
      // Buzzer air habis
      buzzer3x();
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Air habis");
      lcd.setCursor(0, 1);
      lcd.print("Pump OFF");
      delay(2000);
      
    } else {
      Serial.println("Ultrasonic gak valid (sekibetan)");
      lcd.setCursor(0, 1);
      lcd.print("Gak valid     ");
      delay(500);
    }
    
  } else {
    Serial.println("Gak ada air (" + String(jarak, 2) + " cm)");
    lcd.setCursor(0, 1);
    lcd.print("Gak ada air   ");
  }
  
  delay(500);
}