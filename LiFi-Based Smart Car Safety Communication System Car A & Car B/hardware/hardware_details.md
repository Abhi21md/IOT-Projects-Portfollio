# Hardware Details

-----------------------------------
Car-A (WiFi + Smoke Lock)
-----------------------------------

Board: NodeMCU ESP8266
Motor Driver: L298N
Sensor: MQ3 Alcohol/Smoke
Communication: WiFi + LiFi

Pins:
ENA → D0
IN1 → D1
IN2 → D2
IN3 → D3
IN4 → D4
ENB → D5
MQ3 → A0

-----------------------------------
Car-B (LiFi Decode Receiver)
-----------------------------------

Board: Arduino / ESP
Sensor: LDR Module
Display: 16x2 I2C LCD

LDR_PIN → D2
LCD I2C → SDA, SCL

-----------------------------------
Required Components

• 2x NodeMCU / Arduino
• 2x L298N Motor Drivers
• 4 DC Motors
• MQ3 Sensor
• LDR Sensor
• 16x2 I2C LCD
• LED for LiFi transmission
• Batteries
• Jumper wires
