HARDWARE DETAILS
----------------

MCU 1: NodeMCU (ESP8266)
--------------------------------
ENA → D0
IN1 → D1
IN2 → D2
IN3 → D3
IN4 → D4
ENB → D5
Metal Detector → D6

Motor Driver: L298N

MCU 2: Radar Module
--------------------------------
Servo X → D2
Servo Z → D1

Ultrasonic Left:
Trig → D5
Echo → D6
Laser LED → D7

Ultrasonic Right:
Trig → D3
Echo → D4
Laser LED → D8

Buzzer → D0

MCU 3: ESP32-CAM
--------------------------------
AI Thinker ESP32-CAM
Camera: OV2640
Streaming over WiFi

Power Requirements:
- Separate 7.4V battery for motors
- 5V regulated supply for logic
- Common GND across modules
