/* ESP32 WIFI control car using Blynk
 * Converted from ESP8266 version
 */
#define BLYNK_TEMPLATE_ID "TMPL3tKnm_wB9"
#define BLYNK_TEMPLATE_NAME "VOLTAGE"
#define BLYNK_AUTH_TOKEN "1c_8W6u7EIighyAD-XfyOnYiCDc3mz-l"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Motor Pins (change as per your wiring on ESP32)
#define ENA  16   // Motor A speed pin (PWM)
#define IN1  17   // Motor A direction
#define IN2  18
#define IN3  19   // Motor B direction
#define IN4  21
#define ENB  22   // Motor B speed pin (PWM)

// PWM setup
#define ENA_CHANNEL 0
#define ENB_CHANNEL 1
#define PWM_FREQ 1000
#define PWM_RESOLUTION 8   // 8-bit (0-255)

// Movement flags
bool forward = 0;
bool backward = 0;
bool left = 0;
bool right = 0;
int Speed = 255;   // default speed

// Blynk credentials
char auth[] = "1c_8W6u7EIighyAD-XfyOnYiCDc3mz-l"; //Enter your Blynk application auth token
char ssid[] = "Smart Bin";        // Your WiFi SSID
char pass[] = "7899262658";       // Your WiFi Password
 
void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Setup PWM channels
  ledcSetup(ENA_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(ENB_CHANNEL, PWM_FREQ, PWM_RESOLUTION);

  // Attach channels to pins
  ledcAttachPin(ENA, ENA_CHANNEL);
  ledcAttachPin(ENB, ENB_CHANNEL);

  Blynk.begin(auth, ssid, pass);
}

// Blynk virtual pin functions
BLYNK_WRITE(V0) { forward = param.asInt(); }
BLYNK_WRITE(V1) { backward = param.asInt(); }
BLYNK_WRITE(V2) { left = param.asInt(); }
BLYNK_WRITE(V3) { right = param.asInt(); }
BLYNK_WRITE(V4) { Speed = param.asInt(); }

void loop() {
  Blynk.run();
  smartcar();
}

void smartcar() {
  if (forward) {
    carforward();
    Serial.println("carforward");
  } else if (backward) {
    carbackward();
    Serial.println("carbackward");
  } else if (left) {
    carturnleft();
    Serial.println("carleft");
  } else if (right) {
    carturnright();
    Serial.println("carright");
  } else {
    carStop();
    Serial.println("carstop");
  }
}

// Car movement functions
void carforward() {
  ledcWrite(ENA_CHANNEL, Speed);
  ledcWrite(ENB_CHANNEL, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carbackward() {
  ledcWrite(ENA_CHANNEL, Speed);
  ledcWrite(ENB_CHANNEL, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carturnleft() {
  ledcWrite(ENA_CHANNEL, Speed);
  ledcWrite(ENB_CHANNEL, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carturnright() {
  ledcWrite(ENA_CHANNEL, Speed);
  ledcWrite(ENB_CHANNEL, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carStop() {
  ledcWrite(ENA_CHANNEL, 0);
  ledcWrite(ENB_CHANNEL, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
