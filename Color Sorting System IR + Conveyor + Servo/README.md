# Color Sorting System (ESP32 + TCS3200)

## Overview

This project implements an automatic color-based object sorting system using ESP32 and TCS3200 color sensor.

Objects are detected using an IR sensor and sorted into Red, Green, or Blue bins using servo motors.

---

## Features

• Real-time color detection  
• IR-based object detection  
• Three servo-based sorting gates  
• Low-cost automation prototype  
• Serial debugging for calibration  

---

## Components Used

- ESP32
- TCS3200 Color Sensor
- IR Sensor
- 3x SG90 Servo Motors
- External 5V Power Supply

---

## Working Logic

1. IR sensor detects object.
2. TCS3200 measures RGB frequency.
3. Lowest frequency determines color.
4. Corresponding servo rotates.
5. Object is diverted to respective bin.

---

## Pin Configuration

| Component | ESP32 Pin |
|-----------|-----------|
| S0        | 4         |
| S1        | 5         |
| S2        | 18        |
| S3        | 19        |
| OUT       | 21        |
| IR Sensor | 34        |
| Red Servo | 25        |
| Green Servo | 26      |
| Blue Servo | 27       |

---

## Applications

- Industrial automation
- Recycling plants
- Smart packaging
- Educational robotics projects

---

Developed as part of IoT & Embedded Systems Portfolio.
