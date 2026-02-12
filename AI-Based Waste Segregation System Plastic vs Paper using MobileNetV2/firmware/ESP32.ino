#include <AccelStepper.h>
#include <ESP32Servo.h>

Servo servo1;

// SENSOR PINS
#define IR_PIN       5       // Object detection
#define PROXI_PIN    18      // Metal detection
#define BUZZER_PIN   23
#define SOIL_PIN     34      // Moisture sensor

// SERVO PIN
#define SERVO_PIN    13

// STEPPER PINS
#define IN1 25
#define IN2 26
#define IN3 27
#define IN4 14

// AccelStepper setup
AccelStepper stepper(AccelStepper::FULL4WIRE, IN1, IN3, IN2, IN4);

int wetThreshold = 20;

void setup() {
  Serial.begin(115200);

  pinMode(IR_PIN, INPUT);
  pinMode(PROXI_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  servo1.attach(SERVO_PIN);

  stepper.setMaxSpeed(800);
  stepper.setAcceleration(400);

  servo1.write(180);
  delay(1000);
  servo1.write(70);
  delay(500);

  Serial.println("SYSTEM READY");
}

// ----------------- FUNCTIONS ------------------

void beep() {
  tone(BUZZER_PIN, 1000, 300);
}

void openGate() {
  servo1.write(180);
  delay(700);
  servo1.write(70);
  delay(400);
}

void rotateTo(long degrees) {
  long steps = degrees * 11.377;
  stepper.moveTo(steps);
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }
  delay(300);
}

int readMoisture() {
  int avg = 0;

  for (int i = 0; i < 3; i++) {
    int raw = analogRead(SOIL_PIN);
    raw = constrain(raw, 500, 2800);
    avg += map(raw, 500, 2800, 100, 0);
    delay(50);
  }

  avg /= 3;
  Serial.print("Moisture: ");
  Serial.println(avg);

  return avg;
}

// ----------------- MAIN LOOP ------------------

void loop() {

  // METAL DIRECT
  if (digitalRead(PROXI_PIN) == LOW) {
    Serial.println("METAL");
    beep();
    rotateTo(300);     // metal bin
    openGate();
    rotateTo(0);
    return;
  }

  // OBJECT DETECTED
  if (digitalRead(IR_PIN) == LOW) {
    Serial.println("OBJECT");
    beep();

    int moisture = readMoisture();

    // WET BIN
    if (moisture > wetThreshold) {
      Serial.println("WET");
      rotateTo(120);    // wet bin
      openGate();
      rotateTo(0);
      return;
    }

    // DRY WASTE -> WAIT FOR PYTHON
    Serial.println("DRY");
    waitForAI();
  }
}

void waitForAI() {
  unsigned long start = millis();
  Serial.println("WAITING_AI");

  while (millis() - start < 8000) {  // wait 8 seconds
    if (Serial.available()) {
      String result = Serial.readStringUntil('\n');
      result.trim();

      if (result == "PLASTIC") {
        Serial.println("Plastic detected");
        rotateTo(240);
        openGate();
        rotateTo(0);
        return;
      }

      if (result == "PAPER") {
        Serial.println("Paper detected");
        rotateTo(0);
        openGate();
        rotateTo(0);
        return;
      }
    }
  }

  Serial.println("AI TIMEOUT -> default PAPER");
  rotateTo(0);
  openGate();
  rotateTo(0);
}
 