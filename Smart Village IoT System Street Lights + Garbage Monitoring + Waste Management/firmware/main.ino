#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// ---------------- BLYNK ----------------
#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_TEMPLATE_NAME "SmartVillage"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

char ssid[] = "Smart Bin";
char pass[] = "7899262658";

// ---------------- STREET LIGHT ----------------
#define LDR_PIN A0
#define IR1 D5
#define IR2 D6
#define IR3 D7

#define LAMP1 D0
#define LAMP2 D3
#define LAMP3 D4

int darkThreshold = 600;
unsigned long offDelay = 3000;
unsigned long lastDetect1 = 0;
unsigned long lastDetect2 = 0;
unsigned long lastDetect3 = 0;

// ---------------- GARBAGE MONITORING ----------------
#define TRIG D1
#define ECHO D2
#define BUZZER D8

int binHeight = 25; // cm total height

// ---------------- LCD ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- SERVO (Waste Gate Example) ----------------
Servo wasteServo;
#define SERVO_PIN D9

BlynkTimer timer;

// ================== FUNCTION TO READ BIN LEVEL ==================
long getDistance() {

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  long distance = duration * 0.034 / 2;

  return distance;
}

// ================== SMART STREET LIGHT ==================
void controlStreetLights() {

  int ldrValue = analogRead(LDR_PIN);
  bool isDark = (ldrValue > darkThreshold);

  bool ir1 = digitalRead(IR1) == LOW;
  bool ir2 = digitalRead(IR2) == LOW;
  bool ir3 = digitalRead(IR3) == LOW;

  if (isDark) {

    if (ir1) {
      digitalWrite(LAMP1, HIGH);
      lastDetect1 = millis();
    }

    if (ir2) {
      digitalWrite(LAMP2, HIGH);
      lastDetect2 = millis();
    }

    if (ir3) {
      digitalWrite(LAMP3, HIGH);
      lastDetect3 = millis();
    }

    if (millis() - lastDetect1 > offDelay) digitalWrite(LAMP1, LOW);
    if (millis() - lastDetect2 > offDelay) digitalWrite(LAMP2, LOW);
    if (millis() - lastDetect3 > offDelay) digitalWrite(LAMP3, LOW);

  } else {
    digitalWrite(LAMP1, LOW);
    digitalWrite(LAMP2, LOW);
    digitalWrite(LAMP3, LOW);
  }
}

// ================== GARBAGE MONITOR ==================
void monitorGarbage() {

  long distance = getDistance();
  int levelPercent = map(distance, binHeight, 2, 0, 100);
  levelPercent = constrain(levelPercent, 0, 100);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Village");

  lcd.setCursor(0, 1);
  lcd.print("Bin:");
  lcd.print(levelPercent);
  lcd.print("%");

  Blynk.virtualWrite(V0, levelPercent);

  if (levelPercent >= 80) {
    digitalWrite(BUZZER, HIGH);
    Blynk.logEvent("bin_full", "Garbage Bin Almost Full!");
  } else {
    digitalWrite(BUZZER, LOW);
  }
}

// ================== SETUP ==================
void setup() {

  Serial.begin(9600);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);

  pinMode(LAMP1, OUTPUT);
  pinMode(LAMP2, OUTPUT);
  pinMode(LAMP3, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);

  wasteServo.attach(SERVO_PIN);
  wasteServo.write(0);

  lcd.init();
  lcd.backlight();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, controlStreetLights);
  timer.setInterval(2000L, monitorGarbage);

  lcd.setCursor(0, 0);
  lcd.print("SMART VILLAGE");
  delay(2000);
  lcd.clear();
}

// ================== LOOP ==================
void loop() {
  Blynk.run();
  timer.run();
}
