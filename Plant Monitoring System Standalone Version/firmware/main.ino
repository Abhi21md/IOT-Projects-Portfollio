#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ---------------- PIN DEFINITIONS ----------------
#define SOIL_PIN A0
#define DHTPIN 2
#define DHTTYPE DHT11
#define RELAY_PIN 7
#define BUZZER 8

// ---------------- OBJECTS ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

// ---------------- SETTINGS ----------------
int soilThreshold = 40;        // % threshold for dry soil
float tempThreshold = 35.0;    // Temperature alert level

void setup() {

  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(BUZZER, LOW);

  lcd.init();
  lcd.backlight();

  dht.begin();

  lcd.setCursor(0,0);
  lcd.print("Plant Monitor");
  lcd.setCursor(0,1);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

// ---------------- SOIL MOISTURE FUNCTION ----------------
int readSoil() {

  int rawValue = analogRead(SOIL_PIN);

  // Map to percentage
  int moisture = map(rawValue, 1023, 0, 0, 100);
  moisture = constrain(moisture, 0, 100);

  return moisture;
}

// ---------------- MAIN LOOP ----------------
void loop() {

  int soilMoisture = readSoil();
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Soil: ");
  Serial.print(soilMoisture);
  Serial.print("% | Temp: ");
  Serial.print(temperature);
  Serial.print("C | Humidity: ");
  Serial.println(humidity);

  lcd.clear();

  // Line 1 → Temperature & Humidity
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print("C H:");
  lcd.print(humidity);

  // Line 2 → Soil & Pump
  lcd.setCursor(0,1);
  lcd.print("Soil:");
  lcd.print(soilMoisture);
  lcd.print("% ");

  // -------- AUTO WATER CONTROL --------
  if (soilMoisture < soilThreshold) {

    digitalWrite(RELAY_PIN, HIGH);  // Pump ON
    lcd.print("ON ");
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);

  } else {

    digitalWrite(RELAY_PIN, LOW);   // Pump OFF
    lcd.print("OFF");
  }

  // -------- HIGH TEMP ALERT --------
  if (temperature > tempThreshold) {
    digitalWrite(BUZZER, HIGH);
    delay(300);
    digitalWrite(BUZZER, LOW);
  }

  delay(3000);
}
