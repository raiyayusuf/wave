// ================== WAVE - FINAL + TELEGRAM ==================
// Ultrasonic 1 + Turbidity + Ultrasonic 2 + pH + LCD + Buzzer + Pump + Telegram
// ESP32 DevKitC V4 (32D)

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// ================== WIFI ==================
const char* ssid = "omah azel";
const char* password = "azelia545177";

// ================== TELEGRAM ==================
#define BOT_TOKEN "8983741538:AAH0LSuUqc2Z7hQwiTR8ejF5QgzurCeDi5Q"
#define OWNER_CHAT_ID "8149678877"
#define GROUP_CHAT_ID "-5536262947"
#define BOT_USERNAME "@wave_test_yusuf_bot"

// ================== PIN ==================
#define TRIG_PIN_1 4
#define ECHO_PIN_1 5
#define TRIG_PIN_2 16
#define ECHO_PIN_2 17
#define TURBIDITY_PIN 34
#define PH_PIN 35
#define BUZZER_PIN 32
#define PUMP_1 33
#define PUMP_2 27

// ================== LCD ==================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================== TELEGRAM OBJEK ==================
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
unsigned long lastTimeBotRan;
const unsigned long botRequestDelay = 1000;

// ================== KALIBRASI TURBIDITY ==================
int ADC_JERNIH_MIN = 570;
int ADC_JERNIH_MAX = 660;
int ADC_KERUH_MIN = 0;
int ADC_KERUH_MAX = 390;

// ================== KALIBRASI pH ==================
float m = -5.756;
float b = 21.42;

// ================== THRESHOLD ULTRASONIC ==================
#define JARAK_ADA_AIR 12.00
#define JARAK_KOSONG 15.50

// ================== VALIDASI ==================
#define DURASI_VALID 3000

// ================== WAKTU ==================
#define WAKTU_STABIL 3000
#define WAKTU_BACA 5000
#define WAKTU_STABIL_PH 5000
#define DELAY_MATI 1000
#define DELAY_HASIL 3000

// ================== VARIABEL GLOBAL ==================
bool DETEKSI_TURBIDITY = false;
int adcValue = 0;
String statusTurb = "";
float pH = 0;
String statusPH = "";
String statusLCD = "";
bool dataTurbidityAda = false;
bool dataPHAda = false;

// ================== FUNGSI BEEP 1x ==================
void beep1x() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(50);
  digitalWrite(BUZZER_PIN, LOW);
}

// ================== FUNGSI BEEP 3x ==================
void buzzer3x() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
  }
}

// ================== FUNGSI INTRO ==================
void intro() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Project Wave");
  lcd.setCursor(0, 1);
  lcd.print("SMP Kinder");
  
  // Beep 3x di intro
  buzzer3x();
  delay(400);
  
  digitalWrite(BUZZER_PIN, HIGH);
  delay(600);
  digitalWrite(BUZZER_PIN, LOW);
  
  delay(200);
  
  for (int i = 0; i < 2; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
  }
  
  delay(1000);
  lcd.clear();
}

// ================== FUNGSI ULTRASONIC ==================
float bacaJarak(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  long duration = pulseIn(echo, HIGH, 30000);
  float jarak = duration * 0.0343 / 2.0;
  
  return jarak;
}

// ================== FUNGSI VALIDASI ==================
bool cekAirValid(int trig, int echo) {
  unsigned long startTime = millis();
  
  while (millis() - startTime < DURASI_VALID) {
    float jarak = bacaJarak(trig, echo);
    if (jarak > JARAK_ADA_AIR) {
      return false;
    }
    delay(100);
  }
  return true;
}

// ================== FUNGSI PUMP ==================
void pump1On() { digitalWrite(PUMP_1, HIGH); }
void pump1Off() { digitalWrite(PUMP_1, LOW); }
void pump2On() { digitalWrite(PUMP_2, HIGH); }
void pump2Off() { digitalWrite(PUMP_2, LOW); }

// ================== FUNGSI TUNGGU AIR HABIS ==================
void tungguAirHabis() {
  while (true) {
    float jarak = bacaJarak(TRIG_PIN_1, ECHO_PIN_1);
    
    lcd.setCursor(0, 1);
    lcd.print("Jarak: ");
    lcd.print(jarak, 1);
    lcd.print("cm  ");
    
    if (jarak >= JARAK_KOSONG) {
      delay(DELAY_MATI);
      float cek = bacaJarak(TRIG_PIN_1, ECHO_PIN_1);
      if (cek >= JARAK_KOSONG) {
        break;
      }
    }
    delay(200);
  }
}

// ================== FUNGSI TELEGRAM ==================
void kirimTelegram(String pesan) {
  bot.sendMessage(OWNER_CHAT_ID, pesan, "Markdown");
  bot.sendMessage(GROUP_CHAT_ID, pesan, "Markdown");
}

