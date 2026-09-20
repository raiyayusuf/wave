// ================== TEST pH SENSOR ==================
// Sensor pH PH-4502C + Arduino Uno
// Kalibrasi: pH 7 = 1.65V, pH 4 = 2.059V (asumsi)
// Rumus: pH = m × voltage + b

// ================== PIN ==================
#define PH_PIN A0

// ================== KALIBRASI ==================
float m = -7.335;      // slope (hitung ulang)
float b = 19.10;       // offset (hitung ulang)

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
  
  Serial.println("\n\n🧪 TEST pH SENSOR PH-4502C");
  Serial.println("═══════════════════════════════════════");
  Serial.println(getTimestamp() + " Boot time: " + String(bootTime) + " ms");
  Serial.println(getTimestamp() + " Pin: A0");
  Serial.println("═══════════════════════════════════════");
  Serial.println("Kalibrasi:");
  Serial.println("  pH 7 = 1.65 V");
  Serial.println("  pH 4 = 2.059 V (asumsi)");
  Serial.println("  Slope (m) = " + String(m, 3));
  Serial.println("  Offset (b) = " + String(b, 3));
  Serial.println("═══════════════════════════════════════\n");
  
  delay(2000);
}

// ================== LOOP ==================
void loop() {
  long total = 0;
  for (int i = 0; i < 10; i++) {
    total += analogRead(PH_PIN);
    delay(10);
  }
  int adcValue = total / 10;
  float voltage = (adcValue / 1023.0) * 5.0;
  
  float pH = (m * voltage) + b;
  pH = constrain(pH, 0, 14);
  
  String status;
  if (pH < 6.5) {
    status = "ASAM 🟡";
  } else if (pH > 7.5) {
    status = "BASA 🔵";
  } else {
    status = "NETRAL 🟢";
  }
  
  Serial.println(getTimestamp() + " ─────────────────────────────");
  Serial.println(getTimestamp() + " 📊 ADC: " + String(adcValue) + " (0-1023)");
  Serial.println(getTimestamp() + " ⚡ Voltage: " + String(voltage, 3) + " V");
  Serial.println(getTimestamp() + " 🧪 pH: " + String(pH, 2));
  Serial.println(getTimestamp() + " 📌 Status: " + status);
  
  delay(1000);
}