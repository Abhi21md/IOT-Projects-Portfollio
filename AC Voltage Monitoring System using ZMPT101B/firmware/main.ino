#define BLYNK_TEMPLATE_ID "TMPL3Tgv-rM5O"
#define BLYNK_TEMPLATE_NAME "vtg"
#define BLYNK_AUTH_TOKEN "BGApT16o7ufVKyKthlJVGJVgbE-347he"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <math.h>

char ssid[] = "Smart Bin";       
char pass[] = "7899262658";      

const int voltagePin = 35;              // Analog input pin for ZMPT101B
float calibration = 850.0;              // Calibrated value
const int sampleCount = 500;           // Samples per cycle
float offset = 0.0;                    // DC offset

BlynkTimer timer;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);             // 12-bit ADC
  delay(1000);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  offset = getOffset();

  timer.setInterval(500L, sendVoltageToBlynk);  // Update every 500ms
}

void loop() {
  Blynk.run();
  timer.run();
}

// Function to read voltage offset when AC is OFF
float getOffset() {
  long sum = 0;
  for (int i = 0; i < 500; i++) {
    sum += analogRead(voltagePin);
    delayMicroseconds(1000);
  }
  return sum / 500.0;
}

// Function to measure RMS voltage
float getRMSVoltage() {
  long squareSum = 0;
  for (int i = 0; i < sampleCount; i++) {
    int raw = analogRead(voltagePin);
    int val = raw - offset;
    squareSum += val * val;
    delayMicroseconds(1000);
  }

  float mean = squareSum / (float)sampleCount;
  float rms = sqrt(mean);
  float voltage = (rms * calibration) / 2048.0;
  return voltage;
}

// Send voltage value and status to Blynk
void sendVoltageToBlynk() {
  float voltage = getRMSVoltage();
  if (voltage < 15.0) voltage = 0.0;  // Ignore noise

  Serial.print("AC Voltage (RMS): ");
  Serial.print(voltage, 1);
  Serial.println(" V");

  Blynk.virtualWrite(V0, voltage);

  if (voltage == 0.0) {
    Blynk.virtualWrite(V1, "AC OFF");
  } else if (voltage >= 180 && voltage <= 260) {
    Blynk.virtualWrite(V1, "AC ON - SAFE");
  } else {
    Blynk.virtualWrite(V1, "AC ON - UNSAFE");
  }
}