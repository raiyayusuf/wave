// ================== TEST BUZZER ==================
// Buzzer + ESP32 DevKitC V4 (32D)
// Pin: GPIO 32

#define BUZZER_PIN 32

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);  // OFF
  
  Serial.println("\nTEST BUZZER");
}

void loop() {
  
  // 2. Nyala-mati 3x, 0.5 detik
  Serial.println("Beep 3x (0.5 detik)");
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
  }
  
  Serial.println("⭕ OFF 2 detik");
  digitalWrite(BUZZER_PIN, LOW);
  delay(1000);

  // 1. Nyala 2 detik, mati 2 detik
  Serial.println("ON 2 detik");
  digitalWrite(BUZZER_PIN, HIGH);
  delay(500);
  
  Serial.println("⭕ OFF 2 detik");
  digitalWrite(BUZZER_PIN, LOW);
  delay(1000);
  
  // 3. Jeda 2 detik
  Serial.println("⭕ Jeda 2 detik");
  delay(2000);
}