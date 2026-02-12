/************************************************************
   Project: Fingerprint-Based Authentication Module
   Board: Arduino UNO / Nano
   Sensor: R307 / R305 / AS608
   Feature: Standalone Authentication + LCD + Relay
************************************************************/

#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// --------- Fingerprint Serial ----------
SoftwareSerial mySerial(2, 3);   // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// --------- LCD ----------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --------- Output Pins ----------
#define BUZZER 6
#define RELAY 7

void setup() {

  Serial.begin(9600);
  mySerial.begin(57600);

  pinMode(BUZZER, OUTPUT);
  pinMode(RELAY, OUTPUT);

  digitalWrite(RELAY, LOW);
  digitalWrite(BUZZER, LOW);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Fingerprint");
  lcd.setCursor(0,1);
  lcd.print("Initializing...");

  if (finger.begin()) {
    lcd.clear();
    lcd.print("Sensor Ready");
    delay(1500);
  } else {
    lcd.clear();
    lcd.print("Sensor Error");
    while (1);
  }

  finger.getTemplateCount();
}

void loop() {

  lcd.setCursor(0,0);
  lcd.print("Place Finger   ");

  int id = getFingerprintID();

  if (id > 0) {
    accessGranted(id);
  }
}

int getFingerprintID() {

  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    accessDenied();
    return -1;
  }

  return finger.fingerID;
}

void accessGranted(int id) {

  lcd.clear();
  lcd.print("Access Granted");
  lcd.setCursor(0,1);
  lcd.print("ID: ");
  lcd.print(id);

  digitalWrite(BUZZER, HIGH);
  digitalWrite(RELAY, HIGH);

  delay(2000);

  digitalWrite(BUZZER, LOW);
  digitalWrite(RELAY, LOW);

  lcd.clear();
}

void accessDenied() {

  lcd.clear();
  lcd.print("Access Denied");

  digitalWrite(BUZZER, HIGH);
  delay(500);
  digitalWrite(BUZZER, LOW);

  delay(1500);
  lcd.clear();
}
