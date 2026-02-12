# Dual Ultrasonic Radar Surveillance System

## Overview
This project is a dual-direction ultrasonic radar surveillance system built using NodeMCU (ESP8266), two ultrasonic sensors, and two SG90 servo motors.

The system continuously sweeps horizontally and vertically. When an object is detected within the threshold distance, the system locks onto the object and activates visual and audio alerts.

---

## Key Features

- Dual ultrasonic object detection (Left & Right)
- Horizontal + Vertical radar sweep
- Object locking mechanism
- LED beam indication (Left / Right)
- Buzzer alert system
- Real-time serial monitoring
- Adjustable detection threshold

---

## Working Principle

1. Both ultrasonic sensors continuously measure distance.
2. Two servos create radar-style sweeping motion.
3. If object distance <= 40 cm:
   - Corresponding LED turns ON
   - Buzzer activates
   - Radar locks position
4. When object moves away:
   - Sweep resumes

---

## Hardware Used

- NodeMCU ESP8266
- 2 × HC-SR04 Ultrasonic Sensors
- 2 × SG90 Servo Motors
- 2 × LEDs
- 1 × Buzzer
- Breadboard
- Jumper Wires

---

## Pin Configuration

| Component | Pin |
|------------|------|
| Servo X (Horizontal) | D2 |
| Servo Z (Vertical) | D1 |
| Left Trig | D5 |
| Left Echo | D6 |
| Left LED | D7 |
| Right Trig | D3 |
| Right Echo | D4 |
| Right LED | D8 |
| Buzzer | D0 |

---

## Folder Structure

