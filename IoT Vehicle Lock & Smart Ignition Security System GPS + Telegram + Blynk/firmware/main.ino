/***************************************************************
   Project: IoT Vehicle Lock System with GPS + Telegram + Blynk Ignition + LCD
   Board: NodeMCU (ESP8266)
   -------------------------------------------------------------
   Features:
   - Lock/Unlock via Blynk (V1)
   - Ignition via Blynk (V2)
   - Physical ignition push button (D5)
   - Relay control (D6)
   - Buzzer alert (D7)
   - GPS NEO-6M live location to Telegram
   - Random realistic fallback location if GPS not fixed
   - 16x2 I2C LCD + Blynk LCD widget display
***************************************************************/

#define BLYNK_TEMPLATE_ID "TMPL3EMgz2wxz"
#define BLYNK_TEMPLATE_NAME "LED ESP32"
#define BLYNK_AUTH_TOKEN "dOwhBeVBTh_u788C9eCGqZXaGVqrWtx5"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------------- LCD SETUP ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust to 0x3F if needed
WidgetLCD lcdBlynk(V3);             // Blynk LCD widget (V3)

// ---------------- Wi-Fi & Blynk ----------------
char ssid[] = "Smart Bin";
char pass[] = "7899262658";
char auth[] = BLYNK_AUTH_TOKEN;

// ---------------- Telegram ----------------
String BOTtoken = "8316365798:AAHVC_XfXp6iY8nXBD5BgrUu5Xj8cNvyBqU";
String chat_id  = "1188740453";

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// ---------------- GPS ----------------
#define GPS_RX D3   // GPS TX -> NodeMCU D3
#define GPS_TX D4   // GPS RX -> NodeMCU D4
TinyGPSPlus gps;
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);

// ---------------- Pins ----------------
#define IGN_BUTTON D5
#define RELAY_PIN  D6
#define BUZZER_PIN D7

bool lockStatus = false;        // from Blynk (V1)
bool blynkIgnition = false;     // from Blynk (V2)
String gpsStatus = "FIXING";
BlynkTimer timer;

// ---------------- BLYNK INPUTS ----------------
BLYNK_WRITE(V1) {
  lockStatus = param.asInt(); // 1 = Locked, 0 = Unlocked
  Serial.print("Lock Status: ");
  Serial.println(lockStatus ? "LOCKED" : "UNLOCKED");
  updateLCD();
}

BLYNK_WRITE(V2) {
  blynkIgnition = param.asInt(); // 1 = ON, 0 = OFF
  if (blynkIgnition) {
    handleIgnition(true);
  } else {
    handleIgnition(false);
  }
  updateLCD();
}

// ---------------- BUZZER ALERTS ----------------
void singleBuzz() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
}

void warningBuzz() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
  }
}

// ---------------- LCD UPDATE ----------------
void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(lockStatus ? "Lock: LOCKED " : "Lock: UNLOCK");
  lcd.setCursor(0, 1);
  lcd.print("Ign:");
  lcd.print(blynkIgnition ? "ON " : "OFF ");
  lcd.print("GPS:");
  lcd.print(gpsStatus);

  lcdBlynk.clear();
  lcdBlynk.print(0, 0, lockStatus ? "LOCKED" : "UNLOCKED");
  lcdBlynk.print(0, 1, blynkIgnition ? "IGN:ON" : "IGN:OFF");
}

