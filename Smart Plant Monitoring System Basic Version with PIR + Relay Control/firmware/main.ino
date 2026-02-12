// Include the necessary libraries
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h> // LCD library
#define BLYNK_TEMPLATE_ID "TMPL3ZcrN-kwX"
#define BLYNK_TEMPLATE_NAME "Smart Plant Monitoring"
#define BLYNK_AUTH_TOKEN "MvQZGZDUonHKRtxDh2utL4zBa9g6PdXl"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// LCD I2C address
LiquidCrystal_PCF8574 lcd(0x27);

char auth[] = "MvQZGZDUonHKRtxDh2utL4zBa9g6PdXl";
char ssid[] = "Smart Bin";              
char pass[] = "7899262658";             
DHT dht(D4, DHT11); // DHT sensor pin and type
BlynkTimer timer;

// Pin Definitions
#define soil A0        // Soil Moisture Sensor
#define PIR D5         // PIR Motion Sensor
#define RELAY_PIN_1 D3 // Relay
#define PUSH_BUTTON_1 D7 // Button

// Virtual Pins
#define VPIN_TEMP V10     // Virtual pin for temperature
#define VPIN_HUMIDITY V11 // Virtual pin for humidity
#define VPIN_SOIL V12     // Virtual pin for soil moisture
#define VPIN_PIR_LED V13  // Virtual pin for motion detection LED
#define VPIN_RELAY V14    // Virtual pin for relay control
#define VPIN_PIR_TOGGLE V15 // Virtual pin for PIR enable/disable

int PIR_ToggleValue = 0;
int relay1State = LOW;
int pushButton1State = HIGH;

// Previous values to detect significant changes
float prevTemp = -1.0;
float prevHumidity = -1.0;
int prevSoilMoisture = -1;
bool prevMotionState = false;

void setup() {
  Serial.begin(9600);

  // Initialize LCD
  lcd.begin(16, 2); 
  lcd.setBacklight(255);
  lcd.setCursor(0, 0);
  lcd.print("  Initializing  ");
  Serial.println("Initializing System...");

  for (int a = 5; a <= 10; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(500);
  }

  lcd.clear();
  lcd.setCursor(11, 1);
  lcd.print("W:OFF");
  Serial.println("Initialization Complete.");

  // Pin Modes
  pinMode(PIR, INPUT);
  pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, LOW);
  pinMode(PUSH_BUTTON_1, INPUT_PULLUP);

  // Blynk and sensor initialization
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();

  // Timer functions (poll sensors less frequently to avoid exceeding limits)
  timer.setInterval(10000L, soilMoistureSensor); // Read soil moisture every 10 seconds
  timer.setInterval(15000L, DHT11sensor);        // Read temperature and humidity every 15 seconds
  timer.setInterval(2000L, checkPhysicalButton); // Check button state every 2 seconds
}

// DHT11 sensor readings
void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Update only if values have changed significantly
  if (abs(t - prevTemp) >= 0.5 || abs(h - prevHumidity) >= 1) {
    prevTemp = t;
    prevHumidity = h;

    Blynk.virtualWrite(VPIN_TEMP, t);  
    Blynk.virtualWrite(VPIN_HUMIDITY, h);
    Serial.print("Blynk Update: Temperature sent to V10: ");
    Serial.println(t);
    Serial.print("Blynk Update: Humidity sent to V11: ");
    Serial.println(h);

    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(t, 1);
    lcd.print("C");

    lcd.setCursor(8, 0);
    lcd.print("H:");
    lcd.print(h, 1);
    lcd.print("%");
  }
}

// Soil moisture readings
void soilMoistureSensor() {
  int value = analogRead(soil);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  if (value != prevSoilMoisture) {
    prevSoilMoisture = value;

    Blynk.virtualWrite(VPIN_SOIL, value);
    Serial.print("Blynk Update: Soil Moisture sent to V12: ");
    Serial.println(value);

    lcd.setCursor(0, 1);
    lcd.print("S:");
    lcd.print(value);
    lcd.print("%   ");
  }
}

// PIR motion detection
void PIRsensor() {
  bool motionDetected = digitalRead(PIR);
  WidgetLED LED(VPIN_PIR_LED);

  if (motionDetected != prevMotionState) {
    prevMotionState = motionDetected;

    if (motionDetected) {
      Blynk.logEvent("pirmotion", "WARNING! Motion Detected!");
      LED.on();
      Serial.println("Blynk Update: Motion detected, LED on V13 turned ON.");
    } else {
      LED.off();
      Serial.println("Blynk Update: No motion detected, LED on V13 turned OFF.");
    }
  }
}

// Relay control via physical button
void checkPhysicalButton() {
  if (digitalRead(PUSH_BUTTON_1) == LOW) {
    if (pushButton1State != LOW) {
      relay1State = !relay1State;
      digitalWrite(RELAY_PIN_1, relay1State);
      Blynk.virtualWrite(VPIN_RELAY, relay1State);
      Serial.print("Blynk Update: Relay state changed to ");
      Serial.println(relay1State == HIGH ? "ON" : "OFF");
    }
    pushButton1State = LOW;
  } else {
    pushButton1State = HIGH;
  }
}

// Blynk write handlers
BLYNK_WRITE(VPIN_RELAY) {
  relay1State = param.asInt();
  digitalWrite(RELAY_PIN_1, relay1State);
  Serial.print("Blynk Update: Relay control received, state set to ");
  Serial.println(relay1State == HIGH ? "ON" : "OFF");
}

BLYNK_WRITE(VPIN_PIR_TOGGLE) {
  PIR_ToggleValue = param.asInt();
  Serial.print("Blynk Update: PIR toggle state received, set to ");
  Serial.println(PIR_ToggleValue == 1 ? "ENABLED" : "DISABLED");
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(VPIN_RELAY);
  Blynk.syncVirtual(VPIN_PIR_TOGGLE);
  Serial.println("Blynk: Synced virtual pins.");
}

void loop() {
  // PIR motion toggle check
  if (PIR_ToggleValue == 1) {
    lcd.setCursor(5, 1);
    lcd.print("M:ON ");
    PIRsensor();
  } else {
    lcd.setCursor(5, 1);
    lcd.print("M:OFF");
    WidgetLED LED(VPIN_PIR_LED);
    LED.off();
  }

  // Relay state on LCD
  if (relay1State == HIGH) {
    lcd.setCursor(11, 1);
    lcd.print("W:ON ");
  } else {
    lcd.setCursor(11, 1);
    lcd.print("W:OFF");
  }

  Blynk.run();
  timer.run();
}