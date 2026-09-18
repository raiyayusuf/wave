#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// ================== KONFIGURASI WIFI ==================
// const char* ssid = "omah azel";
// const char* password = "azelia545177";

// const char* ssid = "iyanwifi";
// const char* password = "iyanganteng";

const char* ssid = "IG : @Triwahyu45";
const char* password = "@Aguswahyu45";

// ================== KONFIGURASI TELEGRAM ==================
#define BOT_TOKEN "8983741538:AAH0LSuUqc2Z7hQwiTR8ejF5QgzurCeDi5Q"
#define OWNER_CHAT_ID "8149678877"

// ================== OBJEK ==================
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// ================== VARIABEL ==================
unsigned long lastTimeBotRan;
const unsigned long botRequestDelay = 100;  // 100ms (lebih cepat)
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

// ================== FUNGSI HANDLE PESAN ==================
void handleNewMessages(int numNewMessages) {
  Serial.println(getTimestamp() + " 📥 Ada " + String(numNewMessages) + " pesan baru!");

  for (int i = 0; i < numNewMessages; i++) {
    unsigned long t0 = millis();
    
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;

    Serial.println(getTimestamp() + " ─────────────────────────────");
    Serial.println(getTimestamp() + " 👤 Dari: " + from_name);
    Serial.println(getTimestamp() + " 🆔 Chat ID: " + chat_id);
    Serial.println(getTimestamp() + " 💬 Pesan: " + text);

    if (text == "/start") {
      String welcome = "🌊 *Selamat datang di WAVE Bot!*\n\n";
      welcome += "Halo " + from_name + "!\n\n";
      welcome += "Command yang tersedia:\n";
      welcome += "/start - Mulai\n";
      welcome += "/status - Cek status\n";
      welcome += "/ping - Test koneksi\n";
      
      unsigned long t1 = millis();
      bot.sendMessage(chat_id, welcome, "Markdown");
      unsigned long t2 = millis();
      
      Serial.println(getTimestamp() + " 📤 Balas /start (" + String(t2 - t1) + " ms)");
      Serial.println(getTimestamp() + " ⏱️ Total: " + String(t2 - t0) + " ms");
    }
    else if (text == "/status") {
      String status = "📊 *STATUS WAVE*\n\n";
      status += "🌊 Turbidity: 45%\n";
      status += "🧪 pH: 7.2\n";
      status += "💧 Pump 1: OFF\n";
      status += "💧 Pump 2: ON\n";
      
      unsigned long t1 = millis();
      bot.sendMessage(chat_id, status, "Markdown");
      unsigned long t2 = millis();
      
      Serial.println(getTimestamp() + " 📤 Balas /status (" + String(t2 - t1) + " ms)");
    }
    else if (text == "/ping") {
      unsigned long t1 = millis();
      bot.sendMessage(chat_id, "🏓 Pong! ESP32 aktif.", "");
      unsigned long t2 = millis();
      
      Serial.println(getTimestamp() + " 📤 Balas /ping (" + String(t2 - t1) + " ms)");
    }
    else {
      bot.sendMessage(chat_id, "❓ Command gak dikenal. Ketik /start", "");
    }
    
    Serial.println(getTimestamp() + " ─────────────────────────────");
  }
}

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);
  delay(1000);
  bootTime = millis();
  
  Serial.println("\n\n🌊 WAVE Bot Starting...");
  Serial.println(getTimestamp() + " Boot time: " + String(bootTime) + " ms");

  Serial.print(getTimestamp() + " Connect ke WiFi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\n" + getTimestamp() + " ✅ WiFi connected!");
  Serial.println(getTimestamp() + " IP: " + WiFi.localIP().toString());
  Serial.println(getTimestamp() + " RSSI: " + String(WiFi.RSSI()) + " dBm");

  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  unsigned long t1 = millis();
  bot.sendMessage(OWNER_CHAT_ID, "🌊 WAVE Bot aktif!\nKetik /start buat mulai.", "");
  unsigned long t2 = millis();
  
  Serial.println(getTimestamp() + " 📤 Startup message (" + String(t2 - t1) + " ms)");
  Serial.println(getTimestamp() + " ✅ Bot siap!");
}

// ================== LOOP ==================
void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    unsigned long t0 = millis();
    
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    
    unsigned long t1 = millis();
    
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    
    if (t1 - t0 > 50) {
      Serial.println(getTimestamp() + " 🔄 Fetch update: " + String(t1 - t0) + " ms");
    }
    
    lastTimeBotRan = millis();
  }
}