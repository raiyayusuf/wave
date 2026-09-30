// ================== TEST ULTRASONIC ==================
// HC-SR04 + ESP32 DevKitC V4
// Tanpa library

// ================== PIN ==================
#define TRIG_PIN 4
#define ECHO_PIN 5

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  delay(1000);
}

void loop() {
  // Kirim pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Baca echo
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  
  // Konversi ke cm
  float jarak = duration * 0.0343 / 2.0;
  
  // Tampilkan
  Serial.print("Jarak: ");
  Serial.print(jarak, 2);
  Serial.println(" cm");
  
  delay(500);
}