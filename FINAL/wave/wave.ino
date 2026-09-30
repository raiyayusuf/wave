// ================== WAVE - STEP 1 ==================
// Ultrasonic + Turbidity + ESP32 32D
// Logika: Ultrasonic deteksi air -> Turbidity baca

// ================== PIN ==================
#define TRIG_PIN 4
#define ECHO_PIN 5
#define TURBIDITY_PIN 34

// ================== RANGE KALIBRASI TURBIDITY ==================
int ADC_JERNIH_MIN = 570;
int ADC_JERNIH_MAX = 660;
int ADC_KERUH_MIN = 0;
int ADC_KERUH_MAX = 390;

// ================== THRESHOLD ULTRASONIC ==================
#define JARAK_ADA_AIR 12.00   // cm

// ================== WAKTU ==================
#define WAKTU_STABIL 5000     // 5 detik
#define WAKTU_BACA 5000       // 5 detik

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

// ================== FUNGSI TURBIDITY ==================
int bacaTurbidity() {
  long total = 0;
  for (int i = 0; i < 10; i++) {
    total += analogRead(TURBIDITY_PIN);
    delay(10);
  }
  return total / 10;
}

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  analogReadResolution(10);
  analogSetAttenuation(ADC_11db);
  
  Serial.println("\n🌊 WAVE - STEP 1");
  Serial.println("═════════════════════════════");
}

// ================== LOOP ==================
void loop() {
  // 1. Baca jarak
  float jarak = bacaJarak();
  
  // 2. Cek ada air?
  if (jarak <= JARAK_ADA_AIR) {
    // Ada air
    Serial.println("💧 Air terdeteksi (" + String(jarak, 2) + " cm)");
    Serial.println("⏳ Stabilin 5 detik...");
    
    // 3. Tunggu stabil
    delay(WAKTU_STABIL);
    
    // 4. Baca turbidity 5 detik
    Serial.println("📊 Baca turbidity 5 detik...");
    
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
    
    // 5. Tentukan status
    String status;
    if (adcValue >= ADC_JERNIH_MIN && adcValue <= ADC_JERNIH_MAX) {
      status = "JERNIH ✅";
    } else if (adcValue >= ADC_KERUH_MIN && adcValue <= ADC_KERUH_MAX) {
      status = "KERUH ❌";
    } else {
      status = "⚠️ TIDAK VALID";
    }
    
    // 6. Tampilkan hasil
    Serial.println("─────────────────────────────");
    Serial.println("📊 HASIL TURBIDITY");
    Serial.println("   ADC    : " + String(adcValue));
    Serial.println("   Voltage: " + String(voltage, 3) + " V");
    Serial.println("   Status : " + status);
    Serial.println("─────────────────────────────\n");
    
  } else {
    // Gak ada air
    Serial.println("⭕ Gak ada air (" + String(jarak, 2) + " cm)");
  }
  
  delay(1000);
}