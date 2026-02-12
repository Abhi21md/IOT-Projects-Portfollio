#define BLYNK_TEMPLATE_ID "TMPL3XSjzwAQy"
#define BLYNK_TEMPLATE_NAME "Medicine"
#define BLYNK_AUTH_TOKEN "RPHoSxnIETvW-eJjcBLlZnAVUSAsASuD"

#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Blynk Auth
char auth[] = "RPHoSxnIETvW-eJjcBLlZnAVUSAsASuD";

// WiFi Credentials
char ssid[] = "Smart Bin";
char pass[] = "7899262658";

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pins
#define SET_BTN     D4
#define HR_BTN      D5
#define MIN_BTN     D6
#define LED_MORNING D7
#define LED_AFTERNOON D8
#define LED_EVENING D3
#define BUZZER      D0

int alarmHour = 8;
int alarmMinute = 0;
bool alarmActive = false;
bool alertGiven = false;
bool freezeLCD = false;

unsigned long freezeStart = 0;
unsigned long lastBlynkNotification = 0;

bool setupMode = false;
unsigned long pressStart = 0;

BlynkTimer timer;

// ============================
// STOP ALERT FUNCTION
// ============================
void stopAlert() {
  alarmActive = false;
  freezeLCD = false;
  digitalWrite(BUZZER, LOW);
  digitalWrite(LED_MORNING, LOW);
  digitalWrite(LED_AFTERNOON, LOW);
  digitalWrite(LED_EVENING, LOW);
  Blynk.virtualWrite(V2, "Idle");
  Blynk.virtualWrite(V3, 0);
}

// ============================
// TRIGGER ALARM
// ============================
void triggerAlarm(int hr) {
  alarmActive = true;
  freezeLCD = true;
  freezeStart = millis();
  digitalWrite(BUZZER, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("*MEDICINE TIME*");
  lcd.setCursor(0, 1);

  String slot = "General Slot";

  if (hr >= 6 && hr < 12) {
    digitalWrite(LED_MORNING, HIGH);
    slot = "Morning Slot";
  } 
  else if (hr >= 12 && hr < 18) {
    digitalWrite(LED_AFTERNOON, HIGH);
    slot = "Afternoon Slot";
  }
  else if (hr >= 18 && hr <= 22) {
    digitalWrite(LED_EVENING, HIGH);
    slot = "Evening Slot";
  }

  lcd.print(slot);
  Blynk.virtualWrite(V2, slot);

  // Avoid notification spam. Minimum 30 sec gap
  if (millis() - lastBlynkNotification > 30000) {
    Blynk.logEvent("med_time", "Time to take your medicine");
    lastBlynkNotification = millis();
  }
}

// ============================
// BLYNK STOP BUTTON (V3)
// ============================
BLYNK_WRITE(V3) {
  if (alarmActive) stopAlert();
}

// ============================
// DISPLAY TIME + BLYNK
// ============================
void updateClock() {
  if (freezeLCD) {
    if (millis() - freezeStart >= 5000) {
      freezeLCD = false;
    } else {
      return;  // hold alert screen
    }
  }

  DateTime now = rtc.now();

  lcd.setCursor(0, 0);
  int hr = now.hour();
  String period = hr >= 12 ? "PM" : "AM";
  if (hr > 12) hr -= 12;
  if (hr == 0) hr = 12;

  lcd.print((hr < 10 ? "0" : "") + String(hr) + ":");
  lcd.print((now.minute() < 10 ? "0" : "") + String(now.minute()) + ":");
  lcd.print((now.second() < 10 ? "0" : "") + String(now.second()) + " " + period);

  lcd.setCursor(0, 1);
  lcd.print("Next: ");
  lcd.print((alarmHour < 10 ? "0" : "") + String(alarmHour) + ":");
  lcd.print((alarmMinute < 10 ? "0" : "") + String(alarmMinute) + "   ");

  Blynk.virtualWrite(V0, String(hr) + ":" + now.minute() + " " + period);
  Blynk.virtualWrite(V1, String(alarmHour) + ":" + alarmMinute);
}

// ============================
// CHECK ALARM
// ============================
void checkAlarm() {
  DateTime now = rtc.now();

  if (now.hour() == alarmHour && now.minute() == alarmMinute && !alertGiven) {
    triggerAlarm(now.hour());
    alertGiven = true;
  }

  if (now.minute() != alarmMinute) alertGiven = false;
}

// ============================
// BUTTON HANDLER
// ============================
void buttonHandler() {
  if (digitalRead(SET_BTN) == LOW) {
    if (pressStart == 0) pressStart = millis();
  } 
  else if (pressStart > 0) {
    if (millis() - pressStart >= 2000) {
      setupMode = !setupMode;
      lcd.clear();
      lcd.print(setupMode ? "SET ALARM MODE" : "Saved");
      delay(800);
    }
    pressStart = 0;
  }

  if (setupMode) {
    if (digitalRead(HR_BTN) == LOW) { delay(200); alarmHour = (alarmHour + 1) % 24; }
    if (digitalRead(MIN_BTN) == LOW) { delay(200); alarmMinute = (alarmMinute + 1) % 60; }
  }

  if (alarmActive && digitalRead(SET_BTN) == LOW) stopAlert();
}

// ============================
// SETUP
// ============================
void setup() {
  Serial.begin(9600);
  Wire.begin(D2, D1);

  lcd.begin(16, 2);
  lcd.setBacklight(true);

  rtc.begin();

  pinMode(SET_BTN, INPUT_PULLUP);
  pinMode(HR_BTN, INPUT_PULLUP);
  pinMode(MIN_BTN, INPUT_PULLUP);
  pinMode(LED_MORNING, OUTPUT);
  pinMode(LED_AFTERNOON, OUTPUT);
  pinMode(LED_EVENING, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  stopAlert();
  lcd.clear();
  lcd.print("Medicine System");
  delay(1500);

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(1000L, updateClock);
  timer.setInterval(1500L, checkAlarm);
}

// ============================
// LOOP
// ============================
void loop() {
  Blynk.run();
  timer.run();
  buttonHandler();
}
