// ================== TEST pH SENSOR ==================
// Sensor pH PH-4502C + Arduino Uno
// Kalibrasi: pH 4 = 2.928V, pH 9.18 = 2.160V

#define PH_PIN A0

float m = -6.745;   // slope
float b = 23.75;    // offset

void setup() {
  Serial.begin(9600);
  delay(1000);
}

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
  
  Serial.print("ADC: ");
  Serial.print(adcValue);
  Serial.print(" | V: ");
  Serial.print(voltage, 3);
  Serial.print(" | pH: ");
  Serial.println(pH, 2);
  
  delay(1000);
}