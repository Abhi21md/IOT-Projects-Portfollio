
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LDR_PIN 2

LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long windowStart = 0;
const unsigned long sampleWindow = 1000;

int pulseCount = 0;
int lastState = HIGH;

float frequency = 0;

// Accident detection timing
int accidentConfirmCount = 0;      // Counts how many consecutive seconds in accident range
bool accidentLatched = false;      // True when accident confirmed
unsigned long accidentLatchTime = 0;

void setup() {
  pinMode(LDR_PIN, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Car Decode");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();

  windowStart = millis();
}

void loop() {
  int currentState = digitalRead(LDR_PIN);

  if (currentState != lastState) {
    pulseCount++;
    lastState = currentState;
  }