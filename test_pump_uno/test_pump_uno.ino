// ================== TEST PUMP + L298N ==================
// L298N + Pump + Arduino Uno

// ================== PIN ==================
#define IN1 7   // L298N IN1

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
  
  pinMode(IN1, OUTPUT);
  digitalWrite(IN1, LOW);  // Pump OFF
  
  Serial.println("\n\n💧 TEST PUMP + L298N");
  Serial.println("═══════════════════════════════════════");
  Serial.println(getTimestamp() + " Boot time: " + String(bootTime) + " ms");
  Serial.println(getTimestamp() + " Pin: D" + String(IN1));
  Serial.println("═══════════════════════════════════════");
  Serial.println("Pump nyala-mati tiap 5 detik");
  Serial.println("═══════════════════════════════════════\n");
  
  delay(2000);
}

// ================== LOOP ==================
void loop() {
  // Pump ON
  Serial.println(getTimestamp() + " 💧 Pump ON");
  digitalWrite(IN1, HIGH);
  delay(5000);
  
  // Pump OFF
  Serial.println(getTimestamp() + " ⭕ Pump OFF");
  digitalWrite(IN1, LOW);
  delay(5000);
}