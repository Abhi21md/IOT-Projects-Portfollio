#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ------------ BLYNK CREDENTIALS ------------
char auth[] = "YOUR_BLYNK_AUTH";
char ssid[] = "YOUR_WIFI";
char pass[] = "YOUR_PASSWORD";

// ------------ PIN DEFINITIONS ------------
#define SOIL A0
#define DHTPIN D4
#define RELAY D5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;

// ------------ VARIABLES ------------
int soilPercent = 0;
float temperature = 0;
float humidity = 0;

int prevSoil = -1;
float prevTemp = -100;
float prevHum = -100;

int soilThreshold = 35;  // Auto pump ON below this

// ------------ SENSOR FUNCTION ------------
void readSensors() {

  int rawSoil = analogRead(SOIL);
  soilPercent = map(rawSoil, 1023, 0, 0, 100);

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // -------- CHANGE DETECTION --------
  if (abs(soilPercent - prevSoil) > 3 ||
      abs(temperature - prevTemp) > 1 ||
      abs(humidity - prevHum) > 3) {

    Blynk.virtualWrite(V0, soilPercent);
    Blynk.virtualWrite(V1, temperature);
    Blynk.virtualWrite(V2, humidity);

    prevSoil = soilPercent;
    prevTemp = temperature;
    prevHum = humidity;
  }

  // -------- AUTO PUMP CONTROL --------
  if (soilPercent < soilThreshold) {
    digitalWrite(RELAY, LOW);   // Pump ON (Active LOW)
    Blynk.virtualWrite(V3, 1);
  } else {
    digitalWrite(RELAY, HIGH);  // Pump OFF
    Blynk.virtualWrite(V3, 0);
  }

  // -------- LCD DISPLAY --------
  lcd.setCursor(0, 0);
  lcd.print("Soil:");
  lcd.print(soilPercent);
  lcd.print("%   ");

  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print(" H:");
  lcd.print(humidity);
  lcd.print("   ");
}

void setup() {
  Serial.begin(9600);

  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);

  lcd.init();
  lcd.backlight();

  dht.begin();
  Blynk.begin(auth, ssid, pass);

  timer.setInterval(2000L, readSensors);
}

void loop() {
  Blynk.run();
  timer.run();
}
