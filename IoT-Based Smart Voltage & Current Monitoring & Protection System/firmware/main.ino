#define BLYNK_TEMPLATE_ID "TMPL3Tgv-rM5O"
#define BLYNK_TEMPLATE_NAME "vtg"
#define BLYNK_AUTH_TOKEN "BGApT16o7ufVKyKthlJVGJVgbE-347he"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

// WiFi
char ssid[] = "Smart Bin";
char pass[] = "7899262658";

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pins
const int voltagePin = 35;
const int currentPin = 34;
const int relayPin   = 26;
const int buzzerPin  = 25;

// ADC / calibration
const int vSamples = 1000;
float voltageCalibration = 864.0;
long vOffset = 0;
float prevV = 0.0;

float acsSensitivity = 0.185;
float currentZero = 2048;
float manualScale = 0.67;
float noLoadCutoff = 0.08;

// Fault / thresholds
float setVoltageLow  = 160.0;
float setVoltageHigh = 260.0;
float setCurrentLimit = 8.0;

// Flags
bool voltageFaultFlag = false;
bool currentFaultFlag = false;
bool tripped = false;
bool manualRelayOverride = false;

// Buzzer timing
const unsigned long buzzerOnMs  = 3000UL;
const unsigned long buzzerOffMs = 1000UL;
unsigned long buzzerLastToggle = 0;
bool buzzerState = false;

BlynkTimer timer;

// Blynk pins
#define VOLT_DS     V0
#define CURR_DS     V1
#define STATUS_DS   V2
#define RESET_DS    V3
#define RELAY_CTRL  V12
#define VOLT_SLIDER V10
#define VOLT_MAX_SLIDER V4
#define CURR_SLIDER V11

float readVoltage();
float readCurrent();
long readVOffset();
long readCurrentOffset();
void autoZeroCalibration();
void updateBuzzer();
void evaluateFaults();
void setRelay(bool on);

void sendStatus(String msg) {
  Blynk.virtualWrite(STATUS_DS, msg);
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0); lcd.print("Power Monitor");
  lcd.setCursor(0,1); lcd.print("Starting...");

  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  digitalWrite(buzzerPin, LOW);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  vOffset = readVOffset();
  currentZero = readCurrentOffset();

  sendStatus("BOOTING...");
  timer.setInterval(1000L, evaluateFaults);
  timer.setInterval(200L, updateBuzzer);
  timer.setInterval(3000L, autoZeroCalibration);
}

void loop() {
  Blynk.run();
  timer.run();
}

// Sliders
BLYNK_WRITE(VOLT_SLIDER) { setVoltageLow = param.asFloat(); }
BLYNK_WRITE(VOLT_MAX_SLIDER) { setVoltageHigh = param.asFloat(); }
BLYNK_WRITE(CURR_SLIDER) { setCurrentLimit = param.asFloat(); }

// Manual relay control
BLYNK_WRITE(RELAY_CTRL) {
  manualRelayOverride = (param.asInt() != 0);
  if (manualRelayOverride) {
    setRelay(true);
    tripped = false;
    currentFaultFlag = false;
    sendStatus("RELAY ON MANUAL");
  } else {
    setRelay(false);
    sendStatus("RELAY OFF MANUAL");
  }
}

// Reset
BLYNK_WRITE(RESET_DS) {
  if (param.asInt() == 1) {
    tripped = false;
    voltageFaultFlag = false;
    currentFaultFlag = false;
    setRelay(true);
    sendStatus("RESET DONE");
    Blynk.virtualWrite(RESET_DS, 0);
  }
}

void updateBuzzer() {
  bool needBuzzer = voltageFaultFlag || currentFaultFlag;
  unsigned long now = millis();
  if (!needBuzzer) { noTone(buzzerPin); buzzerState = false; return; }

  if (!buzzerState && now - buzzerLastToggle >= buzzerOffMs) {
    tone(buzzerPin, 1000);
    buzzerState = true;
    buzzerLastToggle = now;
  } else if (buzzerState && now - buzzerLastToggle >= buzzerOnMs) {
    noTone(buzzerPin);
    buzzerState = false;
    buzzerLastToggle = now;
  }
}

