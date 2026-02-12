# Hardware Configuration

---------------------------------
NodeMCU (ESP8266)
---------------------------------

---------------------------------
DHT11 Sensor
---------------------------------
DATA → D4
VCC → 3.3V
GND → GND

---------------------------------
Soil Moisture Sensor
---------------------------------
Analog Output → A0
VCC → 3.3V
GND → GND

---------------------------------
Relay Module
---------------------------------
IN → D7
VCC → 5V
GND → GND

Logic:
HIGH = Motor ON
LOW  = Motor OFF

---------------------------------
LCD (I2C 16x2)
---------------------------------
SDA → D2
SCL → D1
Address → 0x27

---------------------------------
Blynk Virtual Pins
---------------------------------
V0  → Soil %
V1  → Temperature
V2  → Humidity
V4  → Auto Mode Switch
V5  → Pump Status LED
V10 → Manual Pump Button
