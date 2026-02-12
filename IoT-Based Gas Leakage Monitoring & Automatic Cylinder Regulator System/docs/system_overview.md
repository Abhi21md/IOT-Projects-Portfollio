# IoT Gas Leakage Monitoring & Automatic Regulator Control

Board: NodeMCU ESP8266
Sensor: MQ2 Gas Sensor
Actuator: Servo Motor (Cylinder Regulator)
Cloud: Blynk IoT
Display: 16x2 I2C LCD

--------------------------------------------------

## System Purpose

This system continuously monitors LPG gas levels using MQ2 sensor.

If gas crosses the threshold:
• Buzzer activates
• Regulator automatically closes
• Alert is sent to Blynk
• LCD shows warning

It also supports:
• Adjustable gas threshold from Blynk
• Manual open/close override
• Real-time cloud monitoring
• Smart LCD status display

--------------------------------------------------

## Core Features

✔ Real-time gas monitoring
✔ Adjustable threshold (V4 slider)
✔ Servo-based automatic regulator control
✔ Manual override via Blynk button
✔ Buzzer alert system
✔ LCD live status display
✔ Alert state change detection
✔ Reduced cloud spam using SEND_DIFF logic
✔ Clean user interface
✔ Safety-first automatic closure logic
