/*************** WATER FLOW + SOIL MOISTURE + AUTO PUMP + MANUAL OVERRIDE + ACTIVE LOW RELAY + BLYNK ***************
   Final Stable & Accurate Version (with Auto-Unblock, Calibration Slider, Hysteresis & Filtering)
*****************************************************************************************************/

#define BLYNK_TEMPLATE_NAME "DRAINAGE"
#define BLYNK_TEMPLATE_ID "TMPL3vJpPM1UI"
#define BLYNK_AUTH_TOKEN "y593CdjN48aR2v6TpPxoB0daE9CYgok9"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// ---------------- PIN CONFIGURATION ----------------
#define FLOWSENSOR_PIN D2
#define BUZZER_PIN     D5
#define RELAY_PIN      D6
#define PUMP_PIN       D1
#define SOIL_PIN       A0

// ---------------- WIFI CREDENTIALS ----------------
char ssid[] = "Smart Bin";
char pass[] = "7899262658";

// ---------------- FLOW VARIABLES ----------------
volatile int pulseCount;
float calibrationFactor = 7.5; // Default, adjustable via Blynk slider (V5)
float flowRate = 0, filteredFlow = 0, prevFlow = 0;
unsigned long flowMilliLitres = 0, totalMilliLitres = 0, oldTime = 0;

// Flow thresholds with hysteresis
const float blockageThresholdLow  = 0.12; // Flow below this = blockage
const float blockageThresholdHigh = 0.50; // Flow above this = clear

bool blockageActive = false; // Tracks blockage state

// ---------------- SOIL VARIABLES ----------------
int soilValue = 0;
int lastSoil = 0;
const int drySafeLevel = 1000;   // Below = overflow
const int autoPumpMoisture = 900;

// ---------------- PUMP VARIABLES ----------------
int pumpManual = 0;
bool pumpRunning = false;
unsigned long pumpStartTime = 0;
const int autoPumpRunTime = 15000;
const int autoPumpCooldown = 30000;
unsigned long lastPumpEnd = 0;

// ---------------- ALERT VARIABLES ----------------
unsigned long lastAlertTime = 0;
const unsigned long alertCooldown = 60000; // 1 min gap

// ---------------- BLYNK HANDLERS ----------------
BLYNK_WRITE(V3) { // Manual Pump Control
  pumpManual = param.asInt();
  if (pumpManual == 1) {
    digitalWrite(PUMP_PIN, LOW);  // Pump ON (Active LOW)
    pumpRunning = true;
    Serial.println("Manual Pump: ON (Override)");
  } else {
    digitalWrite(PUMP_PIN, HIGH); // Pump OFF
    pumpRunning = false;
    Serial.println("Manual Pump: OFF (Auto Mode Active)");
  }
}

// Live Calibration Adjustment via Slider
BLYNK_WRITE(V5) {
  calibrationFactor = param.asFloat();
  Serial.print("New Calibration Factor: ");
  Serial.println(calibrationFactor);
}

void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(9600);

  pinMode(FLOWSENSOR_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(PUMP_PIN, HIGH); // Pump OFF initially (Active LOW)

  attachInterrupt(digitalPinToInterrupt(FLOWSENSOR_PIN), pulseCounter, FALLING);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("System Online (Final Optimized Version)");
}

// ---------------- MAIN LOOP ----------------
void loop() {
  Blynk.run();
  unsigned long now = millis();

  // --- Smoothed Soil Reading ---
  int currentSoil = analogRead(SOIL_PIN);
  soilValue = (currentSoil + lastSoil) / 2; // Average filter
  lastSoil = soilValue;
  Blynk.virtualWrite(V2, soilValue);

  // --- Flow Calculation (1 second interval) ---
  if (now - oldTime > 1000) {
    detachInterrupt(digitalPinToInterrupt(FLOWSENSOR_PIN));

    flowRate = ((1000.0 / (now - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = now;
    pulseCount = 0;

    // Weighted smoothing for noise reduction
    filteredFlow = (flowRate * 0.25f) + (prevFlow * 0.75f);
    prevFlow = filteredFlow;

    flowMilliLitres = (filteredFlow / 60.0f) * 1000.0f;
    totalMilliLitres += flowMilliLitres;

    Blynk.virtualWrite(V0, filteredFlow);
    Blynk.virtualWrite(V1, totalMilliLitres / 1000.0f);

    Serial.print("Flow: "); Serial.print(filteredFlow, 2);
    Serial.print(" L/min | Soil: "); Serial.println(soilValue);

    bool overflow = (soilValue < drySafeLevel);

    // --- BLOCKAGE DETECTION & AUTO UNBLOCK ---
    if (!blockageActive && filteredFlow < blockageThresholdLow) {
      blockageActive = true;
      Blynk.logEvent("blockage_alert", "Pipe Blockage Detected - Pump Activated");
      Serial.println("⚠ Blockage Detected!");

      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(RELAY_PIN, HIGH);
      if (pumpManual == 0) digitalWrite(PUMP_PIN, LOW); // Pump ON
    }

    else if (blockageActive && filteredFlow > blockageThresholdHigh) {
      blockageActive = false;
      Blynk.logEvent("blockage_clear", "Flow Restored - System Normal");
      Serial.println("✅ Blockage Cleared!");

      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(RELAY_PIN, LOW);
      if (pumpManual == 0) digitalWrite(PUMP_PIN, HIGH); // Pump OFF
    }

    // --- OVERFLOW DETECTION ---
    if (overflow) {
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(RELAY_PIN, HIGH);
      if (pumpManual == 0) digitalWrite(PUMP_PIN, LOW); // Pump ON
      if (now - lastAlertTime > alertCooldown) {
        Blynk.logEvent("overflow_alert", "Drain Overflow Detected - Pump Activated");
        lastAlertTime = now;
      }
    } else if (!blockageActive && !overflow) {
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(RELAY_PIN, LOW);
      if (pumpManual == 0) digitalWrite(PUMP_PIN, HIGH); // Pump OFF
    }

    // --- AUTO PUMP LOGIC ---
    if (pumpManual == 0) {
      if (!pumpRunning && soilValue < autoPumpMoisture && (now - lastPumpEnd) > autoPumpCooldown) {
        digitalWrite(PUMP_PIN, LOW);   // Pump ON
        pumpRunning = true;
        pumpStartTime = now;
        Serial.println("Auto Pump: ON (Low Moisture)");
        delay(300); // Stabilize relay
      }

      if (pumpRunning && (now - pumpStartTime >= autoPumpRunTime)) {
        digitalWrite(PUMP_PIN, HIGH);  // Pump OFF
        pumpRunning = false;
        lastPumpEnd = now;
        Serial.println("Auto Pump: OFF (Cycle Complete)");
      }
    }
 
    attachInterrupt(digitalPinToInterrupt(FLOWSENSOR_PIN), pulseCounter, FALLING);
  }
}