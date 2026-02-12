#include "../config/credentials.h"

#define BLYNK_TEMPLATE_ID BILLING_TEMPLATE_ID
#define BLYNK_TEMPLATE_NAME BILLING_TEMPLATE_NAME
#define BLYNK_AUTH_TOKEN BILLING_AUTH_TOKEN

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_PCF8574.h>
#include <SPI.h>
#include <MFRC522.h>
#include "HX711.h"
#include <time.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASSWORD;

#define VPIN_BILL V7

LiquidCrystal_PCF8574 lcd(0x27);

// Buttons & IO
#define PIN_RESET_BUTTON 5
#define PIN_DONE_BUTTON 4
#define PIN_REMOVE_LAST 13
#define PIN_ADD_LED 2
#define PIN_REMOVE_LED 15
#define PIN_BUZZER 12
#define PIN_GRAIN_BUTTON 14

// RFID
#define SS_PIN 17
#define RST_PIN 16
MFRC522 rfid(SS_PIN, RST_PIN);

// HX711
#define HX711_DOUT_PIN 26
#define HX711_SCK_PIN 27
HX711 scale;

double GRAIN_PRICE_PER_KG = 40.0;

double total = 0;
int itemCount = 0;

void setup() {

  Serial.begin(115200);
  lcd.begin(16,2);
  lcd.setBacklight(1);
  lcd.print("SMART TROLLEY");

  WiFi.begin(ssid, pass);
  Blynk.begin(auth, ssid, pass);

  SPI.begin();
  rfid.PCD_Init();

  scale.begin(HX711_DOUT_PIN, HX711_SCK_PIN);
  scale.set_scale();
  scale.tare();

  delay(1500);
  lcd.clear();
  lcd.print("Add Items...");
}

void loop() {
  Blynk.run();
}
