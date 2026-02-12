#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WiFi Credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

ESP8266WebServer server(80);

// Motor Pins
#define IN1 D1
#define IN2 D2
#define IN3 D5
#define IN4 D6
#define ENA D3
#define ENB D4

#define IR_SENSOR D7
#define BUZZER D8
#define LED D0

int targetSlot = 0;
int currentSlot = 0;
bool moving = false;

// ---------------- HTML PAGE ----------------
String webpage = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>BIBLIORA Book Finder</title>
<style>
body { text-align:center; font-family:Arial; }
button { padding:15px; margin:5px; font-size:18px; }
</style>
</head>
<body>
<h2>BIBLIORA - Select Slot</h2>
<form action="/set">
<input type="number" name="slot" min="1" max="10" required>
<button type="submit">Find Book</button>
</form>
</body>
</html>
)=====";

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IR_SENSOR, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);

  analogWrite(ENA, 200);
  analogWrite(ENB, 200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/html", webpage);
  });

  server.on("/set", []() {
    targetSlot = server.arg("slot").toInt();
    if (targetSlot >= 1 && targetSlot <= 10) {
      currentSlot = 0;
      moving = true;
      moveForward();
    }
    server.send(200, "text/html", webpage);
  });

  server.begin();
}

// ---------------- LOOP ----------------
void loop() {
  server.handleClient();

  if (moving && digitalRead(IR_SENSOR) == LOW) {
    delay(300);
    currentSlot++;

    if (currentSlot == targetSlot) {
      stopRobot();
      alert();
      moving = false;
    }
  }
}

// ---------------- MOTOR CONTROL ----------------
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopRobot() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ---------------- ALERT ----------------
void alert() {
  digitalWrite(LED, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(2000);
  digitalWrite(LED, LOW);
  digitalWrite(BUZZER, LOW);
}
