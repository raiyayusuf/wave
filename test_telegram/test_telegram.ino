#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// ================== KONFIGURASI ==================
// const char* ssid = "omah azel";
// const char* password = "azelia545177";

// const char* ssid = "iyanwifi";
// const char* password = "iyanganteng";

const char* ssid = "IG : @Triwahyu45";
const char* password = "@Aguswahyu45";

#define BOT_TOKEN "8983741538:AAH0LSuUqc2Z7hQwiTR8ejF5QgzurCeDi5Q"
#define CHAT_ID "8149678877"

// ================== OBJEK ==================
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// ================== VARIABEL ==================
unsigned long lastTimeBotRan;
const unsigned long botRequestDelay = 1000;

// ================== FUNGSI HANDLE PESAN ==================
void handleNewMessages(int numNewMessages) {
  Serial.println("Handle pesan baru...");
  Serial.println("Jumlah pesan: " + String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;

    Serial.println("Dari: " + from_name);
    Serial.println("Pesan: " + text);

    if (text == "/start") {
      String welcome = "🌊 *Selamat datang di WAVE Bot!*\n\n";
      welcome += "Halo " + from_name + "!\n\n";
      welcome += "Bot ini buat test koneksi ESP32 ke Telegram.\n\n";
      welcome += "Command yang tersedia:\n";
      welcome += "/start - Mulai\n";
      welcome += "/status - Cek status\n";
      welcome += "/ping - Test koneksi\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
    else if (text == "/status") {
      String status = "📊 *STATUS WAVE*\n\n";
      status += "🌊 Turbidity: 45%\n";
      status += "🧪 pH: 7.2\n";
      status += "💧 Pump 1: OFF\n";
      status += "💧 Pump 2: ON\n";
      status += "⚠️ Air: KOTOR\n";
      bot.sendMessage(chat_id, status, "Markdown");
    }
    else if (text == "/ping") {
      bot.sendMessage(chat_id, "🏓 Pong! ESP32 aktif.", "");
    }
    else {
      bot.sendMessage(chat_id, "❓ Command gak dikenal. Ketik /start", "");
    }
  }
}

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n🌊 WAVE Bot Starting...");

  Serial.print("Connect ke WiFi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\n✅ WiFi connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  bot.sendMessage(CHAT_ID, "🌊 WAVE Bot aktif!\nKetik /start buat mulai.", "");
}

// ================== LOOP ==================
void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("Ada pesan baru!");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeBotRan = millis();
  }
}