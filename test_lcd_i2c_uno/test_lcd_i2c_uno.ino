// ================== TEST LCD I2C ==================
// LCD 16x2 + Arduino Uno

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  Wire.begin();
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