# Hardware Configuration

---------------------------------
NodeMCU (ESP8266)
---------------------------------

---------------------------------
DHT11 Sensor
---------------------------------
DATA → D4
VCC  → 3.3V
GND  → GND

---------------------------------
Soil Moisture Sensor
---------------------------------
Analog Output → A0
VCC → 3.3V
GND → GND

---------------------------------
PIR Motion Sensor
---------------------------------
OUT → D5
VCC → 5V
GND → GND

---------------------------------
Relay Module
---------------------------------
IN  → D3
VCC → 5V
GND → GND

---------------------------------
Push Button
---------------------------------
Pin → D7
Mode → INPUT_PULLUP

---------------------------------
LCD (16x2 I2C)
---------------------------------
SDA → D2
SCL → D1
Address → 0x27

---------------------------------
Blynk Virtual Pins
---------------------------------
V10 → Temperature
V11 → Humidity
V12 → Soil Moisture
V13 → PIR LED
V14 → Relay Control
V15 → PIR Enable Toggle
