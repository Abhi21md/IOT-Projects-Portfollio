// SMART IRRIGATION SYSTEM (Auto + Manual)
// Author: Abhishek Madiwal
// NodeMCU + DHT11 + Soil Moisture + Relay + LCD + Blynk IoT
// Relay Logic: HIGH = Motor ON, LOW = Motor OFF

#define BLYNK_TEMPLATE_ID "TMPL3_vvWM5He"
#define BLYNK_TEMPLATE_NAME "Irrigation"
#define BLYNK_AUTH_TOKEN "y0F8SplU5mxDDHvWM9Xf6eXclyuYnfAQ"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// WiFi credentials
char ssid[] = "Smart Bin";
char pass[] = "7899262658";
char auth[] = BLYNK_AUTH_TOKEN;

// ---------------- PIN CONFIG ----------------
#define DHTPIN     D4
#define DHTTYPE    DHT11
#define RELAY_PIN  D7
#define SOIL_PIN   A0

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// ---------------- CONSTANTS ----------------
const float SOIL_WET_THRESHOLD = 90.0;

// ---------------- STATES ----------------
bool pumpState = false;
bool autoMode = true;
bool manualPumpRequest = false;

// ---------------- BLYNK PINS ----------------
#define VP_SOIL        V0
#define VP_TEMP        V1
#define VP_HUM         V2    // ✅ Humidity Gauge
#define VP_MANUAL     V10
#define VP_AUTO        V4
#define VP_PUMP_STATUS V5

// ---------------- FUNCTIONS ----------------
float readSoilPercent() {
  int raw = analogRead(SOIL_PIN);
  int dry = 350;
  int wet = 860;

  raw = constrain(raw, dry, wet);
  float percent = (float)(raw - dry) / (wet - dry) * 100.0;
  return constrain(percent, 0.0, 100.0);
}

void controlPump(float soil) {
  if (autoMode) {
    pumpState = (soil >= SOIL_WET_THRESHOLD);
  } else {
    pumpState = manualPumpRequest;
  }

  digitalWrite(RELAY_PIN, pumpState ? HIGH : LOW);
  Blynk.virtualWrite(VP_PUMP_STATUS, pumpState ? 255 : 0);
}

// ---------------- BLYNK HANDLERS ----------------
BLYNK_WRITE(VP_MANUAL) {
  if (autoMode) {
    Blynk.virtualWrite(VP_MANUAL, 0);
    return;
  }
  manualPumpRequest = param.asInt();
  controlPump(readSoilPercent());
}

BLYNK_WRITE(VP_AUTO) {
  autoMode = param.asInt();
  if (autoMode) {
    manualPumpRequest = false;
    Blynk.virtualWrite(VP_MANUAL, 0);
  }
  controlPump(readSoilPercent());
}

// ---------------- SENSOR UPDATE ----------------
void updateSensors() {
  float soil = readSoilPercent();
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();

  controlPump(soil);

  // -------- LCD DISPLAY --------
  lcd.clear();
  lcd.setCursor(0, 0);
  if (!isnan(temp) && !isnan(hum)) {
    lcd.print("T:");
    lcd.print(temp, 1);
    lcd.print((char)223);
    lcd.print("C ");
    lcd.print("H:");
    lcd.print((int)hum);
    lcd.print("%");
  } else {
    lcd.print("DHT Error");
  }

  lcd.setCursor(0, 1);
  lcd.print("Soil:");
  lcd.print((int)soil);
  lcd.print("% ");
  lcd.print(pumpState ? "ON " : "OFF");
  lcd.print(autoMode ? "A" : "M");

  // -------- BLYNK UPDATE --------
  Blynk.virtualWrite(VP_SOIL, (int)soil);
  Blynk.virtualWrite(VP_TEMP, temp);
  Blynk.virtualWrite(VP_HUM, hum);          // ✅ Humidity Gauge
  Blynk.virtualWrite(VP_PUMP_STATUS, pumpState ? 255 : 0);
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  lcd.init();
  lcd.backlight();
  lcd.print("Smart Irrigation");
  delay(1500);
  lcd.clear();

  dht.begin();
  Blynk.begin(auth, ssid, pass);

  timer.setInterval(7000L, updateSensors);
  updateSensors();
}

// ---------------- LOOP ----------------
void loop() {
  Blynk.run();
  timer.run();
}
