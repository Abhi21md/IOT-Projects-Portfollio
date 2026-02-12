# IoT-Based Vehicle Speed Monitoring System

Board: NodeMCU ESP8266
Platform: Blynk IoT
Sensors: Dual IR Sensors
Display: 16x2 I2C LCD
Alert: Active HIGH Buzzer

--------------------------------------------------

## Working Principle

Two IR sensors are placed at a fixed distance (20 cm).

When a vehicle passes:
• IR1 detects vehicle → Start timer
• IR2 detects vehicle → Stop timer
• Speed calculated using:

Speed = (Distance / Time) × 3.6

Converted to km/h.

--------------------------------------------------

## Features

✔ Real-time speed calculation
✔ Adjustable speed limit from Blynk
✔ Overspeed buzzer alert
✔ Event logging via Blynk
✔ Car count tracking
✔ LCD live display
✔ Cloud logging to Blynk Terminal

--------------------------------------------------

## System Flow

1. Vehicle crosses IR1
2. Timer starts
3. Vehicle crosses IR2
4. Speed calculated
5. Compared with set speed limit
6. If exceeded:
   → Buzzer ON (2 sec)
   → Blynk event triggered
7. Data sent to Blynk
8. LCD displays result
