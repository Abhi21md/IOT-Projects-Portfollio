#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_TEMPLATE_NAME "WATER"
#define BLYNK_AUTH_TOKEN "MbJjs5rBZYlEWiErNuInSi92iow8aDA0"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Wi-Fi credentials
char ssid[] = "Smart Bin";
char pass[] = "7899262658";

// DS18B20 temperature sensor
#define ONE_WIRE_BUS D2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// TDS sensor
#define TDS_PIN A0

// Buzzer
#define BUZZER_PIN D1

// TDS calculation (based on voltage)
float getTDS(int analogValue) {
  float voltage = analogValue * (3.3 / 1024.0);
  float tdsValue = (133.42 * voltage * voltage * voltage) - 
                   (255.86 * voltage * voltage) + 
                   (857.39 * voltage);
  return tdsValue;
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  sensors.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  Blynk.run();

  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  int analogValue = analogRead(TDS_PIN);
  float tds = getTDS(analogValue);

  // Send data to Blynk
  Blynk.virtualWrite(V0, tds);
  Blynk.virtualWrite(V1, tempC);

  // Debug
  Serial.print("TDS: "); Serial.print(tds); Serial.print(" ppm | ");
  Serial.print("Temp: "); Serial.print(tempC); Serial.println(" °C");

  // Alert conditions
  if (tds > 500 || tempC > 32) {
    digitalWrite(BUZZER_PIN, HIGH);
    Blynk.logEvent("water_alert", "High TDS or Temperature Detected!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(2000);
}