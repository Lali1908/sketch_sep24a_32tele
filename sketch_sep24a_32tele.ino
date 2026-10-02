// Lab 1
// ESP32 + DHT11 + Telegram
// แจ้งเตือน Temperature, Humidity ทุกๆ 60 วินาที


#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "DHT.h"

// ---------------- ตั้งค่า Wi-Fi ----------------
#define WIFI_SSID "ZFlip6Lalita"
#define WIFI_PASSWORD "341457>_8"

// ---------------- ตั้งค่า Telegram ----------------
#define BOTtoken "8896311824:AAE0PfDME57Z654ku4X48vKY3YH2c6mu6_0"
#define CHAT_ID "8734456953"

// ---------------- ตั้งค่า DHT11 ----------------
#define DHTPIN 27         // ขา DATA ต่อที่ D27
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

unsigned long previousMillis = 0;
const long interval = 60000; // ทุกๆ 60000 ms (60 วินาที)

void setup() {
  Serial.begin(115200);
  dht.begin();

  // เชื่อมต่อ Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  // ข้ามการตรวจ Certificate ของ SSL
  client.setInsecure();

  // ส่งข้อความแจ้งเตือนเมื่อบอร์ดเริ่มทำงาน
  bot.sendMessage(CHAT_ID, "ESP32 DHT11 System Started!", "");
}

void loop() {
  unsigned long currentMillis = millis();

  // ทำงานทุกๆ 60 วินาที
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // ตรวจสอบว่าอ่านค่าสำเร็จหรือไม่
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // แสดงค่าออก Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" °C | Humidity: ");
    Serial.print(h);
    Serial.println(" %");

    // เตรียมข้อความส่งเข้า Telegram
    String message = "📄*Report DHT11*\n";
    message += "🌡️ Temp: " + String(t, 1) + " °C\n";
    message += "💧 Humi: " + String(h, 1) + " %";

    // ส่งข้อความไปที่ Telegram
    if (bot.sendMessage(CHAT_ID, message, "Markdown")) {
      Serial.println("Telegram message sent successfully!");
    } else {
      Serial.println("Failed to send Telegram message.");
    }
  }
}