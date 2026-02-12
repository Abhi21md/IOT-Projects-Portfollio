#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- PIN DEFINITIONS ----------------
#define SENSOR 2
#define START_BTN 3
#define STOP_BTN 4
#define RESET_BTN 5
#define RELAY 6

volatile unsigned int count = 0;
unsigned int targetCount = 0;
bool running = false;

// ---------------- INTERRUPT FUNCTION ----------------
void counter() {
  if (running) {
    count++;
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(SENSOR, INPUT_PULLUP);
  pinMode(START_BTN, INPUT_PULLUP);
  pinMode(STOP_BTN, INPUT_PULLUP);
  pinMode(RESET_BTN, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);

  digitalWrite(RELAY, LOW);

  attachInterrupt(digitalPinToInterrupt(SENSOR), counter, FALLING);

  lcd.init();
  lcd.backlight();

  // Read target value from EEPROM
  EEPROM.get(0, targetCount);

  if (targetCount == 0 || targetCount > 10000) {
    targetCount = 10;   // Default value
    EEPROM.put(0, targetCount);
  }

  displayData();
}

void loop() {

  // ---------------- START ----------------
  if (digitalRead(START_BTN) == LOW) {
    delay(200);
    running = true;
    digitalWrite(RELAY, HIGH);
  }

  // ---------------- STOP ----------------
  if (digitalRead(STOP_BTN) == LOW) {
    delay(200);
    running = false;
    digitalWrite(RELAY, LOW);
  }

  // ---------------- RESET ----------------
  if (digitalRead(RESET_BTN) == LOW) {
    delay(200);
    count = 0;
  }

  // ---------------- AUTO STOP ----------------
  if (count >= targetCount) {
    running = false;
    digitalWrite(RELAY, LOW);
  }

  displayData();
}

// ---------------- LCD DISPLAY ----------------
void displayData() {
  lcd.setCursor(0, 0);
  lcd.print("Count: ");
  lcd.print(count);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("Target: ");
  lcd.print(targetCount);
  lcd.print("   ");
}
