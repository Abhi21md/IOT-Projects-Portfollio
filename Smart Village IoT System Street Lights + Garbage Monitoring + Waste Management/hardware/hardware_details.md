HARDWARE CONNECTION DETAILS
----------------------------

Board: NodeMCU ESP8266

---------------- STREET LIGHT ----------------

LDR → A0

IR Sensors:
IR1 → D5
IR2 → D6
IR3 → D7

Street Lamps:
Lamp1 → D0
Lamp2 → D3
Lamp3 → D4

---------------- GARBAGE MONITOR ----------------

Ultrasonic Sensor:
TRIG → D1
ECHO → D2

Buzzer → D8

---------------- SERVO ----------------
Servo Gate → D9 (example expansion)

---------------- LCD ----------------
I2C LCD:
SDA → D2
SCL → D1
Address → 0x27

NOTE:
Ensure common ground across all modules.
Use external 5V supply for lamps if required.
