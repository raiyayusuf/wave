// ================== TEST LED RGB ==================
// LED RGB + ESP32 DevKitC V4 (32D)
// Nyala bergantian: Merah -> Hijau -> Biru
// Aktif HIGH

// ================== PIN ==================
#define LED_R 23
#define LED_G 19
#define LED_B 18

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  
  // Semua OFF
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);
  
  Serial.println("TEST LED RGB");
}

void loop() {
  // MERAH ON
  Serial.println("MERAH ON");
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);
  delay(2000);
  
  // MERAH OFF
  Serial.println("MERAH OFF");
  digitalWrite(LED_R, LOW);
  delay(2000);
  
  // HIJAU ON
  Serial.println("HIJAU ON");
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, LOW);
  delay(2000);
  
  // HIJAU OFF
  Serial.println("HIJAU OFF");
  digitalWrite(LED_G, LOW);
  delay(2000);
  
  // BIRU ON
  Serial.println("BIRU ON");
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, HIGH);
  delay(2000);
  
  // BIRU OFF
  Serial.println("BIRU OFF");
  digitalWrite(LED_B, LOW);
  delay(2000);
}