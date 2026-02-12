#define BLYNK_TEMPLATE_ID "TMPL33CbdJryS" 
#define BLYNK_TEMPLATE_NAME "Smart Dustbin"
#define BLYNK_AUTH_TOKEN "zjd0ubkK1IQOy-jeltTyQtB0heaBd1x3"

#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <Servo.h>
#include <BlynkSimpleEsp8266.h>  // Blynk library

// WiFi Credentials
char auth[] = "zjd0ubkK1IQOy-jeltTyQtB0heaBd1x3";
char ssid[] = "Smart Bin";              
char password[] = "7899262658";  // Declare the password here
LiquidCrystal_PCF8574 lcd(0x27);  // LCD address
Servo servo1;

// Pin assignments
const int trigPin = D8;         // Presence sensor (Ultrasonic)
const int echoPin = D2;
const int potPin = A0;          // Soil Moisture Sensor
const int trigDryPin = D4;      // Dry Waste Ultrasonic
const int echoDryPin = D5;
const int trigWetPin = D6;      // Wet Waste Ultrasonic
const int echoWetPin = D7;

// Variables
long duration;
int distance = 0;
int soil = 0;
int fsoil = 0;
int maxDryValue = 20;
int Ultra_Distance = 15;         // Distance to detect waste
int containerHeight = 16;        // Container height in cm

unsigned long lastWasteHandledTime = 0;
const unsigned long checkLevelInterval = 5000;
bool wasteDetected = false;
bool garbageDetected = false;    // Flag to track garbage detection state

// Blynk Timer
BlynkTimer timer;

void setup() {
  Serial.begin(115200);         // Start serial communication
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigDryPin, OUTPUT);
  pinMode(echoDryPin, INPUT);
  pinMode(trigWetPin, OUTPUT);
  pinMode(echoWetPin, INPUT);
  
  servo1.attach(D0);            // Attach servo to D0

  delay(100);                   // Small delay before starting LCD
  Wire.begin(D1, D3);           // Initialize I2C
  lcd.begin(16, 2);             // Initialize LCD with 16x2 size
  lcd.setBacklight(1);          // Turn on backlight

  // Initial Display
  lcd.setCursor(0, 0);
  lcd.print("SMART DUSTBIN");
  lcd.setCursor(0, 1);
  lcd.print("USE ME");
  delay(3000);                  // Show for 3 seconds
  lcd.clear();                  // Clear the screen after showing the initial message

  // Connect to Wi-Fi and Blynk
  Blynk.begin(auth, ssid, password);  // Use the password variable here

  Serial.println("System Initialized");
}

void loop() {
  // Run Blynk and Timer
  Blynk.run();
  timer.run();

  // Read presence sensor
  distance = readUltrasonicDistance(trigPin, echoPin);

  if (distance < Ultra_Distance && distance > 1 && !garbageDetected) {
    wasteDetected = true;
    garbageDetected = true;  // Set the flag to true when garbage is detected
    handleWasteSegregation();
    lastWasteHandledTime = millis();  // Reset last handled time
  }

  // When no waste is detected for 5 seconds
  if (!wasteDetected && millis() - lastWasteHandledTime >= checkLevelInterval) {
    displayWasteLevel();
    delay(2000);                 // Show levels for 2 seconds
    lastWasteHandledTime = millis();
  } else if (!wasteDetected && !garbageDetected) {
    // Show segregation message continuously when no waste is detected
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dry Wet Waste");
    lcd.setCursor(0, 1);
    lcd.print("Segregator");
  }

  // If garbage is detected, show the message for 2 seconds and then return to idle
  if (garbageDetected) {
    delay(2000); // Display for 2 seconds
    garbageDetected = false;  // Reset the garbage detection flag
    lcd.clear();  // Clear the screen before going back to idle state
    wasteDetected = false;    // Reset waste detection flag
  }

  delay(1000);
}

void handleWasteSegregation() {
  lcd.clear();                   // Clear the LCD
  delay(100);                    // Avoid flickering

  // Read soil moisture
  fsoil = 0;
  for (int i = 0; i < 3; i++) {
    soil = analogRead(potPin);
    soil = constrain(soil, 200, 1023);  // Adjust for NodeMCU's 0–1V range
    fsoil += map(soil, 200, 1023, 100, 0);
    delay(75);
  }
  fsoil /= 3;

  if (fsoil > maxDryValue) {
    // Wet waste detected
    lcd.setCursor(0, 0);
    lcd.print("Garbage Detected");
    lcd.setCursor(0, 1);
    lcd.print("WET");
    Serial.println("WET Waste Detected");
    servo1.write(180);           // Open wet waste compartment
    Blynk.virtualWrite(V4, "WET");  // Send waste type to Blynk
  } else {
    // Dry waste detected
    lcd.setCursor(0, 0);
    lcd.print("Garbage Detected");
    lcd.setCursor(0, 1);
    lcd.print("DRY");
    Serial.println("DRY Waste Detected");
    servo1.write(0);             // Open dry waste compartment
    Blynk.virtualWrite(V4, "DRY");  // Send waste type to Blynk
  }

  delay(1500);
  servo1.write(90);              // Reset servo to neutral position

  // Important Update:
  // After resetting the servo to neutral, immediately return to the idle state and show the segregation message again
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dry Wet Waste");
  lcd.setCursor(0, 1);
  lcd.print("Segregator");

  // No delay after servo neutral, this ensures the LCD goes back to the cycle immediately
}

int readUltrasonicDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  int distance = duration * 0.034 / 2;
  return distance;
}

void displayWasteLevel() {
  int dryDistance = readUltrasonicDistance(trigDryPin, echoDryPin);
  int wetDistance = readUltrasonicDistance(trigWetPin, echoWetPin);

  int dryLevel = mapToLevel(dryDistance, containerHeight);
  int wetLevel = mapToLevel(wetDistance, containerHeight);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dry: ");
  lcd.print(dryLevel);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Wet: ");
  lcd.print(wetLevel);
  lcd.print("%");

  Serial.print("Dry Level: ");
  Serial.print(dryLevel);
  Serial.print("%, Wet Level: ");
  Serial.print(wetLevel);
  Serial.println("%");

  // Send waste levels to Blynk
  Blynk.virtualWrite(V1, dryLevel);  // Dry level to Blynk
  Blynk.virtualWrite(V2, wetLevel);  // Wet level to Blynk

  // If level is 75% or above, trigger alert
  if (dryLevel >= 75 || wetLevel >= 75) {
    Blynk.virtualWrite(V3, "ALERT: FULL");  // Full bin alert
    // Send Blynk notification and email for high level
    Blynk.logEvent("fullbin", "Warning: Bin is at 75% or more capacity!");  // Event for email and notification
  } else {
    Blynk.virtualWrite(V3, "");  // Clear alert if below 75%
  }
}

int mapToLevel(int distance, int containerHeight) {
  if (distance <= containerHeight * 0.25) {
    return 100;  // 0 to 4 cm
  } else if (distance <= containerHeight * 0.5) {
    return 75;   // 4 to 8 cm
  } else if (distance <= containerHeight * 0.75) {
    return 50;   // 8 to 12 cm
  } else if (distance <= containerHeight) {
    return 25;   // 12 to 16 cm
  } else {
    return 0;    // Beyond 16 cm
  }
}
