/********** Car-B Decode System (Receiver) **********/
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

  if (millis() - windowStart >= sampleWindow) {

    frequency = (float)pulseCount / 2.0;
    pulseCount = 0;
    windowStart = millis();

    classifyPattern(frequency);
  }
}

/********** Pattern Classifier **********/
void classifyPattern(float freq) {

  // If accident already confirmed → hold display for 3 seconds
  if (accidentLatched) {

    if (millis() - accidentLatchTime < 3000) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Accident Alert!");
      lcd.setCursor(0, 1);
      lcd.print("Freq: ");
      lcd.print(freq, 1);
      lcd.print(" Hz");
      return;
    } 
    
    else {
      accidentLatched = false;        // Release latch after 3 seconds
      accidentConfirmCount = 0;
    }
  }

  // ---- Alcohol Detection (Instant) ----
  if (freq >= 6 && freq <= 10) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alcohol Detected");
    lcd.setCursor(0, 1);
    lcd.print("Freq: ");
    lcd.print(freq, 1);
    lcd.print(" Hz");

    // Reset accident counter when alcohol or idle
    accidentConfirmCount = 0;
    return;
  }

  // ---- Accident Detection (Requires 2 second confirmation) ----
  if (freq >= 10 && freq <= 25) {

    accidentConfirmCount++;

    if (accidentConfirmCount >= 2) {    // 2 consecutive seconds
      accidentLatched = true;
      accidentLatchTime = millis();     // Start 3 second display timer

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Accident Alert!");
      lcd.setCursor(0, 1);
      lcd.print("Confirmed");
      return;
    }

    // Show "checking" status while waiting for 2 confirmations
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Checking Accident");
    lcd.setCursor(0, 1);
    lcd.print("Freq: ");
    lcd.print(freq, 1);
    lcd.print(" Hz");
    return;
  }

  // ---- Idle ----
  accidentConfirmCount = 0;  // Reset confirmation when out of range

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("No Signal / Idle");
  lcd.setCursor(0, 1);
  lcd.print("Freq: ");
  lcd.print(freq, 1);
  lcd.print(" Hz");
}
