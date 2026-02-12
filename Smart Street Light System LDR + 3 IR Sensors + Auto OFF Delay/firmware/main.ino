// Arduino UNO + LDR + 3 IR Sensors + 3 Street Lamps

const int LDR_PIN = A0;

const int IR1 = 2;
const int IR2 = 3;
const int IR3 = 4;

const int LAMP1 = 5;
const int LAMP2 = 6;
const int LAMP3 = 7;

// Light threshold
int darkThreshold = 500;

// Auto turn-off delay
unsigned long offDelay = 2000;
unsigned long lastDetect1 = 0;
unsigned long lastDetect2 = 0;
unsigned long lastDetect3 = 0;

void setup() {
  Serial.begin(9600);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);

  pinMode(LAMP1, OUTPUT);
  pinMode(LAMP2, OUTPUT);
  pinMode(LAMP3, OUTPUT);

  digitalWrite(LAMP1, LOW);
  digitalWrite(LAMP2, LOW);
  digitalWrite(LAMP3, LOW);

  Serial.println("Smart Street Light System Started");
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);
  bool isDark = (ldrValue > darkThreshold);

  bool ir1 = digitalRead(IR1) == LOW;
  bool ir2 = digitalRead(IR2) == LOW;
  bool ir3 = digitalRead(IR3) == LOW;

  // Print LDR and threshold
  Serial.print("LDR Value: ");
  Serial.print(ldrValue);
  Serial.print(" | Threshold: ");
  Serial.print(darkThreshold);
  Serial.print(" | Darkness: ");
  Serial.println(isDark ? "YES" : "NO");

  // Print IR sensor states
  Serial.print("IR1: "); Serial.print(ir1 ? "DETECTED" : "NO");
  Serial.print(" | IR2: "); Serial.print(ir2 ? "DETECTED" : "NO");
  Serial.print(" | IR3: "); Serial.println(ir3 ? "DETECTED" : "NO");

  // Darkness condition
  if (isDark) {
    if (ir1) {
      digitalWrite(LAMP1, HIGH);
      lastDetect1 = millis();
    }
    if (ir2) {
      digitalWrite(LAMP2, HIGH);
      lastDetect2 = millis();
    }
    if (ir3) {
      digitalWrite(LAMP3, HIGH);
      lastDetect3 = millis();
    }

    if (millis() - lastDetect1 > offDelay) digitalWrite(LAMP1, LOW);
    if (millis() - lastDetect2 > offDelay) digitalWrite(LAMP2, LOW);
    if (millis() - lastDetect3 > offDelay) digitalWrite(LAMP3, LOW);

  } else {
    digitalWrite(LAMP1, LOW);
    digitalWrite(LAMP2, LOW);
    digitalWrite(LAMP3, LOW);
  }

  // Print lamp states
  Serial.print("Lamp1: "); Serial.print(digitalRead(LAMP1) ? "ON" : "OFF");
  Serial.print(" | Lamp2: "); Serial.print(digitalRead(LAMP2) ? "ON" : "OFF");
  Serial.print(" | Lamp3: "); Serial.println(digitalRead(LAMP3) ? "ON" : "OFF");

  Serial.println("-------------------------------------------");

  delay(300);
}