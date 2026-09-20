// ================== TEST TURBIDITY SENSOR ==================
// Sensor Turbidity SEN-0175 + Arduino Uno
// Output: Serial Monitor

// ================== PIN ==================
#define TURBIDITY_PIN A0

// ================== RANGE KALIBRASI ==================
// JERNIH
int ADC_JERNIH_MIN = 440;
int ADC_JERNIH_MAX = 460;

// KERUH
int ADC_KERUH_MIN = 0;
int ADC_KERUH_MAX = 200;

// ================== VARIABEL ==================
unsigned long bootTime;

// ================== FUNGSI TIMESTAMP ==================
String getTimestamp() {
  unsigned long ms = millis() - bootTime;
  unsigned long sec = ms / 1000;
  unsigned long min = sec / 60;
  sec = sec % 60;
  ms = ms % 1000;
  
  char buffer[20];
  sprintf(buffer, "[%02lu:%02lu.%03lu]", min, sec, ms);
  return String(buffer);
}

// ================== SETUP ==================
void setup() {
  Serial.begin(9600);
  delay(1000);
  bootTime = millis();
  
  Serial.println("\n\n🌊 TEST TURBIDITY SENSOR");
  Serial.println("═══════════════════════════════════════");
  Serial.println(getTimestamp() + " Boot time: " + String(bootTime) + " ms");
  Serial.println(getTimestamp() + " Pin: A0");
  Serial.println("═══════════════════════════════════════");
  Serial.println("Range kalibrasi:");
  Serial.println("  JERNIH: ADC " + String(ADC_JERNIH_MIN) + " - " + String(ADC_JERNIH_MAX));
  Serial.println("  KERUH : ADC " + String(ADC_KERUH_MIN) + " - " + String(ADC_KERUH_MAX));
  Serial.println("═══════════════════════════════════════\n");
  
  delay(2000);
}

// ================== LOOP ==================
void loop() {
  // Baca ADC 10x, ambil rata-rata
  long total = 0;
  for (int i = 0; i < 10; i++) {
    total += analogRead(TURBIDITY_PIN);
    delay(10);
  }
  int adcValue = total / 10;
  
  // Konversi ke tegangan (display doang)
  float voltage = (adcValue / 1023.0) * 5.0;
  
  // Tentukan status berdasarkan range
  String status;
  String keterangan;
  
  if (adcValue >= ADC_JERNIH_MIN && adcValue <= ADC_JERNIH_MAX) {
    // JERNIH
    status = "JERNIH ✅";
    keterangan = "Air jernih terdeteksi";
  } 
  else if (adcValue >= ADC_KERUH_MIN && adcValue <= ADC_KERUH_MAX) {
    // KERUH
    status = "KERUH ❌";
    keterangan = "Air keruh terdeteksi";
  } 
  else {
    // Di luar range
    status = "⚠️ TIDAK VALID";
    keterangan = "Silahkan masukan sensor ke air";
  }
  
  // Tampilkan
  Serial.println(getTimestamp() + " ─────────────────────────────");
  Serial.println(getTimestamp() + " 📊 ADC: " + String(adcValue) + " (0-1023)");
  Serial.println(getTimestamp() + " ⚡ Voltage: " + String(voltage, 3) + " V");
  Serial.println(getTimestamp() + " 📌 Status: " + status);
  Serial.println(getTimestamp() + " 💬 " + keterangan);
  
  delay(1000);
}