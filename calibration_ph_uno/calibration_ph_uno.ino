#define Sensor A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  int hasil = analogRead(Sensor);
  float volt = hasil * (5.0 / 1023.0);
  Serial.print(volt);
  Serial.println(" V");
  delay(500);
}