// ================== FUNGSI HANDLE PESAN ==================
void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    
    text.replace(BOT_USERNAME, "");
    text.trim();
    
    if (text == "/start") {
      String welcome = "🌊 *Selamat datang di WAVE Bot!*\n\n";
      welcome += "Halo " + from_name + "!\n\n";
      welcome += "*Command:*\n";
      welcome += "/start - Mulai\n";
      welcome += "/status - Cek status\n";
      welcome += "/turbidity - Turbidity terakhir\n";
      welcome += "/ph - pH terakhir\n";
      welcome += "/help - Bantuan\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
    else if (text == "/status") {
      String status = "📊 *STATUS WAVE*\n";
      status += "━━━━━━━━━━━━━━━\n";
      
      if (dataTurbidityAda) {
        status += "📊 Turbidity: " + String(adcValue) + " (" + statusTurb + ")\n";
      } else {
        status += "📊 Turbidity: Belum ada data\n";
      }
      
      if (dataPHAda) {
        status += "🧪 pH: " + String(pH, 2) + " (" + statusPH + ")\n";
      } else {
        status += "🧪 pH: Belum ada data\n";
      }
      
      status += "💧 Pump 1: " + String(digitalRead(PUMP_1) ? "ON" : "OFF") + "\n";
      status += "💧 Pump 2: " + String(digitalRead(PUMP_2) ? "ON" : "OFF") + "\n";
      status += "📌 Status: " + String(DETEKSI_TURBIDITY ? "Proses" : "Idle");
      
      bot.sendMessage(chat_id, status, "Markdown");
    }
    else if (text == "/turbidity") {
      String msg = "📊 *TURBIDITY TERAKHIR*\n";
      msg += "━━━━━━━━━━━━━━━\n";
      
      if (dataTurbidityAda) {
        msg += "📈 ADC: " + String(adcValue) + "\n";
        msg += "📌 Status: " + statusTurb;
      } else {
        msg += "⚠️ Belum ada data turbidity";
      }
      
      bot.sendMessage(chat_id, msg, "Markdown");
    }
    else if (text == "/ph") {
      String msg = "🧪 *pH TERAKHIR*\n";
      msg += "━━━━━━━━━━━━━━━\n";
      
      if (dataPHAda) {
        msg += "🧪 pH: " + String(pH, 2) + "\n";
        msg += "📌 Status: " + statusPH;
      } else {
        msg += "⚠️ Belum ada data pH";
      }
      
      bot.sendMessage(chat_id, msg, "Markdown");
    }
    else if (text == "/help") {
      String help = "🌊 *WAVE BOT*\n";
      help += "━━━━━━━━━━━━━━━\n";
      help += "*Command:*\n";
      help += "/start - Mulai\n";
      help += "/status - Cek status\n";
      help += "/turbidity - Turbidity terakhir\n";
      help += "/ph - pH terakhir\n";
      help += "/help - Bantuan";
      bot.sendMessage(chat_id, help, "Markdown");
    }
    else {
      bot.sendMessage(chat_id, "❓ Command gak dikenal. Ketik /help", "");
    }
  }
}

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(TRIG_PIN_1, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(TRIG_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_2, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PUMP_1, OUTPUT);
  pinMode(PUMP_2, OUTPUT);
  
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(PUMP_1, LOW);
  digitalWrite(PUMP_2, LOW);
  
  analogReadResolution(10);
  analogSetAttenuation(ADC_11db);
  
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  Serial.println("\nWAVE - FINAL + TELEGRAM");
  
  // ================== KONEKSI WIFI ==================
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connect WiFi...");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  beep1x();
  
  Serial.print("Connect WiFi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("\nWiFi OK");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  
  // WiFi OK
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi OK");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  beep1x();
  delay(2000);
  
  // ================== KONEKSI TELEGRAM ==================
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Telegram...");
  beep1x();
  
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  
  bot.sendMessage(OWNER_CHAT_ID, "🌊 *WAVE Bot aktif!*\nKetik /help buat command.", "Markdown");
  bot.sendMessage(GROUP_CHAT_ID, "🌊 *WAVE Bot aktif!*\nKetik /help buat command.", "Markdown");
  
  // Telegram OK
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Telegram OK");
  beep1x();
  delay(1500);
  
  lcd.clear();
  
  // ================== INTRO ==================
  intro();
}

// ================== LOOP ==================
void loop() {
  // ================== TELEGRAM HANDLE ==================
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
  
  // ============================================================
  // TINWALL 1 — Ultrasonic 1 + Turbidity
  // ============================================================
  float jarak1 = bacaJarak(TRIG_PIN_1, ECHO_PIN_1);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jarak1: ");
  lcd.print(jarak1, 1);
  lcd.print("cm");
  
  if (jarak1 <= JARAK_ADA_AIR) {
    lcd.setCursor(0, 1);
    lcd.print("Validasi...");
    beep1x();
    
    if (cekAirValid(TRIG_PIN_1, ECHO_PIN_1)) {
      Serial.println("Air terdeteksi valid");
      
      kirimTelegram("💧 *AIR TERDETEKSI*\n━━━━━━━━━━━━━━━\n📍 Tinwall 1\n📏 Jarak: " + String(jarak1, 1) + " cm");
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Air terdeteksi");
      beep1x();
      
      buzzer3x();
      delay(1000);
      
      Serial.println("Stabilin 3 detik...");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Stabilin...");
      beep1x();
      delay(WAKTU_STABIL);
      
      Serial.println("Baca turbidity 5 detik...");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Baca turbidity");
      lcd.setCursor(0, 1);
      lcd.print("5 detik...");
      beep1x();
      
      long totalADC = 0;
      int jumlahBaca = 0;
      unsigned long startTime = millis();
      
      while (millis() - startTime < WAKTU_BACA) {
        totalADC += analogRead(TURBIDITY_PIN);
        jumlahBaca++;
        delay(100);
      }
      
      adcValue = totalADC / jumlahBaca;
      float voltage = (adcValue / 1023.0) * 3.3;
      
      if (adcValue >= ADC_JERNIH_MIN && adcValue <= ADC_JERNIH_MAX) {
        statusTurb = "JERNIH";
      } else if (adcValue >= ADC_KERUH_MIN && adcValue <= ADC_KERUH_MAX) {
        statusTurb = "KERUH";
      } else {
        statusTurb = "TIDAK VALID";
      }
      
      Serial.println("-----------------------------");
      Serial.println("HASIL TURBIDITY");
      Serial.println("  ADC    : " + String(adcValue));
      Serial.println("  Voltage: " + String(voltage, 3) + " V");
      Serial.println("  Status : " + statusTurb);
      Serial.println("-----------------------------\n");
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Turb: ");
      lcd.print(adcValue);
      lcd.setCursor(0, 1);
      lcd.print(statusTurb);
      beep1x();
      
      buzzer3x();
      delay(DELAY_HASIL);
      
      // ================== CEK TURBIDITY VALID ==================
      if (statusTurb == "JERNIH" || statusTurb == "KERUH") {
        dataTurbidityAda = true;
        
        kirimTelegram("📊 *HASIL TURBIDITY*\n━━━━━━━━━━━━━━━\n📈 ADC: " + String(adcValue) + "\n⚡ Voltage: " + String(voltage, 3) + " V\n📌 Status: " + statusTurb);
        
        if (statusTurb == "JERNIH") {
          Serial.println("Pump 1 ON...");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Pump 1 ON");
          beep1x();
          
          kirimTelegram("💧 *PUMP 1 ON*\n━━━━━━━━━━━━━━━\n📌 Status: JERNIH\n⏳ Nunggu air habis...");
          
          pump1On();
          tungguAirHabis();
          pump1Off();
        } 
        else if (statusTurb == "KERUH") {
          Serial.println("Pump 2 ON...");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Pump 2 ON");
          beep1x();
          
          kirimTelegram("💧 *PUMP 2 ON*\n━━━━━━━━━━━━━━━\n📌 Status: KERUH\n⏳ Nunggu air habis...");
          
          pump2On();
          tungguAirHabis();
          pump2Off();
        }
        
        buzzer3x();
        
        kirimTelegram("⭕ *AIR HABIS*\n━━━━━━━━━━━━━━━\n✅ Siap ke Tinwall 2");
        
        DETEKSI_TURBIDITY = true;
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Air habis");
        beep1x();
        delay(2000);
        
      } else {
        Serial.println("Turbidity TIDAK VALID - ulang");
        
        kirimTelegram("⚠️ *TURBIDITY TIDAK VALID*\n━━━━━━━━━━━━━━━\n📈 ADC: " + String(adcValue) + "\n📌 Status: " + statusTurb + "\n🔄 Ulang...");
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tidak valid");
        lcd.setCursor(0, 1);
        lcd.print("Ulang...");
        buzzer3x();
        delay(2000);
      }
      
    } else {
      Serial.println("Ultrasonic 1 gak valid");
      lcd.setCursor(0, 1);
      lcd.print("Gak valid     ");
      beep1x();
      delay(500);
    }
    
  } else {
    Serial.println("Tinwall 1 kosong");
    lcd.setCursor(0, 1);
    lcd.print("Gak ada air   ");
  }
  
  // ============================================================
  // TINWALL 2 — Ultrasonic 2 + pH
  // ============================================================
  if (DETEKSI_TURBIDITY) {
    float jarak2 = bacaJarak(TRIG_PIN_2, ECHO_PIN_2);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Jarak2: ");
    lcd.print(jarak2, 1);
    lcd.print("cm");
    
    if (jarak2 <= JARAK_ADA_AIR) {
      lcd.setCursor(0, 1);
      lcd.print("Validasi...");
      beep1x();
      
      if (cekAirValid(TRIG_PIN_2, ECHO_PIN_2)) {
        Serial.println("Air Tinwall 2 valid");
        
        kirimTelegram("💧 *AIR TERDETEKSI*\n━━━━━━━━━━━━━━━\n📍 Tinwall 2\n📏 Jarak: " + String(jarak2, 1) + " cm");
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Air terdeteksi");
        beep1x();
        
        buzzer3x();
        delay(1000);
        
        Serial.println("Stabilin pH 5 detik...");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Stabilin pH...");
        beep1x();
        delay(WAKTU_STABIL_PH);
        
        Serial.println("Baca pH 5 detik...");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Baca pH");
        lcd.setCursor(0, 1);
        lcd.print("5 detik...");
        beep1x();
        
        long totalADC_PH = 0;
        int jumlahBaca_PH = 0;
        unsigned long startTime_PH = millis();
        
        while (millis() - startTime_PH < WAKTU_BACA) {
          totalADC_PH += analogRead(PH_PIN);
          jumlahBaca_PH++;
          delay(100);
        }
        
        int adcPH = totalADC_PH / jumlahBaca_PH;
        float voltagePH = (adcPH / 1023.0) * 3.3;
        pH = (m * voltagePH) + b;
        pH = constrain(pH, 0, 14);
        
        if (pH >= 6.0 && pH <= 7.5) {
          statusPH = "Aman untuk tanaman";
          statusLCD = "Aman";
        } else if (pH < 6.0) {
          statusPH = "Terlalu asam";
          statusLCD = "Asam";
        } else {
          statusPH = "Terlalu basa";
          statusLCD = "Basa";
        }
        
        dataPHAda = true;
        
        Serial.println("-----------------------------");
        Serial.println("HASIL pH");
        Serial.println("  ADC    : " + String(adcPH));
        Serial.println("  Voltage: " + String(voltagePH, 3) + " V");
        Serial.println("  pH     : " + String(pH, 2));
        Serial.println("  Status : " + statusPH);
        Serial.println("-----------------------------\n");
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("pH: ");
        lcd.print(pH, 2);
        lcd.setCursor(0, 1);
        lcd.print(statusLCD);
        beep1x();
        
        buzzer3x();
        delay(DELAY_HASIL);
        
        kirimTelegram("🧪 *HASIL pH*\n━━━━━━━━━━━━━━━\n🧪 pH: " + String(pH, 2) + "\n📌 Status: " + statusPH);
        
        Serial.println("=============================");
        Serial.println("HASIL AKHIR");
        Serial.println("  Turbidity: " + String(adcValue) + " (" + statusTurb + ")");
        Serial.println("  pH       : " + String(pH, 2) + " (" + statusPH + ")");
        Serial.println("=============================\n");
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Turb: ");
        lcd.print(adcValue);
        lcd.setCursor(0, 1);
        lcd.print("pH: ");
        lcd.print(pH, 2);
        beep1x();
        delay(DELAY_HASIL);
        
        lcd.clear();
        lcd.setCursor(0, 0);
        if (statusPH == "Aman untuk tanaman") {
          lcd.print("Air aman");
          lcd.setCursor(0, 1);
          lcd.print("untuk tanaman");
        } else {
          lcd.print("Air tidak aman");
          lcd.setCursor(0, 1);
          lcd.print(statusLCD);
        }
        beep1x();
        delay(DELAY_HASIL);
        
        buzzer3x();
        
        String hasilAkhir = "✅ *WAVE SELESAI*\n";
        hasilAkhir += "━━━━━━━━━━━━━━━\n";
        hasilAkhir += "📊 Turbidity: " + String(adcValue) + " (" + statusTurb + ")\n";
        hasilAkhir += "🧪 pH: " + String(pH, 2) + " (" + statusPH + ")\n";
        if (statusPH == "Aman untuk tanaman") {
          hasilAkhir += "🌱 Air aman untuk tanaman";
        } else {
          hasilAkhir += "⚠️ Air tidak aman untuk tanaman";
        }
        kirimTelegram(hasilAkhir);
        
        DETEKSI_TURBIDITY = false;
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Selesai");
        beep1x();
        delay(2000);
        
      } else {
        Serial.println("Ultrasonic 2 gak valid");
        lcd.setCursor(0, 1);
        lcd.print("Gak valid     ");
        beep1x();
        delay(500);
      }
      
    } else {
      Serial.println("Tinwall 2 kosong");
      lcd.setCursor(0, 1);
      lcd.print("Gak ada air   ");
    }
  }
  
  delay(500);
}