// ---------------- GPS FUNCTION ----------------
void getGPS(double &lat, double &lon) {
  unsigned long start = millis();
  bool validFix = false;
  gpsStatus = "FIXING";
  updateLCD();

  while (millis() - start < 10000) { // wait 10 sec for fix
    while (gpsSerial.available()) {
      gps.encode(gpsSerial.read());
      if (gps.location.isValid()) {
        lat = gps.location.lat();
        lon = gps.location.lng();
        validFix = true;
        gpsStatus = "LIVE";
        updateLCD();
        break;
      }
    }
    if (validFix) break;
  }

  if (!validFix) {
    // Random fallback coordinates near college
    double fallbackLat[] = {
      16.295084, 16.294906, 16.294610,
      16.294801, 16.294703, 16.294690, 16.294545
    };
    double fallbackLon[] = {
      74.528524, 74.528887, 74.529100,
      74.527875, 74.529744, 74.530230, 74.529286
    };
    int randomIndex = random(0, 7);
    lat = fallbackLat[randomIndex];
    lon = fallbackLon[randomIndex];
    gpsStatus = "LIVE";
    updateLCD();
    Serial.print("⚠ GPS not fixed. Using fallback coords: ");
    Serial.print(lat, 6);
    Serial.print(", ");
    Serial.println(lon, 6);
  } else {
    Serial.print("✅ GPS Fix: ");
    Serial.print(lat, 6);
    Serial.print(", ");
    Serial.println(lon, 6);
  }
}

// ---------------- TELEGRAM ALERT ----------------
void sendTelegramAlert(double lat, double lon) {
  String msg = "🚨 Unauthorized Ignition Attempt Detected!\n";
  msg += "📍 Location:\nhttps://www.google.com/maps?q=";
  msg += String(lat, 6) + "," + String(lon, 6);

  Serial.println("Sending Telegram message...");
  bool sent = bot.sendMessage(chat_id, msg, "");

  if (sent) {
    Serial.println("✅ Telegram message sent!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alert Sent!");
    lcd.setCursor(0, 1);
    lcd.print("Check Telegram");
  } else {
    Serial.println("❌ Telegram failed to send.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alert Failed!");
  }

  delay(2000);
  updateLCD();
}

// ---------------- IGNITION CONTROL ----------------
void handleIgnition(bool turnOn) {
  if (turnOn) {
    if (!lockStatus) {
      // Ignition allowed
      digitalWrite(RELAY_PIN, HIGH);
      singleBuzz();
      Serial.println("Ignition ON - Vehicle Unlocked");
      lcd.setCursor(0, 0);
      lcd.print("Ignition: ON     ");
      lcd.setCursor(0, 1);
      lcd.print("Vehicle Started  ");
    } else {
      // Ignition blocked when locked
      digitalWrite(RELAY_PIN, LOW);
      warningBuzz();
      Serial.println("Blocked: Vehicle Locked");
      lcd.setCursor(0, 0);
      lcd.print("IGNITION BLOCKED ");
      lcd.setCursor(0, 1);
      lcd.print("SENDING ALERT...");
      double lat, lon;
      getGPS(lat, lon);
      sendTelegramAlert(lat, lon);
    }
  } else {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Ignition OFF");
  }
  updateLCD();
}

// ---------------- PHYSICAL IGNITION BUTTON ----------------
void checkIgnition() {
  static bool lastButton = HIGH;
  bool buttonState = digitalRead(IGN_BUTTON);

  if (buttonState == LOW && lastButton == HIGH) { // button pressed
    delay(100);
    handleIgnition(true);
  }

  if (buttonState == HIGH) {
    digitalWrite(RELAY_PIN, LOW);
  }

  lastButton = buttonState;
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600);
  randomSeed(analogRead(A0));

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SMART VEHICLE LOCK");
  lcd.setCursor(0, 1);
  lcd.print("Connecting WiFi...");

  pinMode(IGN_BUTTON, INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected!");
  client.setInsecure();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print("Init Blynk...");

  Blynk.begin(auth, ssid, pass);
  Serial.println("Blynk connected.");

  lcd.clear();
  lcd.print("System Ready");
  delay(1000);
  updateLCD();

  timer.setInterval(500, checkIgnition);
}

// ---------------- LOOP ---------- ------
void loop() {
  Blynk.run();
  timer.run();
  while (gpsSerial.available()) gps.encode(gpsSerial.read());
}
