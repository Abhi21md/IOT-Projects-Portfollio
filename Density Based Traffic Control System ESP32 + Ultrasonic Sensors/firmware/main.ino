// ---------------- CONFIG ----------------
#define THRESHOLD 20   // Distance in cm for vehicle detection
#define GREEN_TIME 5000
#define YELLOW_TIME 1000

// ---------------- ULTRASONIC PINS ----------------
// Road 1, 2, 3, 4
int trigPins[4] = {5, 17, 4, 15};
int echoPins[4] = {18, 16, 2, 13};

// ---------------- TRAFFIC LIGHT PINS ----------------
// Format: [Road][R, Y, G]
int redPins[4]    = {25, 14, 32, 19};
int yellowPins[4] = {26, 12, 35, 21};
int greenPins[4]  = {27, 33, 34, 22};

long duration;
int distance[4];

// ---------------- SETUP ----------------
void setup() {

  Serial.begin(115200);

  for (int i = 0; i < 4; i++) {

    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);

    pinMode(redPins[i], OUTPUT);
    pinMode(yellowPins[i], OUTPUT);
    pinMode(greenPins[i], OUTPUT);
  }

  setAllRed();
  Serial.println("Density Based Traffic Control Started");
}

// ---------------- ULTRASONIC FUNCTION ----------------
int getDistance(int trigPin, int echoPin) {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  int dist = duration * 0.034 / 2;

  if (dist == 0 || dist > 400) dist = 400; // No vehicle case

  return dist;
}

// ---------------- SET ALL RED ----------------
void setAllRed() {

  for (int i = 0; i < 4; i++) {
    digitalWrite(redPins[i], HIGH);
    digitalWrite(yellowPins[i], LOW);
    digitalWrite(greenPins[i], LOW);
  }
}

// ---------------- GIVE GREEN TO ROAD ----------------
void giveGreen(int road) {

  setAllRed();

  digitalWrite(redPins[road], LOW);
  digitalWrite(yellowPins[road], HIGH);
  delay(YELLOW_TIME);

  digitalWrite(yellowPins[road], LOW);
  digitalWrite(greenPins[road], HIGH);

  Serial.print("Green Signal Given To Road: ");
  Serial.println(road + 1);

  delay(GREEN_TIME);

  digitalWrite(greenPins[road], LOW);
  digitalWrite(redPins[road], HIGH);
}

// ---------------- MAIN LOOP ----------------
void loop() {

  // Read all 4 road distances
  for (int i = 0; i < 4; i++) {

    distance[i] = getDistance(trigPins[i], echoPins[i]);

    Serial.print("Road ");
    Serial.print(i + 1);
    Serial.print(" Distance: ");
    Serial.print(distance[i]);
    Serial.println(" cm");
  }

  // Find road with minimum distance (highest density)
  int minRoad = 0;

  for (int i = 1; i < 4; i++) {
    if (distance[i] < distance[minRoad]) {
      minRoad = i;
    }
  }

  // If vehicle detected within threshold
  if (distance[minRoad] < THRESHOLD) {
    giveGreen(minRoad);
  }
  else {
    setAllRed();
    Serial.println("No Traffic Detected - All Red");
    delay(1000);
  }

  Serial.println("---------------------------");
}
