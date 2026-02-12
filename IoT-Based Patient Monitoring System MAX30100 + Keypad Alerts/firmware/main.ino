// Undefine HIGH macro to prevent conflict
#undef HIGH

#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h> 
#include "MAX30100_PulseOximeter.h"

#define BLYNK_TEMPLATE_ID "TMPL3cyggP7iS"
#define BLYNK_TEMPLATE_NAME "Patient Monitoring System"
#define BLYNK_AUTH_TOKEN "mADseSupO75k_RAIdjBZr6dTcc8FPp37"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Blynk Credentials
char auth[] = "mADseSupO75k_RAIdjBZr6dTcc8FPp37"; 
char ssid[] = "Smart Bin";  
char pass[] = "7899262658";  

// LCD Initialization (I2C) and MAX30100 on D1 (SCL) & D2 (SDA)
LiquidCrystal_PCF8574 lcd(0x27);
PulseOximeter pox;

// Keypad Configuration
const byte ROW_NUM = 4;
const byte COLUMN_NUM = 4;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pin_rows[ROW_NUM] = {D3, D4, D5, D6};
byte pin_column[COLUMN_NUM] = {D7, D8, D0, A0};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

// Predefined Messages
const char* messages[] = {
  "I want water",       
  "I need medicine",    
  "I am in pain",       
  "Help me!",           
  "I feel sick",        
  "I need assistance"  
};

// Buzzer Pin
int buzzerPin = D0;  

// Heart Rate Data Reporting Interval
uint32_t tsLastReport = 0;

void onBeatDetected() {
  Serial.println("Heart Beat Detected!");
}

void setup() {
  Serial.begin(9600);
  Wire.begin(D2, D1);  // SDA=D2, SCL=D1 (Shared for MAX30100 and LCD)

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.setBacklight(255);  
  lcd.setCursor(0, 0);
  lcd.print("Patient Monitoring");
  lcd.setCursor(0, 1);
  lcd.print("System");

  // Initialize Buzzer
  pinMode(buzzerPin, OUTPUT);

  // Connect to Wi-Fi and Blynk
  Blynk.begin(auth, ssid, pass);
  Serial.println("Patient Monitoring System Ready");

  // Initialize MAX30100
  if (!pox.begin()) {
    Serial.println("MAX30100 Init FAILED");
    while (true);  
  } else {
    Serial.println("MAX30100 Init SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  Blynk.run();  

  // Update Heart Rate and SpO2 every second
  pox.update();
  if (millis() - tsLastReport > 1000) {
    float bpm = pox.getHeartRate();
    float spO2 = pox.getSpO2();

    // Always display readings on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BPM: ");
    lcd.print(bpm);
    lcd.setCursor(0, 1);
    lcd.print("SpO2: ");
    lcd.print(spO2);
    lcd.print("%");

    // Send values to Blynk, but make sure to handle invalid readings
    if (bpm > 0 && spO2 > 0) {
      // Send valid readings to Blynk
      Blynk.virtualWrite(V1, bpm);  // Virtual pin V1 for BPM
      Blynk.virtualWrite(V2, spO2);  // Virtual pin V2 for SpO2
    } else {
      // Send 0 if the values are invalid (finger removed)
      Blynk.virtualWrite(V1, 0);  // Reset BPM to 0
      Blynk.virtualWrite(V2, 0);  // Reset SpO2 to 0
    }

    tsLastReport = millis();
  }

  char key = keypad.getKey();
  if (key) {
    lcd.clear();

    if (key >= '1' && key <= '6') {  
      int msgIndex = key - '1';  
      lcd.setCursor(0, 0);
      lcd.print("Message Sent:");
      lcd.setCursor(0, 1);
      lcd.print(messages[msgIndex]);
      tone(buzzerPin, 1000, 200);  

      // Send Message to Blynk App
      Blynk.virtualWrite(V0, messages[msgIndex]);
      Serial.println(messages[msgIndex]);
    } else if (key == '*') {  // Emergency
      lcd.setCursor(0, 0);
      lcd.print("Emergency Alert");
      lcd.setCursor(0, 1);
      lcd.print("Call for Help");
      tone(buzzerPin, 3000, 1000);  

      // Send Emergency Alert to Blynk
      Blynk.logEvent("notification", "Emergency Alert: Call for Help");
      Serial.println("Emergency Alert Sent!");
    } else {  // Invalid Key
      lcd.setCursor(0, 0);
      lcd.print("Invalid Key");
      tone(buzzerPin, 500, 100);  
    }

    delay(2000);  
    lcd.clear();

    // Display "Patient Monitoring System" after key event
    lcd.setCursor(0, 0);
    lcd.print("Patient Monitoring");
    lcd.setCursor(0, 1);
    lcd.print("System");
  }
}