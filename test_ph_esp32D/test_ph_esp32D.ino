// ================== TEST pH SENSOR ==================
// Sensor pH PH-4502C + ESP32 DevKitC V4 (32D)
// Tanpa voltage divider

#define PH_PIN 35

float m = -5.756;   // slope (kalibrasi ulang ESP32)
float b = 21.42;    // offset (kalibrasi ulang ESP32)

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
}

void loop() {
  long total = 0;
  for (int i = 0; i < 10; i++) {
    total += analogRead(PH_PIN);
    delay(10);
  }
  int adcValue = total / 10;
  float voltage = (adcValue / 4095.0) * 3.3;
  
  float pH = (m * voltage) + b;
  pH = constrain(pH, 0, 14);
  
  Serial.print("ADC: ");
  Serial.print(adcValue);
  Serial.print(" | V: ");
  Serial.print(voltage, 3);
  Serial.print(" | pH: ");
  Serial.println(pH, 2);
  
  delay(500);
}