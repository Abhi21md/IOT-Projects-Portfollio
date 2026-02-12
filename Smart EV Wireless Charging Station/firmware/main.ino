/* 
   Smart EV Charging Station - NodeMCU (ESP8266)
   Revised: non-blocking buzzer + robust ultrasonic sampling
   ------------------------------------------------
   - Ultrasonic sampling: multi-sample + average (better at short range)
   - Non-blocking buzzer scheduler (no delay())
   - All other logic preserved: V1 time, V2 manual, V3 LED, auto-start <=5cm, ignore >15cm
*/

#define BLYNK_TEMPLATE_NAME "Wireless Car Charging"
#define BLYNK_AUTH_TOKEN "AahUlLCbobUW1wNRW3lD2RvzTF9-viy6"
#define BLYNK_TEMPLATE_ID "TMPL3UNuiJPWL"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// -------- WiFi & Blynk Credentials --------
char auth[] = "AahUlLCbobUW1wNRW3lD2RvzTF9-viy6";
char ssid[] = "Smart Bin";
char pass[] = "7899262658";

// -------- Pin Assignments --------
const uint8_t relayPin  = D7; // Relay module
const uint8_t trigPin   = D6; // Ultrasonic TRIG
const uint8_t echoPin   = D5; // Ultrasonic ECHO (3.3V divider required)
const uint8_t buzzerPin = D4; // Active buzzer (HIGH = on)

// I2C LCD (SDA=D2, SCL=D1)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// -------- Variables --------
bool charging = false;
bool readyToCharge = false;
bool relayState = false;
unsigned long startMillis = 0;
unsigned long elapsedMillis = 0;
unsigned long chargeLimitMs = 0;
int userMinutes = 0;
bool carDetected = false;

// -------- Ultrasonic Sampling Settings --------
const int ULTRA_SAMPLES = 5;         // number of readings to average
const unsigned long PULSE_TIMEOUT = 30000UL; // microseconds for pulseIn timeout

// -------- Non-blocking Buzzer Scheduler --------
// pattern: 0 = idle, 1 = once, 2 = twice, 3 = triple, 4 = long, 5 = continuous on, 6 = continuous off
volatile uint8_t buzzerPattern = 0;
unsigned long buzzerNextToggle = 0;
int buzzerPhase = 0;        // phase within pattern (on/off segments)
bool buzzerState = false;   // current pin state

// pattern timing (ms)
const unsigned long BEEP_ON  = 120;
const unsigned long BEEP_OFF = 100;
const unsigned long LONG_BEEP_MS = 800;

// schedule a buzzer pattern (thread-safe enough for this sketch)
void scheduleBuzzer(uint8_t pattern) {
  buzzerPattern = pattern;
  buzzerPhase = 0;
  buzzerNextToggle = millis(); // start immediately
  buzzerState = false;
  digitalWrite(buzzerPin, LOW);
}

// called every loop to handle buzzer without blocking
void handleBuzzer() {
  unsigned long now = millis();
  if (buzzerPattern == 0) return;

  // Continuous patterns
  if (buzzerPattern == 5) { // continuous ON
    if (!buzzerState) { digitalWrite(buzzerPin, HIGH); buzzerState = true; }
    return;
  }
  if (buzzerPattern == 6) { // continuous OFF
    if (buzzerState) { digitalWrite(buzzerPin, LOW); buzzerState = false; }
    buzzerPattern = 0; // done
    return;
  }

  // One-shot patterns (1,2,3,4)
  if (now < buzzerNextToggle) return;

  if (buzzerPattern == 4) {
    // long beep: one ON for LONG_BEEP_MS then off
    if (buzzerPhase == 0) {
      digitalWrite(buzzerPin, HIGH);
      buzzerState = true;
      buzzerNextToggle = now + LONG_BEEP_MS;
      buzzerPhase = 1;
    } else {
      digitalWrite(buzzerPin, LOW);
      buzzerState = false;
      buzzerPattern = 0;
      buzzerPhase = 0;
    }
    return;
  }

  // patterns 1/2/3: short beeps repeated
  int count = buzzerPattern; // number of beeps
  if (buzzerPhase % 2 == 0) {
    // turn ON
    digitalWrite(buzzerPin, HIGH);
    buzzerState = true;
    buzzerNextToggle = now + BEEP_ON;
    buzzerPhase++;
  } else {
    // turn OFF (between beeps)
    digitalWrite(buzzerPin, LOW);
    buzzerState = false;
    buzzerPhase++;
    if (buzzerPhase/2 >= count) {
      // finished pattern
      buzzerPattern = 0;
      buzzerPhase = 0;
    } else {
      buzzerNextToggle = now + BEEP_OFF;
    }
  }
}

// -------- Ultrasonic Helpers (multi-sample, ignore zeros) --------
unsigned long getPulseDurationOnce() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  unsigned long duration = pulseIn(echoPin, HIGH, PULSE_TIMEOUT);
  return duration;
}

long getDistanceCmAvg() {
  unsigned long sum = 0;
  int valid = 0;
  for (int i = 0; i < ULTRA_SAMPLES; ++i) {
    unsigned long d = getPulseDurationOnce();
    if (d > 0 && d < PULSE_TIMEOUT) {
      long cm = (long)(d / 58UL);
      // ignore clearly wrong large values
      if (cm > 2 && cm < 400) {
        sum += cm;
        valid++;
      }
    }
    // small pause between samples (non-blocking friendly)
    delayMicroseconds(2000);
  }
  if (valid == 0) return 999;
  return (long)(sum / valid);
}

