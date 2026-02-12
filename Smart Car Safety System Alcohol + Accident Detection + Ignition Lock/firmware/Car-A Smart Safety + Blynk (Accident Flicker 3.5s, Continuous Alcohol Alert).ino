//********** Car-A Smart Safety + Blynk (Accident Flicker 3.5s, Continuous Alcohol Alert) **********/
#define BLYNK_TEMPLATE_ID "TMPL3vjrAeNMK"
#define BLYNK_TEMPLATE_NAME "Smart Car Safety"
#define BLYNK_AUTH_TOKEN "0OE_axFoGaA9snoAepW3Yt3LqaywCMLd"

#include <Wire.h>
#include <MPU6050.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// ---- Pin configuration ----
int alcoholSensor = A0;
int ignitionRelay = D6;
int buzzer = D7;
int alertLED = D5;   // LED indicator

// Set true if LED is active LOW (ON when pin = LOW)
bool LED_ACTIVE_LOW = false;

int alcoholThreshold = 1000;
bool alcoholAlertActive = false;
bool accidentAlertActive = false;
bool systemReady = false;

unsigned long previousMillis = 0;
unsigned long accidentStartTime = 0;
bool accidentTimerActive = false;

int ledState = LOW;
int patternType = 0;  // 0=none, 1=alcohol, 2=accident

MPU6050 mpu;
BlynkTimer timer;
// ============ WIFI ===============
char ssid[] = "Smart Car";
char pass[] = "7899262658";

// Blynk virtual pins
#define V_ALCOHOL     V3
#define V_IGNITION    V4
#define V_LCD         V5
#define V_THRESHOLD   V6

/********** LCD Display **********/
void showLCD(String msg) { Blynk.virtualWrite(V_LCD, msg); }

/********** LED Helper **********/
void ledON()  { digitalWrite(alertLED, LED_ACTIVE_LOW ? LOW : HIGH); }
void ledOFF() { digitalWrite(alertLED, LED_ACTIVE_LOW ? HIGH : LOW); }

/********** LED Pattern Controller **********/
void updateLEDPattern() {
  unsigned long currentMillis = millis();

  // --- Alcohol pattern: continuous 8Hz flashing ---
  if (patternType == 1) {
    if (currentMillis - previousMillis >= 60) {  // 8Hz blink (120ms total period)
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(alertLED, LED_ACTIVE_LOW ? !ledState : ledState);
    }
  }

  // --- Accident pattern: fast flicker for 3.5s only ---
  else if (patternType == 2 && accidentTimerActive) {
    if (currentMillis - previousMillis >= 25) {  // 20Hz blink (50ms total period)
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(alertLED, LED_ACTIVE_LOW ? !ledState : ledState);
    }

    // Stop after 3.5 seconds
    if (millis() - accidentStartTime > 3500) {
      accidentTimerActive = false;
      patternType = 0;
      ledOFF();
    }
  }

  // --- Normal state ---
  else {
    ledOFF();
  }
}

/********** Setup **********/
void setup() {
  Serial.begin(9600);

  pinMode(ignitionRelay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(alertLED, OUTPUT);

  digitalWrite(ignitionRelay, HIGH);
  digitalWrite(buzzer, LOW);
  ledOFF();

  Wire.begin();  // D2 (SDA), D1 (SCL)
  mpu.initialize();

  WiFi.mode(WIFI_STA);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(300, checkSensors);
  timer.setInterval(20, updateLEDPattern);

  showLCD("System Ready");
  systemReady = true;

  Serial.println("System initialized successfully.");
}

/********** Loop **********/
void loop() {
  Blynk.run();
  timer.run();
}

/********** Sensor Monitoring **********/
void checkSensors() {
  if (!systemReady) return;

  int alcoholValue = analogRead(alcoholSensor);
  Blynk.virtualWrite(V_ALCOHOL, alcoholValue);

  // --- Alcohol Detection (continuous blinking) ---
  if (alcoholValue > alcoholThreshold) {
    digitalWrite(ignitionRelay, LOW);
    digitalWrite(buzzer, HIGH);

    if (!alcoholAlertActive) {
      Blynk.logEvent("alcohol", "Alcohol Detected in Vehicle");
      showLCD("Alcohol Detected");
      alcoholAlertActive = true;
      patternType = 1;  // Continuous 8Hz blink
    }
  } else {
    alcoholAlertActive = false;
  }

  // --- Crash Detection (3.5s flicker only) ---
  long shock = abs(mpu.getAccelerationX()) +
               abs(mpu.getAccelerationY()) +
               abs(mpu.getAccelerationZ());

  if (shock > 60000) {
    if (!accidentAlertActive) {
      Blynk.logEvent("accident", "Accident Detected");
      showLCD("Accident Detected!");
      digitalWrite(buzzer, HIGH);
      accidentAlertActive = true;
      patternType = 2;
      accidentTimerActive = true;
      accidentStartTime = millis();
    }
  } else {
    accidentAlertActive = false;
  }

  // --- Normal State Reset ---
  if (!alcoholAlertActive && !accidentTimerActive) {
    digitalWrite(ignitionRelay, HIGH);
    digitalWrite(buzzer, LOW);
    patternType = 0;
    ledOFF();
    showLCD("Ignition ON");
  }
}

/********** Blynk Controls **********/
BLYNK_WRITE(V_THRESHOLD) { alcoholThreshold = param.asInt(); }   



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
