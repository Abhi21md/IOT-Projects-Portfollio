#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_TEMPLATE_NAME "MineSafety"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

// -------- WIFI --------
char ssid[] = "Smart Bin";
char pass[] = "7899262658";

// -------- SENSOR PINS --------
#define MQ2 A0
#define DHTPIN D4
#define DHTTYPE DHT11
#define BUZZER D3
#define ALERT_LED D5

// -------- RFID --------
#define SS_PIN D8
#define RST_PIN D0
MFRC522 rfid(SS_PIN, RST_PIN);

// -------- LCD --------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// -------- GAS THRESHOLD --------
#define GAS_THRESHOLD 400

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// -------- WORKER DATABASE --------
String workerUIDs[3] = {
  "85473306",
  "23e3591a",
  "4121d655"
};

String workerNames[3] = {
  "Ramesh",
  "Suresh",
  "Mahesh"
};

// ================= SETUP =================
void setup() {

  Serial.begin(9600);

  pinMode(BUZZER, OUTPUT);
  pinMode(ALERT_LED, OUTPUT);

  lcd.init();
  lcd.backlight();

  SPI.begin();
  rfid.PCD_Init();

  dht.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  lcd.setCursor(0,0);
  lcd.print("Mine Monitoring");
  lcd.setCursor(0,1);
  lcd.print("System Ready");

  timer.setInterval(2000L, sendSensorData);
}

// ================= LOOP =================
void loop() {
  Blynk.run();
  timer.run();
  checkWorkerRFID();
}

// ================= SENSOR DATA =================
void sendSensorData() {

  int gasValue = analogRead(MQ2);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Gas:");
  lcd.print(gasValue);

  lcd.setCursor(0,1);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print(" H:");
  lcd.print(humidity);

  Blynk.virtualWrite(V0, gasValue);
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, humidity);

  Serial.print("Gas: ");
  Serial.print(gasValue);
  Serial.print(" Temp: ");
  Serial.print(temperature);
  Serial.print(" Hum: ");
  Serial.println(humidity);

  if (gasValue > GAS_THRESHOLD) {
    digitalWrite(BUZZER, HIGH);
    digitalWrite(ALERT_LED, HIGH);
    Blynk.logEvent("gas_alert", "Dangerous Gas Level in Mine!");
  } else {
    digitalWrite(BUZZER, LOW);
    digitalWrite(ALERT_LED, LOW);
  }
}

// ================= WORKER RFID =================
void checkWorkerRFID() {

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  String uid = "";

  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);
  }

  uid.toLowerCase();
  rfid.PICC_HaltA();

  int index = getWorkerIndex(uid);

  lcd.clear();

  if (index == -1) {
    lcd.print("Unknown Worker");
    Blynk.logEvent("worker_alert", "Unknown Worker Detected!");
    delay(2000);
  } else {
    lcd.print("Worker:");
    lcd.setCursor(0,1);
    lcd.print(workerNames[index]);

    Blynk.virtualWrite(V3, workerNames[index]);
    Blynk.logEvent("worker_entry", workerNames[index] + " Entered Mine");
    delay(2000);
  }
}

// ================= FIND WORKER =================
int getWorkerIndex(String uid) {
  for (int i = 0; i < 3; i++) {
    if (workerUIDs[i] == uid)
      return i;
  }
  return -1;
}
