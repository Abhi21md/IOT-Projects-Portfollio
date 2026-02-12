#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>
#include <ESP32Servo.h>

// ---------------- WIFI ----------------
char ssid[] = "Smart Bin";
char pass[] = "7899262658";

// ---------------- RFID ----------------
#define SS_PIN 5
#define RST_PIN 4
MFRC522 rfid(SS_PIN, RST_PIN);

// ---------------- HX711 ----------------
#define DOUT 32
#define CLK  33
HX711 scale;
float calibration_factor = -7050.0;

// ---------------- LCD ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- SERVO ----------------
Servo gateServo;
#define SERVO_PIN 25

// ---------------- BUZZER ----------------
#define BUZZER 12

// ---------------- BUTTONS ----------------
#define BTN_100G 27
#define BTN_200G 26
#define BTN_TARE 13

// ---------------- USER DATA ----------------
String uids[3] = {
  "f3096c1a",
  "431e771a",
  "13e27c2f"
};

String names[3] = {
  "Abhi",
  "Ramesh",
  "Anand"
};

int balances[3] = {
  500,
  300,
  150
};

// ---------------- VARIABLES ----------------
int currentUser = -1;
int selectedWeight = 0;
int selectedCost = 0;

// ---------------- PRICES ----------------
int price100 = 10;
int price200 = 18;

// ================= BUZZER =================
void beep(int duration) {
  digitalWrite(BUZZER, HIGH);
  delay(duration);
  digitalWrite(BUZZER, LOW);
}

// ================= SETUP =================
void setup() {

  Serial.begin(115200);

  pinMode(BUZZER, OUTPUT);
  pinMode(BTN_100G, INPUT_PULLUP);
  pinMode(BTN_200G, INPUT_PULLUP);
  pinMode(BTN_TARE, INPUT_PULLUP);

  // WiFi (optional future upgrade)
  WiFi.begin(ssid, pass);

  // RFID
  SPI.begin();
  rfid.PCD_Init();

  // LCD
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  // HX711
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor);
  scale.tare();

  // Servo
  gateServo.attach(SERVO_PIN);
  gateServo.write(0);

  lcd.setCursor(0,0);
  lcd.print("SMART GRAIN ATM");
  lcd.setCursor(0,1);
  lcd.print("Scan Card...");
}

// ================= LOOP =================
void loop() {

  // -------- TARE BUTTON --------
  if (digitalRead(BTN_TARE) == LOW) {
    lcd.clear();
    lcd.print("Taring...");
    scale.tare();
    beep(200);
    delay(1500);
    lcd.clear();
    lcd.print("Scan Card...");
  }

  // -------- RFID SCAN --------
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);
  }

  uid.toLowerCase();
  rfid.PICC_HaltA();

  currentUser = getUserIndex(uid);

  lcd.clear();

  if (currentUser == -1) {
    lcd.print("Invalid Card");
    beep(500);
    delay(2000);
    lcd.clear();
    lcd.print("Scan Card...");
    return;
  }

  lcd.print("Welcome ");
  lcd.print(names[currentUser]);
  lcd.setCursor(0,1);
  lcd.print("Bal: Rs.");
  lcd.print(balances[currentUser]);

  beep(200);
  delay(2000);

  selectQuantity();
}

// ================= FIND USER =================
int getUserIndex(String uid) {
  for (int i = 0; i < 3; i++) {
    if (uids[i] == uid)
      return i;
  }
  return -1;
}

// ================= SELECT QUANTITY =================
void selectQuantity() {

  lcd.clear();
  lcd.print("100g / 200g");

  selectedWeight = 0;

  while (selectedWeight == 0) {

    if (digitalRead(BTN_100G) == LOW) {
      selectedWeight = 100;
      selectedCost = price100;
    }

    if (digitalRead(BTN_200G) == LOW) {
      selectedWeight = 200;
      selectedCost = price200;
    }

    delay(100);
  }

  if (balances[currentUser] < selectedCost) {
    lcd.clear();
    lcd.print("Low Balance");
    beep(600);
    delay(2000);
    lcd.clear();
    lcd.print("Scan Card...");
    return;
  }

  dispenseGrain();
}

// ================= DISPENSE GRAIN =================
void dispenseGrain() {

  lcd.clear();
  lcd.print("Dispensing...");
  scale.tare();

  // Open gate
  gateServo.write(90);
  delay(300);

  float weight = 0;

  while (weight < selectedWeight) {
    weight = scale.get_units(3);

    lcd.setCursor(0,1);
    lcd.print("W:");
    lcd.print((int)weight);
    lcd.print("g   ");
    delay(100);
  }

  // Close gate
  gateServo.write(0);
  beep(200);

  balances[currentUser] -= selectedCost;

  lcd.clear();
  lcd.print("Done: ");
  lcd.print(selectedWeight);
  lcd.print("g");

  lcd.setCursor(0,1);
  lcd.print("Bal: Rs.");
  lcd.print(balances[currentUser]);

  delay(3000);

  lcd.clear();
  lcd.print("Scan Card...");
}
