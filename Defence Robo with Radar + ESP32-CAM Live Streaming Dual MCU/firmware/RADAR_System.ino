#include <Servo.h>

Servo servoX;   // SG90 for horizontal sweep
Servo servoZ;   // SG90 for vertical tilt

// Ultrasonic 1 (Left side)
#define trig1 D5
#define echo1 D6
#define led1 D7   // Left LED beam

// Ultrasonic 2 (Right side)
#define trig2 D3
#define echo2 D4
#define led2 D8   // Right LED beam

#define buzzer D0

int distanceThreshold = 40; // cm threshold

// Sweep variables
int posX = 0;
int dirX = 5;
int posZ = 60;
int dirZ = 5;

// ====== Function to get distance from ultrasonic ======
long getDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 20000); // timeout 20ms
  long distance = duration * 0.034 / 2;
  return distance;
}

void setup() {
  Serial.begin(9600);

  servoX.attach(D2, 500, 2500);  // horizontal
  servoZ.attach(D1, 500, 2500);  // vertical

  // Pin setup for both sensors
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(led1, OUTPUT);

  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(led2, OUTPUT);

  pinMode(buzzer, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(buzzer, LOW);

  Serial.println("Dual Ultrasonic Radar System Initialized");
}

void loop() {
  // Read both ultrasonic sensors
  int distLeft = getDistance(trig1, echo1);
  int distRight = getDistance(trig2, echo2);

  Serial.print("X: "); Serial.print(posX);
  Serial.print(" | Z: "); Serial.print(posZ);
  Serial.print(" | Left: "); Serial.print(distLeft);
  Serial.print("cm | Right: "); Serial.println(distRight);

  // Check left ultrasonic sensor
  if (distLeft > 0 && distLeft <= distanceThreshold) {
    digitalWrite(led1, HIGH);
    digitalWrite(buzzer, HIGH);

    Serial.println("Object detected on LEFT side!");
    servoX.write(posX);
    servoZ.write(posZ);

    // Stay locked while object detected
    while (true) {
      distLeft = getDistance(trig1, echo1);
      if (distLeft > distanceThreshold || distLeft <= 0) break;
      delay(100);
    }

    digitalWrite(led1, LOW);
    digitalWrite(buzzer, LOW);
    Serial.println("Left side object gone. Resuming sweep.");
  }

  // Check right ultrasonic sensor
  else if (distRight > 0 && distRight <= distanceThreshold) {
    digitalWrite(led2, HIGH);
    digitalWrite(buzzer, HIGH);

    Serial.println("Object detected on RIGHT side!");
    servoX.write(posX);
    servoZ.write(posZ);

    // Stay locked while object detected
    while (true) {
      distRight = getDistance(trig2, echo2);
      if (distRight > distanceThreshold || distRight <= 0) break;
      delay(100);
    }

    digitalWrite(led2, LOW);
    digitalWrite(buzzer, LOW);
    Serial.println("Right side object gone. Resuming sweep.");
  }

  // Continue sweeping if no object
  servoX.write(posX);
  servoZ.write(posZ);

  delay(50);

  // Update servo positions for sweep
  posX += dirX;
  posZ += dirZ;

  // Horizontal servo sweep (0–180°)
  if (posX >= 180 || posX <= 0) dirX = -dirX;

  // Vertical servo sweep (60–120°)
  if (posZ >= 120 || posZ <= 60) dirZ = -dirZ;
}
