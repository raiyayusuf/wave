// ================== TEST LCD I2C ==================
// LCD 16x2 + ESP32 DevKitC V4 (32D)

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Init I2C ESP32 (SDA=21, SCL=22)
  Wire.begin(21, 22);
  
  lcd.init();
  lcd.backlight();
  
  Serial.println("LCD OK");
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Project     ");
  
  lcd.setCursor(0, 1);
  lcd.print("Wave Kinder ");
  
  delay(500);
}