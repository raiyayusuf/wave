// ================== TEST PUMP + RELAY ==================
// Relay 1 Channel (HIGH Trigger) + Pump + Arduino Uno
// Pin: D13
// Jumper: H (HIGH trigger)

// ================== PIN ==================
#define RELAY_PIN 13   // D13

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
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Relay OFF (HIGH trigger)
  
  Serial.println("\n\n💧 TEST PUMP + RELAY (HIGH Trigger)");
  Serial.println("═══════════════════════════════════════");
  Serial.println(getTimestamp() + " Boot time: " + String(bootTime) + " ms");
  Serial.println(getTimestamp() + " Pin: D" + String(RELAY_PIN));
  Serial.println("═══════════════════════════════════════");
  Serial.println("Pump nyala-mati tiap 5 detik");
  Serial.println("═══════════════════════════════════════\n");
  
  delay(2000);
}

// ================== LOOP ==================
void loop() {
  // Pump ON
  Serial.println(getTimestamp() + " 💧 Pump ON");
  digitalWrite(RELAY_PIN, HIGH);  // HIGH = ON
  delay(5000);
  
  // Pump OFF
  Serial.println(getTimestamp() + " ⭕ Pump OFF");
  digitalWrite(RELAY_PIN, LOW);   // LOW = OFF
  delay(5000);
}