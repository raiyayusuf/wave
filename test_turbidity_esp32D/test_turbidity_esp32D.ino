// ================== TEST TURBIDITY SENSOR ==================
// Sensor Turbidity + ESP32 (10-bit, sama kayak Uno)

#define TURBIDITY_PIN 34

// Range kalibrasi (sama kayak Uno)
int ADC_JERNIH_MIN = 570;
int ADC_JERNIH_MAX = 660;
int ADC_KERUH_MIN = 0;
int ADC_KERUH_MAX = 390;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Set ADC 10-bit (0-1023) — sama kayak Uno
  analogReadResolution(10);
  analogSetAttenuation(ADC_11db);
}

void loop() {
  long total = 0;
  for (int i = 0; i < 10; i++) {
    total += analogRead(TURBIDITY_PIN);
    delay(10);
  }
  int adcValue = total / 10;
  
  // Konversi ke voltage (ESP32: 3.3V)
  float voltage = (adcValue / 1023.0) * 3.3;
  
  // Status
  String status;
  if (adcValue >= ADC_JERNIH_MIN && adcValue <= ADC_JERNIH_MAX) {
    status = "JERNIH ✅";
  } else if (adcValue >= ADC_KERUH_MIN && adcValue <= ADC_KERUH_MAX) {
    status = "KERUH ❌";
  } else {
    status = "⚠️ TIDAK VALID";
  }
  
  Serial.print("ADC: ");
  Serial.print(adcValue);
  Serial.print(" | V: ");
  Serial.print(voltage, 3);
  Serial.print(" | Status: ");
  Serial.println(status);
  
  delay(1000);
}