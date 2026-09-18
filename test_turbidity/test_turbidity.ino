// ================== TEST TURBIDITY SENSOR ==================
// Sensor Turbidity SEN-0175 + ESP32
// Output: Serial Monitor

// ================== PIN ==================
#define TURBIDITY_PIN 34   // GPIO 34 (ADC1_CH6)

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
  Serial.begin(115200);
  delay(1000);
  bootTime = millis();
  
  Serial.println("\n\n🌊 TEST TURBIDITY SENSOR");
  Serial.println("═══════════════════════════════════════");
  Serial.println(getTimestamp() + " Boot time: " + String(bootTime) + " ms");
  Serial.println(getTimestamp() + " Pin: GPIO " + String(TURBIDITY_PIN));
  Serial.println("═══════════════════════════════════════");
  Serial.println("Celup sensor ke air jernih & air keruh");
  Serial.println("Lihat perbedaan nilai ADC");
  Serial.println("═══════════════════════════════════════\n");
  
  // Set resolusi ADC 12-bit (0-4095)
  analogReadResolution(12);
  
  // Set attenuasi ADC (0-3.3V)
  analogSetAttenuation(ADC_11db);
  
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
  
  // Konversi ke tegangan (setelah voltage divider)
  float voltage = (adcValue / 4095.0) * 3.3;
  
  // Konversi ke tegangan asli sensor (sebelum divider)
  // Rasio divider: (R1 + R2) / R2 = (10k + 20k) / 20k = 1.5
  float sensorVoltage = voltage * 1.5;
  
  // Konversi ke persen kekeruhan (perlu kalibrasi)
  // Asumsi: 4.5V = 0% keruh (jernih), 0V = 100% keruh
  float turbidityPercent = map(sensorVoltage * 100, 0, 450, 100, 0);
  turbidityPercent = constrain(turbidityPercent, 0, 100);
  
  // Tampilkan di Serial Monitor
  Serial.println(getTimestamp() + " ─────────────────────────────");
  Serial.println(getTimestamp() + " 📊 ADC: " + String(adcValue) + " (0-4095)");
  Serial.println(getTimestamp() + " ⚡ Voltage (ESP32): " + String(voltage, 3) + " V");
  Serial.println(getTimestamp() + " ⚡ Voltage (Sensor): " + String(sensorVoltage, 3) + " V");
  Serial.println(getTimestamp() + " 🌊 Turbidity: " + String(turbidityPercent, 1) + " %");
  
  // Status kekeruhan
  String status;
  if (turbidityPercent < 30) {
    status = "JERNIH ✅";
  } else if (turbidityPercent < 70) {
    status = "AGAK KERUH ⚠️";
  } else {
    status = "KERUH ❌";
  }
  Serial.println(getTimestamp() + " 📌 Status: " + status);
  
  delay(1000);
}