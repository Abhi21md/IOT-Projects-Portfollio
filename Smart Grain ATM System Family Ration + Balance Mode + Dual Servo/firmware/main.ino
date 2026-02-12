#define BLYNK_TEMPLATE_ID "TMPL3vXLsbmhN"
#define BLYNK_TEMPLATE_NAME "GRAIN ATM"
#define BLYNK_AUTH_TOKEN "A0YQgWzvFJFkACwjRmBVtW7GyaLnC3Iz"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>
#include <ESP32Servo.h>

// WiFi credentials
char ssid[] = "Smart Bin";
char pass[] = "7899262658";

// RFID
#define SS_PIN 5
#define RST_PIN 4
MFRC522 rfid(SS_PIN, RST_PIN);

// HX711
#define DOUT 32
#define CLK  33
HX711 scale;
float calibration_factor = -886.75;

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servos
Servo riceServo;
Servo jowarServo;
#define RICE_SERVO_PIN 25
#define JOWAR_SERVO_PIN 14

// Buzzer
#define BUZZER_PIN 12

// Buttons
#define BTN_200g 27
#define BTN_250g 26

// RFID Users
String uids[5] = {
"23e3591a", // Abhi -> now valid
"431e771a", // Ajit -> Balance mode
"13e27c2f", // Anand -> Fam1
"5330e8f6", // Vishal -> Fam2
"f3096c1a"  // Invalid
};
String names[5] = {"Invalid", "Ajit", "Anand", "Vishal", "Abhi"};
int balances[5] = {0, 700, 0, 0, 0}; // Abhi balance = 0, Ajit = 700

int currentUserIndex = -1;
int totalCollected = 0;
float totalGrainDispensed = 0;

int selectedQty = 0;
int selectedCost = 0;

// only 200g & 250g left
int weights[2] = {200, 250};
int costs[2] = {15, 20};

// Family data
int famMembers[2] = {3, 2}; // Fam1=3 members, Fam2=2 members
int ricePerHead = 100;      // 100g rice per head
int jowarPerHead = 50;      // 50g jowar per head

// Daily ration tracking
bool rationTaken[2] = {false, false};  // Fam1, Fam2
unsigned long lastResetTime = 0;
const unsigned long oneDay = 86400000UL; // 24 hours in ms

// -------------------- Helper --------------------
void beep(int duration) {
digitalWrite(BUZZER_PIN, HIGH);
delay(duration);
digitalWrite(BUZZER_PIN, LOW);
}

bool waitForButton(int pin) {
while (true) {
if (digitalRead(pin) == LOW) {
delay(50); // debounce
while (digitalRead(pin) == LOW);
return true;
}
Blynk.run();
}
}

// Countdown Timer Display
void countdownTimer(int seconds, String message) {
for (int i = seconds; i > 0; i--) {
lcd.clear();
lcd.setCursor(0, 0);
lcd.print(message);
lcd.setCursor(0, 1);
lcd.print("Starting in: ");
lcd.print(i);
lcd.print("s");
delay(1000);
}
}

// -------------------- Setup --------------------
void setup() {
Serial.begin(115200);
pinMode(BUZZER_PIN, OUTPUT);

WiFi.begin(ssid, pass);
Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

SPI.begin();
rfid.PCD_Init();

Wire.begin(21, 22);
lcd.begin(16, 2);
lcd.setBacklight(1);
lcd.setCursor(0, 0);
lcd.print("Smart Grain ATM");
lcd.setCursor(0, 1);
lcd.print("Scan RFID Card");
Serial.println("System Ready - Scan RFID Card");

scale.begin(DOUT, CLK);
scale.set_scale(calibration_factor);
scale.tare();

riceServo.attach(RICE_SERVO_PIN);
riceServo.write(0);
jowarServo.attach(JOWAR_SERVO_PIN);
jowarServo.write(0);

pinMode(BTN_200g, INPUT_PULLUP);
pinMode(BTN_250g, INPUT_PULLUP);

updateBlynkGauges();
Blynk.virtualWrite(V5, (int)totalGrainDispensed);
}

