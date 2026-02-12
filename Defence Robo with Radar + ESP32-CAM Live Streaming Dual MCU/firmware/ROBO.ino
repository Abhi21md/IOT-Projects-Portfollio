/* Night Surveillance Robo Car - NodeMCU (ESP8266)
 * ------------------------------------------------
 * Manual control + Auto mode
 * Metal detector safety lock added
 */

#define BLYNK_TEMPLATE_ID "TMPL3ZtfG1xhM"
#define BLYNK_TEMPLATE_NAME "Robo Car"
#define BLYNK_AUTH_TOKEN "nlCG4SPrjWQRXSDkj3ZM_I-MUqJ319T4"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Motor pins
#define ENA D0
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define ENB D5

// Metal detector pin
#define METAL_PIN D6      // HIGH = metal detected

// Variables
bool forward = 0, backward = 0, left = 0, right = 0;
int Speed = 1023;

bool autoMode = false;
int distanceMeters = 1;
bool autoRunning = false;

unsigned long actionStart = 0;
int autoStep = 0;
unsigned long moveTime = 1000;

char auth[] = "nlCG4SPrjWQRXSDkj3ZM_I-MUqJ319T4";
char ssid[] = "Smart Bin";
char pass[] = "7899262658";

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(METAL_PIN, INPUT);       // metal detector input

  carStop();

  Blynk.begin(auth, ssid, pass);
  Serial.println("System Ready...");
}

// Manual buttons
BLYNK_WRITE(V0) { forward = param.asInt(); }
BLYNK_WRITE(V1) { backward = param.asInt(); }
BLYNK_WRITE(V2) { left = param.asInt(); }
BLYNK_WRITE(V3) { right = param.asInt(); }
BLYNK_WRITE(V4) { Speed = param.asInt(); }

// Auto mode switch
BLYNK_WRITE(V5) {
  autoMode = param.asInt();
  if (autoMode) {
    startAutoSurveillance();
  } else {
    autoRunning = false;
    carStop();
  }
}

// Distance input
BLYNK_WRITE(V6) {
  distanceMeters = param.asInt();
  moveTime = distanceMeters * 1000;
}

void loop() {
  Blynk.run();

  int metalState = digitalRead(METAL_PIN);

  // If metal detected → block forward/back and stop car
  if (metalState == HIGH) {
    carStop();                         // immediate stop
    autoRunning = false;               // stop auto mode
    Serial.println("METAL DETECTED - Forward/Backward LOCKED");

    // Only allow left/right
    if (left) carTurnLeft();
    else if (right) carTurnRight();
    return;
  }

  // Normal operation
  if (autoMode) runAutoSurveillance();
  else manualControl();
}

// ================= Manual Mode =================
void manualControl() {
  if (forward) carForward();
  else if (backward) carBackward();
  else if (left) carTurnLeft();
  else if (right) carTurnRight();
  else carStop();
}

// ================= Auto Mode =================
void startAutoSurveillance() {
  autoRunning = true;
  autoStep = 1;
  actionStart = millis();
}

void runAutoSurveillance() {
  if (!autoRunning) return;

  // If metal suddenly found during auto → stop everything
  if (digitalRead(METAL_PIN) == HIGH) {
    carStop();
    autoRunning = false;
    return;
  }

  unsigned long now = millis();

  switch (autoStep) {
    case 1:
      carForward();
      if (now - actionStart >= moveTime) {
        carStop();
        autoStep = 2;
        actionStart = millis();
      }
      break;

    case 2:
      carBackward();
      if (now - actionStart >= moveTime) {
        carStop();
        autoStep = 1;
        actionStart = millis();
      }
      break;
  }
}

// ================= Motor Functions =================
void carForward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carBackward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carTurnLeft() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carTurnRight() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carStop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
