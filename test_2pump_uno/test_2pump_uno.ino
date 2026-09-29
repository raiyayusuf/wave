// ================== TEST 2 PUMP BERGANTIAN ==================
// L298N + 2 Pump + Arduino Uno
// ENA & ENB di-jumper 5V, IN2 & IN4 di-GND

// ================== PIN ==================
#define IN1 7   // Pump 1
#define IN3 9   // Pump 2

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
  pinMode(IN3, OUTPUT);
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  
  Serial.println("\n\n💧 TEST 2 PUMP BERGANTIAN");
  Serial.println("═══════════════════════════════════════");
  Serial.println(getTimestamp() + " Boot time: " + String(bootTime) + " ms");
  Serial.println("═══════════════════════════════════════");
  Serial.println("Pump 1 ON 5 detik -> Pump 2 ON 5 detik");
  Serial.println("═══════════════════════════════════════\n");
  
  delay(2000);
}

// ================== LOOP ==================
void loop() {
  // Pump 1 ON
  Serial.println(getTimestamp() + " 💧 Pump 1 ON");
  digitalWrite(IN1, HIGH);
  delay(5000);
  digitalWrite(IN1, LOW);
  Serial.println(getTimestamp() + " ⭕ Pump 1 OFF");
  delay(1000);
  
  // Pump 2 ON
  Serial.println(getTimestamp() + " 💧 Pump 2 ON");
  digitalWrite(IN3, HIGH);
  delay(5000);
  digitalWrite(IN3, LOW);
  Serial.println(getTimestamp() + " ⭕ Pump 2 OFF");
  delay(1000);
}