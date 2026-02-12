#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// RFID Setup
#define RST_PIN 9
#define SS_PIN 10
MFRC522 rfid(SS_PIN, RST_PIN);

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change to 0x3F if your LCD uses that address

// Servo and other components
Servo lockServo;
const int servoPin = 3;
const int buzzerPin = 4;
const int greenLED = 5;
const int redLED = 6;

// List of authorized UIDs
byte authorizedUIDs[][4] = {
  {0x63, 0xB8, 0x64, 0xF7}   // Card 2
};

const int numberOfCards = sizeof(authorizedUIDs) / sizeof(authorizedUIDs[0]);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  lcd.begin(16, 2);             
  lcd.setBacklight(1);          
  
  // Show project name at startup
  lcd.setCursor(0, 0);
  lcd.print("RFID Door Lock");
  lcd.setCursor(0, 1);
  lcd.print("System Initializing");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place your card");

  lockServo.attach(servoPin);
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  lockServo.write(0); // Locked position
  Serial.println("Place your RFID card...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  // Buzzer for every scan attempt
  tone(buzzerPin, 800, 100);
  delay(150);

  Serial.print("UID tag : ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  if (isAuthorized(rfid.uid.uidByte)) {
    Serial.println("Access Granted");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Granted");
    lcd.setCursor(0, 1);
    lcd.print("Door Unlocked");

    digitalWrite(greenLED, HIGH);
    
    // Valid card buzzer pattern
    tone(buzzerPin, 1200, 100);
    delay(150);
    tone(buzzerPin, 1200, 100);

    lockServo.write(90); // Unlock
    delay(3000);
    lockServo.write(0);  // Lock again
    digitalWrite(greenLED, LOW);
  } else {
    Serial.println("Access Denied");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Denied!");
    lcd.setCursor(0, 1);
    lcd.print("Invalid Card");

    digitalWrite(redLED, HIGH);
    
    // Invalid card buzzer pattern
    tone(buzzerPin, 400, 1000);
    delay(1000);

    digitalWrite(redLED, LOW);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place your card");

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

bool isAuthorized(byte *uid) {
  for (int i = 0; i < numberOfCards; i++) {
    bool match = true;
    for (byte j = 0; j < 4; j++) {
      if (uid[j] != authorizedUIDs[i][j]) {
        match = false;
        break;
      }
    }
    if (match) return true;
  }
  return false;
}