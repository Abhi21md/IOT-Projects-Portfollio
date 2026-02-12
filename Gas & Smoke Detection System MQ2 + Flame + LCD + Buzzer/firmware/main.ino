#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define MQ2_PIN A0
#define FLAME_PIN 6
#define BUZZER 8
#define ALERT_LED 9

#define GAS_THRESHOLD 400

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

  Serial.begin(9600);

  pinMode(FLAME_PIN, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(ALERT_LED, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Gas Detection");
  lcd.setCursor(0,1);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {

  int gasValue = analogRead(MQ2_PIN);
  bool flameDetected = digitalRead(FLAME_PIN) == LOW;

  lcd.setCursor(0,0);
  lcd.print("Gas:");
  lcd.print(gasValue);
  lcd.print("   ");

  lcd.setCursor(0,1);

  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  if (gasValue > GAS_THRESHOLD || flameDetected) {

    digitalWrite(BUZZER, HIGH);
    digitalWrite(ALERT_LED, HIGH);

    if (flameDetected) {
      lcd.print("FLAME ALERT!");
      Serial.println("FLAME DETECTED!");
    } else {
      lcd.print("GAS LEAK ALERT!");
      Serial.println("GAS LEAK DETECTED!");
    }

  } else {

    digitalWrite(BUZZER, LOW);
    digitalWrite(ALERT_LED, LOW);
    lcd.print("SAFE ENVIRONMENT");
  }

  delay(1000);
}
