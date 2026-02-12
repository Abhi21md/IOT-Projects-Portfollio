# ESP32 WiFi Controlled Robo Car

Board: ESP32
Platform: Blynk IoT
Motor Driver: L298N (or compatible)
Control: WiFi via Mobile App

--------------------------------------------------

## Working Principle

The ESP32 connects to WiFi and Blynk Cloud.

User presses buttons in Blynk app:
• Forward
• Backward
• Left
• Right
• Speed Control Slider

ESP32 receives commands and controls motors using PWM via LEDC channels.

--------------------------------------------------

## Key Features

✔ WiFi based control
✔ Real-time speed adjustment
✔ Smooth PWM control (8-bit resolution)
✔ LEDC hardware PWM (ESP32 native)
✔ Serial debugging output
✔ Simple and stable logic

--------------------------------------------------

## Control Mapping

V0 → Forward  
V1 → Backward  
V2 → Left  
V3 → Right  
V4 → Speed Slider  

--------------------------------------------------

## Motor Control Logic

Speed control via:
ledcWrite(channel, Speed)

Direction controlled using:
IN1, IN2, IN3, IN4 digital signals