// -------------------- Main Loop --------------------
void loop() {
Blynk.run();

// Reset ration every 24 hours
if (millis() - lastResetTime > oneDay) {
rationTaken[0] = false;
rationTaken[1] = false;
lastResetTime = millis();
Serial.println("Daily ration reset");
updateBlynkGauges();
}

if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

String uid = "";
for (byte i = 0; i < rfid.uid.size; i++) {
uid += String(rfid.uid.uidByte[i], HEX);
}
uid.toLowerCase();
rfid.PICC_HaltA();

currentUserIndex = getUserIndex(uid);
lcd.clear();

// Invalid Cards (only last index 4 is now invalid)
if (currentUserIndex == -1 || currentUserIndex == 4) {
beep(500);
lcd.setCursor(0, 0);
lcd.print("Invalid Card");
delay(2000);
lcd.clear();
lcd.print("Scan RFID Card");
return;
}

// Ajit -> Balance Mode
if (currentUserIndex == 1) {
beep(100);
lcd.setCursor(0, 0);
lcd.print("Welcome Ajit");
lcd.setCursor(0, 1);
lcd.print("Balance: ₹");
lcd.print(balances[1]);
delay(2000);
selectQuantityAndGrain();
return;
}

// Fam1 or Fam2
if (currentUserIndex == 2) {  // Anand -> Fam1
if (rationTaken[0]) {
lcd.setCursor(0, 0);
lcd.print("Fam1 Already");
lcd.setCursor(0, 1);
lcd.print("Taken Today");
delay(3000);
lcd.clear();
lcd.print("Scan RFID Card");
return;
}
dispenseFamilyRation(1, famMembers[0]);
rationTaken[0] = true;
updateBlynkGauges();
} else if (currentUserIndex == 3) { // Vishal -> Fam2
if (rationTaken[1]) {
lcd.setCursor(0, 0);
lcd.print("Fam2 Already");
lcd.setCursor(0, 1);
lcd.print("Taken Today");
delay(3000);
lcd.clear();
lcd.print("Scan RFID Card");
return;
}
dispenseFamilyRation(2, famMembers[1]);
rationTaken[1] = true;
updateBlynkGauges();
} else if (currentUserIndex == 0) { // Abhi -> valid now
beep(100);
lcd.setCursor(0, 0);
lcd.print("Welcome Abhi");
lcd.setCursor(0, 1);
lcd.print("No Balance");
delay(2000);
lcd.clear();
lcd.print("Scan RFID Card");
return;
}
}

// -------------------- User Index --------------------
int getUserIndex(String uid) {
for (int i = 0; i < 5; i++) {
if (uids[i] == uid) return i;
}
return -1;
}

// -------------------- Balance Flow -----------------
void selectQuantityAndGrain() {
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Select Qty:");
lcd.setCursor(0, 1);
lcd.print("200g 250g");

selectedQty = 0;
selectedCost = 0;

// Step 1: Select quantity
while (selectedQty == 0) {
if (digitalRead(BTN_200g) == LOW) {
waitForButton(BTN_200g);
selectedQty = weights[0];
selectedCost = costs[0];
} else if (digitalRead(BTN_250g) == LOW) {
waitForButton(BTN_250g);
selectedQty = weights[1];
selectedCost = costs[1];
}
delay(50);
}

if (balances[currentUserIndex] < selectedCost) {
beep(500);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Insufficient Bal");
delay(2000);
lcd.clear();
lcd.print("Scan RFID Card");
return;
}

// Step 2: Select grain using same buttons
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Select Grain:");
lcd.setCursor(0, 1);
lcd.print("Rice=200  Jowar=250");

Servo* chosenServo = nullptr;
while (chosenServo == nullptr) {
if (digitalRead(BTN_200g) == LOW) { // Rice
waitForButton(BTN_200g);
chosenServo = &riceServo;
} else if (digitalRead(BTN_250g) == LOW) { // Jowar
waitForButton(BTN_250g);
chosenServo = &jowarServo;
}
delay(50);
}

dispenseGrain(selectedQty, selectedCost, *chosenServo);
}

void dispenseGrain(int qty, int cost, Servo &servoMotor) {
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Dispensing...");
scale.tare();

servoMotor.write(20); // open
unsigned long startTime = millis();
float weight = 0;

while (weight < qty) {
weight = scale.get_units(3);
lcd.setCursor(0, 1);
lcd.print("Weight: ");
lcd.print((int)weight);
lcd.print("g  ");

if (millis() - startTime > 15000) {      
  lcd.clear();      
  lcd.print("Timeout Error");      
  servoMotor.write(0);      
  delay(2000);      
  return;      
}      
delay(100);

}

servoMotor.write(0); // close
beep(100);

float dispensed = weight;
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Done: ");
lcd.print((int)dispensed);
lcd.print("g");

balances[currentUserIndex] -= cost;
totalCollected += cost;
totalGrainDispensed += dispensed;

updateBlynkGauges();
Blynk.virtualWrite(V5, (int)totalGrainDispensed);

delay(3000);
lcd.clear();
lcd.print("Scan RFID Card");
}

// -------------------- Family Flow -----------------
void dispenseFamilyRation(int famNo, int members) {
int riceQty = members * ricePerHead;
int jowarQty = members * jowarPerHead;

lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Fam");
lcd.print(famNo);
lcd.print(" Members:");
lcd.print(members);
lcd.setCursor(0, 1);
lcd.print("R:");
lcd.print(riceQty);
lcd.print(" J:");
lcd.print(jowarQty);
delay(2000);

// Countdown before Rice
countdownTimer(5, "Collect Rice");

// Dispense Rice
dispenseGrain(riceQty, 0, riceServo);

// Countdown before Jowar
countdownTimer(5, "Collect Jowar");

// Dispense Jowar
dispenseGrain(jowarQty, 0, jowarServo);
}

// -------------------- Blynk Gauges -----------------
void updateBlynkGauges() {
Blynk.virtualWrite(V0, balances[0]);   // Abhi
Blynk.virtualWrite(V1, balances[1]);   // Ajit
Blynk.virtualWrite(V2, rationTaken[0] ? 1 : 0); // Fam1
Blynk.virtualWrite(V3, rationTaken[1] ? 1 : 0); // Fam2
Blynk.virtualWrite(V4, totalCollected);
} 