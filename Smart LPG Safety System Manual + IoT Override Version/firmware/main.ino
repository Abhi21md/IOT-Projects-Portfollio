// Blynk credentials
#define BLYNK_TEMPLATE_ID "TMPL3x6cbS8-_"
#define BLYNK_TEMPLATE_NAME "GAS LVL"
#define BLYNK_AUTH_TOKEN "KB0O_cW0UF3cQGtxvfheYnFJRr0HpXTL"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin definitions
#define MQ2_PIN A0        // Analog pin connected to MQ2 sensor
#define BUZZER_PIN D1     // Buzzer pin
#define SERVO_PIN D2      // Servo connected to gas regulator

// Reassign I2C pins for LCD
#define SDA_PIN D5
#define SCL_PIN D6

// WiFi credentials
char ssid[] = "Smart Bin";
char pass[] = "7899262658";

BlynkTimer timer;
Servo regulatorServo;

// Initialize LCD with I2C address 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int GAS_THRESHOLD = 200;  // Adjust as per calibration
int lastGasValue = 0;           // For change detection
const int SEND_DIFF = 3;        // Minimum difference to send update

bool manualClose = false;       // Manual control flag
int servoPosition = 0;          // Track current servo position
bool showTitle = true;          // Flag to show initial title
bool gasAlertActive = false;    // Flag for gas alert display
bool showingManual = false;     // Flag for manual display

// Forward declaration
void displayGasStatus(int gasValue);

// Function to close the gas regulator
void closeRegulator() {
  regulatorServo.write(140); // Closed position
  servoPosition = 140;
  Serial.println("Regulator CLOSED (140°)");
}

// Function to open the gas regulator
void openRegulator() {
  regulatorServo.write(0); // Open position
  servoPosition = 0;
  Serial.println("Regulator OPEN (0°)");
}

// Function to display gas status on LCD
void displayGasStatus(int gasValue) {
  if (showTitle || showingManual) return; // Skip display during title or manual messages

  lcd.setCursor(0, 0);
  lcd.print("Gas:");
  lcd.print(gasValue);
  lcd.print(" ppm   ");

  lcd.setCursor(0, 1);
  lcd.print("Reg: ");
  lcd.print(servoPosition == 0 ? "OPEN   " : "CLOSED ");
}

// Function to show gas alert on LCD
void showGasAlert() {
  lcd.setCursor(0, 0);
  lcd.print("Gas Detected!   ");
  lcd.setCursor(0, 1);
  lcd.print("Reg: CLOSED     ");
}

// Function to show manual action for 5 seconds
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

// Function to send gas sensor data to Blynk and LCD
void sendSensorData() {
  if (showTitle || showingManual) return; // Skip readings during title/manual display

  int gasValue = analogRead(MQ2_PIN);
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  // Gas alert handling
  if (gasValue > GAS_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);
    Blynk.virtualWrite(V2, 255);  // Red LED ON

    if (!manualClose) {
      closeRegulator();
      Blynk.virtualWrite(V3, 1);
    }

    showGasAlert();
    gasAlertActive = true;
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    Blynk.virtualWrite(V2, 0);  // Red LED OFF

    if (!manualClose) {
      openRegulator();
      Blynk.virtualWrite(V3, 0);
    }

    gasAlertActive = false;
    displayGasStatus(gasValue);
  }

  // Send data to Blynk (only if significant change)
  if (abs(gasValue - lastGasValue) >= SEND_DIFF) {
    Blynk.virtualWrite(V0, gasValue);
    Blynk.virtualWrite(V1, gasValue > GAS_THRESHOLD ? "Gas Detected!" : "Safe");
    lastGasValue = gasValue;
  }
}

// Manual control via Blynk (V3)
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

// Function to stop showing title after 5 seconds
void hideTitle() {
  showTitle = false;
  lcd.clear();
}

void setup() {
  Serial.begin(9600);

  pinMode(MQ2_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  regulatorServo.attach(SERVO_PIN);
  openRegulator();

  // Initialize I2C on custom pins
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas Monitoring Sys");
  lcd.setCursor(0, 1);
  lcd.print(" Initializing...  ");

  // Show title for 5 seconds
  timer.setTimeout(5000L, hideTitle);

  // Connect WiFi + Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(500L, sendSensorData);
  Serial.println("System Initialized...");
}

void loop() {
  Blynk.run();
  timer.run();
} 