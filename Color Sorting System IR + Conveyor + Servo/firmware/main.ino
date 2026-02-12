/************************************************************
   Project: Color Sorting System
   Board: ESP32
   Sensor: TCS3200 Color Sensor
   IR Sensor: Object Detection
   Output: 3 Servo Motors (Red, Green, Blue)
************************************************************/

#include <Arduino.h>
#include <ESP32Servo.h>

// ---------------- TCS3200 PINS ----------------
#define S0  4
#define S1  5
#define S2  18
#define S3  19
#define OUT 21

// ---------------- IR SENSOR ----------------
#define IR_SENSOR 34   // Object detection

// ---------------- SERVO PINS ----------------
#define SERVO_RED   25
#define SERVO_GREEN 26
#define SERVO_BLUE  27

Servo servoRed;
Servo servoGreen;
Servo servoBlue;

// ----------- Frequency Variables ------------
int redFreq = 0;
int greenFreq = 0;
int blueFreq = 0;

// ----------- Neutral Position ---------------
int neutral = 90;
int activePos = 30;

void setup() {

  Serial.begin(115200);

  // TCS3200 pin modes
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // IR Sensor
  pinMode(IR_SENSOR, INPUT);

  // Set frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Attach servos
  servoRed.attach(SERVO_RED);
  servoGreen.attach(SERVO_GREEN);
  servoBlue.attach(SERVO_BLUE);

  // Neutral position
  servoRed.write(neutral);
  servoGreen.write(neutral);
  servoBlue.write(neutral);

  Serial.println("Color Sorting System Ready...");
}

void loop() {

  if (digitalRead(IR_SENSOR) == LOW) {  // Object detected

    delay(300);  // stabilize object

    readColor();

    Serial.print("R: "); Serial.print(redFreq);
    Serial.print(" G: "); Serial.print(greenFreq);
    Serial.print(" B: "); Serial.println(blueFreq);

    detectColor();

    delay(1000);  // wait before next object
  }
}

void readColor() {

  // -------- RED --------
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  redFreq = pulseIn(OUT, LOW);

  delay(100);

  // -------- GREEN ------
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  greenFreq = pulseIn(OUT, LOW);

  delay(100);

  // -------- BLUE -------
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  blueFreq = pulseIn(OUT, LOW);

  delay(100);
}

void detectColor() {

  if (redFreq < greenFreq && redFreq < blueFreq) {
    Serial.println("Detected: RED");
    servoRed.write(activePos);
    delay(500);
    servoRed.write(neutral);
  }

  else if (greenFreq < redFreq && greenFreq < blueFreq) {
    Serial.println("Detected: GREEN");
    servoGreen.write(activePos);
    delay(500);
    servoGreen.write(neutral);
  }

  else if (blueFreq < redFreq && blueFreq < greenFreq) {
    Serial.println("Detected: BLUE");
    servoBlue.write(activePos);
    delay(500);
    servoBlue.write(neutral);
  }

  else {
    Serial.println("Unknown Color");
  }
}
