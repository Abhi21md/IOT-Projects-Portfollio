#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// I2C LCD (change address to 0x3F if 0x27 doesn't work)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DFPlayer serial (SoftwareSerial)
SoftwareSerial mp3(10, 11); // D10 = RX (DFPlayer TX), D11 = TX (DFPlayer RX)

// Flex sensor pins (5 sensors)
const int flex1 = A0;  // Thumb
const int flex2 = A1;  // Index
const int flex3 = A2;  // Middle
const int flex4 = A3;  // Ring
const int flex5 = A6;  // Little finger (analog-only pin on Nano)

int val1, val2, val3, val4, val5;
const int THRESH = 100; // sensor threshold (based on your test)

void setup() {
  Wire.begin();
  lcd.init();
  lcd.backlight();
  mp3.begin(9600);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Patient Assist");
  lcd.setCursor(0,1);
  lcd.print("System Ready");
  delay(1800);
  lcd.clear();
}

void loop() {
  // Read sensors
  val1 = analogRead(flex1);
  val2 = analogRead(flex2);
  val3 = analogRead(flex3);
  val4 = analogRead(flex4);
  val5 = analogRead(flex5);

  // Gesture checks (only one finger bent at a time)
  if (val1 > THRESH && val2 < THRESH && val3 < THRESH && val4 < THRESH && val5 < THRESH) {
    showAndPlay("Need Water", 1);
  }
  else if (val2 > THRESH && val1 < THRESH && val3 < THRESH && val4 < THRESH && val5 < THRESH) {
    showAndPlay("Need Help", 2);
  }
  else if (val3 > THRESH && val1 < THRESH && val2 < THRESH && val4 < THRESH && val5 < THRESH) {
    showAndPlay("I am Hungry", 3);
  }
  else if (val4 > THRESH && val1 < THRESH && val2 < THRESH && val3 < THRESH && val5 < THRESH) {
    showAndPlay("Call Nurse", 4);
  }
  else if (val5 > THRESH && val1 < THRESH && val2 < THRESH && val3 < THRESH && val4 < THRESH) {
    showAndPlay("Need Medicine", 5);
  }
  else {
    lcd.setCursor(0,0);
    lcd.print("Waiting Gesture ");
    lcd.setCursor(0,1);
    lcd.print("                ");
  }

  delay(350);
}

void showAndPlay(const String &message, int track) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Gesture Found:");
  lcd.setCursor(0,1);
  lcd.print(message);

  playTrack(track);
  delay(1700);
  lcd.clear();
}

// Command bytes cast as byte to avoid ambiguity
void playTrack(int num) {
  mp3.write((byte)0x7E);
  mp3.write((byte)0xFF);
  mp3.write((byte)0x06);
  mp3.write((byte)0x03); // play track
  mp3.write((byte)0x00);
  mp3.write((byte)0x00);
  mp3.write((byte)num);
  mp3.write((byte)0xEF);
}