// -------- Relay Control --------
void startCharging(const char* reason) {
  digitalWrite(relayPin, HIGH);
  relayState = true;
  charging = true;
  startMillis = millis();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Charging ON");
  lcd.setCursor(0, 1);
  lcd.print(String(userMinutes) + " min set");
  scheduleBuzzer(4); // long beep
  Blynk.virtualWrite(V3, 255); // LED ON
  Serial.print("Start: "); Serial.println(reason);
}

void stopCharging(const char* reason) {
  digitalWrite(relayPin, LOW);
  relayState = false;
  charging = false;
  elapsedMillis = millis() - startMillis;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Charging Stopped");
  lcd.setCursor(0, 1);
  unsigned long secs = elapsedMillis / 1000UL;
  lcd.print(String(secs / 60) + "m " + String(secs % 60) + "s");
  scheduleBuzzer(3); // triple beep
  Blynk.virtualWrite(V3, 0); // LED OFF
  Serial.print("Stop: "); Serial.println(reason);
}

// -------- Blynk Controls --------
BLYNK_WRITE(V1) {
  userMinutes = param.asInt();
  chargeLimitMs = (unsigned long)userMinutes * 60000UL;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set: " + String(userMinutes) + " min");
  lcd.setCursor(0, 1);
  lcd.print("Waiting...");
  Serial.print("Time set: "); Serial.println(userMinutes);
}

BLYNK_WRITE(V2) {
  int val = param.asInt();
  if (val == 1) {
    if (userMinutes <= 0) {
      userMinutes = 30;
      chargeLimitMs = (unsigned long)userMinutes * 60000UL;
    }
    if (!charging) startCharging("Manual");
  } else {
    if (charging) stopCharging("Manual");
  }
}

// -------- Setup --------
void setup() {
  Serial.begin(115200);

  pinMode(relayPin, OUTPUT); digitalWrite(relayPin, LOW);
  pinMode(trigPin, OUTPUT);  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT); digitalWrite(buzzerPin, LOW);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Charger");
  lcd.setCursor(0, 1);
  lcd.print("Connecting...");

  Blynk.begin(auth, ssid, pass);

  lcd.clear();
  lcd.print("No Car Detected");
  lcd.setCursor(0, 1);
  lcd.print("Ready...");
  Blynk.virtualWrite(V3, 0);

  // start with buzzer idle
  scheduleBuzzer(6); // ensure off
}

// -------- Main Loop --------
void loop() {
  Blynk.run();

  // handle buzzer (non-blocking)
  handleBuzzer();

  long distance = getDistanceCmAvg(); // averaged reading

  // Ignore readings beyond 15cm (no car)
  if (distance > 15) {
    if (carDetected) {
      carDetected = false;
      readyToCharge = false;
      if (charging) stopCharging("Car Moved Away");
    }
    lcd.setCursor(0, 0);
    lcd.print("No Car Detected ");
    lcd.setCursor(0, 1);
    lcd.print("Waiting...      ");
    scheduleBuzzer(6); // ensure off
  } else {
    carDetected = true;

    // Parking guidance (reworked: less blocking and uses non-blocking buzzer)
    if (distance > 10 && distance <= 15) {
      lcd.setCursor(0, 0); lcd.print("Move Closer...  ");
      lcd.setCursor(0, 1); lcd.print("Dist: " + String(distance) + "cm ");
      // single short beep once per new entry to this range
      static long lastBeepTime = 0;
      if (millis() - lastBeepTime > 1200) { scheduleBuzzer(1); lastBeepTime = millis(); }
      readyToCharge = false;
      scheduleBuzzer(6); // not continuous
    } 
    else if (distance > 5 && distance <= 10) {
      lcd.setCursor(0, 0); lcd.print("Stop Now! Ready ");
      lcd.setCursor(0, 1); lcd.print("Dist: " + String(distance) + "cm ");
      static long lastBeepTime2 = 0;
      if (millis() - lastBeepTime2 > 1200) { scheduleBuzzer(2); lastBeepTime2 = millis(); }
      readyToCharge = true;
      scheduleBuzzer(6);
    } 
    else if (distance <= 5) {
      // Perfect
      lcd.setCursor(0, 0); lcd.print("Perfect Parking ");
      lcd.setCursor(0, 1); lcd.print("Dist: " + String(distance) + "cm ");
      readyToCharge = true;
      scheduleBuzzer(6);

      // Auto start if not charging and time set
      if (!charging && chargeLimitMs > 0) {
        startCharging("Auto");
      } else if (charging) {
        lcd.setCursor(0,0); lcd.print("Charging ON     ");
        // keep showing distance/time below
      }
    }
  }

  // Charging time monitor (update LCD every second)
  if (charging) {
    unsigned long now = millis();
    elapsedMillis = now - startMillis;
    static unsigned long lastUi = 0;
    if (now - lastUi >= 1000UL) {
      lastUi = now;
      unsigned long totalSecs = elapsedMillis / 1000UL;
      unsigned long mins = totalSecs / 60UL;
      unsigned long secs = totalSecs % 60UL;
      lcd.setCursor(0, 1);
      lcd.print("Time: " + String(mins) + "m" + String(secs) + "s ");
    }
    // Auto cutoff
    if (chargeLimitMs > 0 && elapsedMillis >= chargeLimitMs) {
      stopCharging("Auto Cutoff");
    }
  }

  // small loop pause to reduce CPU use but keep responsiveness
  delay(20);
}   