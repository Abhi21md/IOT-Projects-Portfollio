# Hardware Details

----------------------------------
Microcontroller
----------------------------------
NodeMCU ESP8266

----------------------------------
Sensor
----------------------------------
HC-SR04 Ultrasonic Sensor

TRIG → D6
ECHO → D5 (Use 1K–2K voltage divider)

----------------------------------
Relay Module
----------------------------------
Relay → D7
Active HIGH

----------------------------------
Buzzer
----------------------------------
Active buzzer → D4
HIGH = ON

----------------------------------
LCD Display
----------------------------------
16x2 I2C LCD
SDA → D2
SCL → D1

----------------------------------
Power Supply
----------------------------------
5V regulated for NodeMCU
Relay external load isolated properly

----------------------------------
Important Notes

• Use voltage divider on Echo pin
• Keep wiring short for ultrasonic stability
• Relay must be rated for charging load