void evaluateFaults() {
  float V = readVoltage();
  float I = (digitalRead(relayPin) == LOW) ? 0.0 : readCurrent();  // Force 0 if relay off

  // AC OFF
  if (V < 40.0) {
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("V:0.0 I:0.00");
    lcd.setCursor(0,1); lcd.print("AC OFF");

    Blynk.virtualWrite(VOLT_DS, 0);
    Blynk.virtualWrite(CURR_DS, 0);
    sendStatus("AC OFF");
    return;
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("V:"); lcd.print(V,1);
  lcd.print(" I:"); lcd.print(I,2);

  Blynk.virtualWrite(VOLT_DS, V);
  Blynk.virtualWrite(CURR_DS, I);

  bool voltageFault = (V > setVoltageHigh) || (V < setVoltageLow);
  bool currentFault = (I > setCurrentLimit);

  if (voltageFault && !voltageFaultFlag) {
    voltageFaultFlag = true;
    tripped = true;
    setRelay(false);
    sendStatus("VOLT FAULT");
    Blynk.logEvent("voltage_fault", "Voltage Out of Range");
  }

  if (!voltageFault && voltageFaultFlag) {
    voltageFaultFlag = false;
    tripped = false;
    if (!currentFaultFlag && !manualRelayOverride) setRelay(true);
    sendStatus("VOLT OK");
    Blynk.logEvent("power_restored", "Voltage Normal");
  }

  if (currentFault && !currentFaultFlag) {
    currentFaultFlag = true;
    tripped = true;
    setRelay(false);
    sendStatus("CURRENT FAULT");
    Blynk.logEvent("current_fault", "Overcurrent Detected");
  }

  lcd.setCursor(0,1);
  if (voltageFaultFlag) lcd.print("VOLT FAULT");
  else if (currentFaultFlag) lcd.print("CURR FAULT");
  else {
    lcd.print("SAFE        ");
    sendStatus("SAFE");
  }
}

void setRelay(bool on) {
  digitalWrite(relayPin, on ? HIGH : LOW);
  Blynk.virtualWrite(RELAY_CTRL, on ? 1 : 0);
}

float readVoltage() {
  long sum = 0;
  for (int i = 0; i < vSamples; i++) {
    int adc = analogRead(voltagePin);
    int centered = adc - vOffset;
    sum += (long)centered * centered;
  }
  float rms = sqrt((float)sum / vSamples);
  float volts = (rms * voltageCalibration) / 2048.0;
  volts = (volts + prevV) / 2.0;
  prevV = volts;
  if (volts < 5.0) volts = 0.0;
  return volts;
}

float readCurrent() {
  long sum = 0;
  int samples = 600;
  for (int i = 0; i < samples; i++) {
    int raw = analogRead(currentPin);
    float centered = raw - currentZero;
    sum += (long)centered * centered;
  }
  float rmsADC = sqrt((float)sum / samples);
  float voltageRMS = (rmsADC * 3.3) / 4095.0;
  float amps = (voltageRMS / acsSensitivity) * manualScale;
  static float smoothI = 0.0;
  smoothI = (smoothI * 0.75) + (amps * 0.25);
  if (smoothI < noLoadCutoff) smoothI = 0.0;
  return smoothI;
}

long readVOffset() {
  long total = 0;
  for (int i = 0; i < vSamples; i++) total += analogRead(voltagePin);
  return total / vSamples;
}

long readCurrentOffset() {
  long total = 0;
  const int N = 1500;
  for (int i = 0; i < N; i++) total += analogRead(currentPin);
  return total / N;
}

void autoZeroCalibration() {
  float I = readCurrent();
  if (I < 0.05) {
    long total = 0;
    const int N = 500;
    for (int i = 0; i < N; i++) total += analogRead(currentPin);
    float measured = (float)total / N;
    currentZero = (currentZero * 0.95) + (measured * 0.05);
  }
}
