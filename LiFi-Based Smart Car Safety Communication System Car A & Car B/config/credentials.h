lifi fetured so added below decode code also /* Nodemcu ESP8266 WIFI control car with Smoke Detection (MQ3)
 * Based on Sritu Hobby code + MQ3 integration
 */

#define BLYNK_TEMPLATE_ID "TMPL3tKnm_wB9"
#define BLYNK_TEMPLATE_NAME "Robo Car"
#define BLYNK_AUTH_TOKEN "1c_8W6u7EIighyAD-XfyOnYiCDc3mz-l"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Motor pins
#define ENA D0
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define ENB D5

// MQ3 Smoke Sensor
#define MQ3_PIN A0

bool forward = 0;
bool backward = 0;
bool left = 0;
bool right = 0;

int Speed = 0;
int smokeValue = 0;
int smokeThreshold = 400;   // default threshold
bool smokeLock = false;

// WiFi credentials
char auth[] = "1c_8W6u7EIighyAD-XfyOnYiCDc3mz-l";
char ssid[] = "Smart Car";
char pass[] = "7899262658";

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Blynk.begin(auth, ssid, pass);
}

// ---------------- BLYNK CONTROLS ----------------

BLYNK_WRITE(V0) { forward  = param.asInt(); }
BLYNK_WRITE(V1) { backward = param.asInt(); }
BLYNK_WRITE(V2) { left     = param.asInt(); }
BLYNK_WRITE(V3) { right    = param.asInt(); }
BLYNK_WRITE(V4) { Speed    = param.asInt(); }

// Slider to set smoke threshold
BLYNK_WRITE(V5) {
  smokeThreshold = param.asInt();
}

// ---------------- MAIN LOOP ----------------

void loop() {
  Blynk.run();
  readSmoke();
  smartcar();
}

// ---------------- SMOKE LOGIC ----------------

void readSmoke() {
  smokeValue = analogRead(MQ3_PIN);

  // Send live smoke value to Gauge
  Blynk.virtualWrite(V6, smokeValue);

  Serial.print("Smoke: ");
  Serial.print(smokeValue);
  Serial.print("  Threshold: ");
  Serial.println(smokeThreshold);

  if (smokeValue > smokeThreshold) {
    if (!smokeLock) {
      smokeLock = true;
      carStop();
      Blynk.logEvent("smoke_detected", "Smoke Detected! Car Locked");
      Serial.println("SMOKE DETECTED - CAR LOCKED");
    }
  } else {
    if (smokeLock) {
      smokeLock = false;
      Serial.println("Smoke Normal - Car Unlocked");
    }
  }
}

// ---------------- CAR CONTROL ----------------

void smartcar() {
  if (smokeLock) {
    carStop();
    return;   // block movement
  }

  if (forward) {
    carforward();
  } else if (backward) {
    carbackward();
  } else if (left) {
    carturnleft();
  } else if (right) {
    carturnright();
  } else {
    carStop();
  }
}

void carforward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carbackward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carturnleft() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carturnright() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}      docode of lifi pattern is in below code /********** Car-B Decode System (Receiver) **********/
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
 