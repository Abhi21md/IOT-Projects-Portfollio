// Blynk credentials
#define BLYNK_TEMPLATE_ID "TMPL3Bt3o8L4r"
#define BLYNK_TEMPLATE_NAME "GAS LVL"
#define BLYNK_AUTH_TOKEN "8UvoTXCsazn6AWHTbA4Ipiiskf6NpGKc"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Pin definitions (NodeMCU)
#define MQ2_PIN A0        // Analog input
#define BUZZER_PIN D5     // GPIO14
#define SERVO_PIN D6      // GPIO12

// NodeMCU default I2C pins
#define SDA_PIN D2        // GPIO4
#define SCL_PIN D1        // GPIO5

// WiFi credentials
char ssid[] = "Smart Bin";
char pass[] = "7899262658";

BlynkTimer timer;
Servo regulatorServo;

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Adjustable threshold from Blynk
int gasThreshold = 200;

int lastGasValue = 0;
const int SEND_DIFF = 8;

bool manualClose = false;
int servoPosition = 0;
bool showTitle = true;
bool showingManual = false;
bool lastGasAlert = false;

void displayGasStatus(int gasValue);

void closeRegulator() {
  regulatorServo.write(140);
  servoPosition = 140;
  Serial.println("Regulator CLOSED");
}

void openRegulator() {
  regulatorServo.write(0);
  servoPosition = 0;
  Serial.println("Regulator OPEN");
}

void displayGasStatus(int gasValue) {
  if (showTitle || showingManual) return;

  lcd.setCursor(0, 0);
  lcd.print("Gas:");
  lcd.print(gasValue);
  lcd.print(" ppm   ");

  lcd.setCursor(0, 1);
  lcd.print("TH:");
  lcd.print(gasThreshold);
  lcd.print("  ");
  lcd.print(servoPosition == 0 ? "OPEN " : "CLSD ");
}

void showGasAlert() {
  lcd.setCursor(0, 0);
  lcd.print("Gas Detected!   ");
  lcd.setCursor(0, 1);
  lcd.print("Reg: CLOSED     ");
}

void showManualAction(String msg) {
  showingManual = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(msg);
  lcd.setCursor(0, 1);
  lcd.print("Reg: ");
  lcd.print(servoPosition == 0 ? "OPEN   " : "CLOSED ");
  timer.setTimeout(5000L, []() {
    showingManual = false;
  });
}

// Receive threshold value from Blynk (V4)
BLYNK_WRITE(V4) {
  gasThreshold = param.asInt();
  Serial.print("Updated Gas Threshold: ");
  Serial.println(gasThreshold);

  Blynk.virtualWrite(V1, String("Threshold: ") + gasThreshold);
}

void sendSensorData() {
  if (showTitle || showingManual) return;

  int gasValue = analogRead(MQ2_PIN);
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  bool gasAlert = gasValue > gasThreshold;

  if (gasAlert) {
    digitalWrite(BUZZER_PIN, HIGH);
    if (!manualClose) closeRegulator();
    showGasAlert();
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    if (!manualClose) openRegulator();
    displayGasStatus(gasValue);
  }

  if (gasAlert != lastGasAlert) {
    Blynk.virtualWrite(V2, gasAlert ? 255 : 0);
    Blynk.virtualWrite(V1, gasAlert ? "Gas Detected!" : "Safe");
    Blynk.virtualWrite(V3, gasAlert ? 1 : 0);
    lastGasAlert = gasAlert;
  }

  if (abs(gasValue - lastGasValue) >= SEND_DIFF) {
    Blynk.virtualWrite(V0, gasValue);
    lastGasValue = gasValue;
  }
}

// Manual button (V3)
BLYNK_WRITE(V3) {
  int state = param.asInt();
  manualClose = (state == 1);

  if (manualClose) {
    closeRegulator();
    showManualAction("Manually Closed");
    Blynk.virtualWrite(V1, "Manually Closed");
  } else {
    openRegulator();
    showManualAction("Manually Opened");
    Blynk.virtualWrite(V1, "Manually Opened");
  }
}

void hideTitle() {
  showTitle = false;
  lcd.clear();
}

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  regulatorServo.attach(SERVO_PIN);
  openRegulator();

  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas Monitoring Sys");
  lcd.setCursor(0, 1);
  lcd.print(" Initializing...  ");

  timer.setTimeout(5000L, hideTitle);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(800L, sendSensorData);

  Serial.println("System Initialized...");
}

void loop() {
  Blynk.run();
  timer.run();
}
