SYSTEM OVERVIEW
---------------

Project Name:
AI Based Smart Waste Segregation System

Description:
This system combines Artificial Intelligence and IoT hardware to automatically segregate waste into:

• Metal
• Wet Waste
• Paper
• Plastic

Working Flow:
1. IR sensor detects object.
2. Metal sensor checks if metal.
3. Moisture sensor checks wet/dry.
4. If dry waste:
   → Camera captures image
   → Python AI model classifies paper or plastic
   → Result sent to ESP32 via Serial
5. Stepper motor rotates bin.
6. Servo opens gate to drop waste.

AI Model:
MobileNetV2 Transfer Learning
Trained on custom dataset:
dataset/
  ├── paper/
  └── plastic/

Hardware + AI Communication:
Serial communication between Python and ESP32.
