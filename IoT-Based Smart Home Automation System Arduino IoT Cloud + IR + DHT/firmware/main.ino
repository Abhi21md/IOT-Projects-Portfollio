#include "thingProperties.h"
#include <DHT.h>

// Relay Pins
const int Switch1 = 18;  // Hall light
const int Switch2 = 19;
const int Switch3 = 22;
const int Switch4 = 23;
const int Switch5 = 5;   // Temp control relay

// IR Sensor Pin
const int irSensor = 27;
bool irTriggered = false;

// DHT Sensor
#define DHTPIN 26
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

float tempThreshold = 30.0;

void setup() {
  Serial.begin(9600);
  delay(1500);

  pinMode(Switch1, OUTPUT);
  pinMode(Switch2, OUTPUT);
  pinMode(Switch3, OUTPUT);
  pinMode(Switch4, OUTPUT);
  pinMode(Switch5, OUTPUT);
  pinMode(irSensor, INPUT);

  dht.begin();

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();

  // IR Motion Detection for Light1
  if (digitalRead(irSensor) == LOW && irTriggered == false) {
    irTriggered = true;
    light1 = true;   // update cloud variable also
    onLight1Change();
    Serial.println("Hall Light ON (IR Trigger)");
  }

  // Temperature Monitoring
  float t = dht.readTemperature();
  if (!isnan(t)) {
    Serial.print("Temp: ");
    Serial.println(t);

    if (t > tempThreshold) {
      light5 = true;
    } else {
      light5 = false;
    }
    onLight5Change();
  }
}

/*********** Cloud Callbacks ***********/

void onLight1Change() {
  if (light1) {
    digitalWrite(Switch1, HIGH);
    Serial.println("Hall Light ON");
  } else {
    digitalWrite(Switch1, LOW);
    Serial.println("Hall Light OFF");
    irTriggered = false; // allow IR to work again
  }
}

void onLight2Change() {
  digitalWrite(Switch2, light2 ? HIGH : LOW);
  Serial.println(light2 ? "Light2 ON" : "Light2 OFF");
}

void onLight3Change() {
  digitalWrite(Switch3, light3 ? HIGH : LOW);
  Serial.println(light3 ? "Light3 ON" : "Light3 OFF");
}

void onLight4Change() {
  digitalWrite(Switch4, light4 ? HIGH : LOW);
  Serial.println(light4 ? "Light4 ON" : "Light4 OFF");
}

void onLight5Change() {
  digitalWrite(Switch5, light5 ? HIGH : LOW);
  Serial.println(light5 ? "Relay ON (Temp)" : "Relay OFF");
}   