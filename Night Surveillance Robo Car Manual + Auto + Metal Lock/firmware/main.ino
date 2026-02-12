/* Night Surveillance Robo Car - NodeMCU (ESP8266)
 * ------------------------------------------------
 * Manual control via Blynk
 * Auto-surveillance patrol: forward & backward in place
 * Created by Abhishek Madiwal (Simplified Auto Mode)
 */

#define BLYNK_TEMPLATE_ID "TMPL3ZtfG1xhM"
#define BLYNK_TEMPLATE_NAME "Robo Car"
#define BLYNK_AUTH_TOKEN "nlCG4SPrjWQRXSDkj3ZM_I-MUqJ319T4"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Motor PINs
#define ENA D0
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define ENB D5

// Variables
bool forward = 0, backward = 0, left = 0, right = 0;
int Speed = 700; // Default speed

bool autoMode = false;
int distanceMeters = 1;  // User input (in meters)
bool autoRunning = false;

// Timed sequence variables
unsigned long actionStart = 0;
int autoStep = 0; // 0=idle, 1=fwd, 2=back
unsigned long moveTime = 1000; // 1 sec per meter (simplified)

// WiFi credentials
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
  carStop();

  Blynk.begin(auth, ssid, pass);
  Serial.println("System Ready...");
}

// Manual control widgets
BLYNK_WRITE(V0) { forward = param.asInt(); }
BLYNK_WRITE(V1) { backward = param.asInt(); }
BLYNK_WRITE(V2) { left = param.asInt(); }
BLYNK_WRITE(V3) { right = param.asInt(); }
BLYNK_WRITE(V4) { Speed = param.asInt(); } // Speed slider

// Auto mode switch
BLYNK_WRITE(V5) {
  autoMode = param.asInt();
  if (autoMode) {
    Serial.println("Auto Mode: ON");
    startAutoSurveillance();
  } else {
    Serial.println("Auto Mode: OFF");
    autoRunning = false;
    carStop();
  }
}

// Distance input (in seconds per meter equivalent)
BLYNK_WRITE(V6) {
  distanceMeters = param.asInt();
  moveTime = distanceMeters * 1000; // 1 second per input number
  Serial.print("Set Auto Move Time: ");
  Serial.print(moveTime / 1000);
  Serial.println(" second(s)");
}

void loop() {
  Blynk.run();

  if (autoMode) {
    runAutoSurveillance();
  } else {
    manualControl();
  }
}

// =============== Manual Control ==================
void manualControl() {
  if (forward) {
    carForward();
    Serial.println("Manual: Forward");
  } else if (backward) {
    carBackward();
    Serial.println("Manual: Backward");
  } else if (left) {
    carTurnLeft();
    Serial.println("Manual: Left");
  } else if (right) {
    carTurnRight();
    Serial.println("Manual: Right");
  } else {
    carStop();
  }
}

// =============== Auto Surveillance Logic ==================
void startAutoSurveillance() {
  autoRunning = true;
  autoStep = 1;
  actionStart = millis();
  Serial.println("Auto-surveillance started (forward-back loop)...");
}

void runAutoSurveillance() {
  if (!autoRunning) return;

  unsigned long currentTime = millis();

  switch (autoStep) {
    case 1: // Forward
      carForward();
      if (currentTime - actionStart >= moveTime) {
        carStop();
        Serial.println("Auto Step 1: Forward complete");
        autoStep = 2;
        actionStart = millis();
      }
      break;

    case 2: // Backward
      carBackward();
      if (currentTime - actionStart >= moveTime) {
        carStop();
        Serial.println("Auto Step 2: Backward complete");
        autoStep = 1; // Repeat the cycle
        actionStart = millis();
      }
      break;

    default:
      carStop();
      autoStep = 1;
      actionStart = millis();
      break;
  }

  // Live update if distance or speed changes
  moveTime = distanceMeters * 1000;
}

// =============== Motor Functions ==================
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
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
