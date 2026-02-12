COLOR SORTING SYSTEM – SYSTEM OVERVIEW
--------------------------------------

This project is an automated color-based object sorting system using:

• ESP32 Microcontroller
• TCS3200 Color Sensor
• IR Object Detection Sensor
• 3 Servo Motors (Red, Green, Blue bins)

WORKING PRINCIPLE:

1. IR sensor detects incoming object.
2. System pauses briefly to stabilize object.
3. TCS3200 measures Red, Green, Blue frequency values.
4. Lowest frequency value determines dominant color.
5. Corresponding servo rotates to divert object.
6. Servo returns to neutral position.

FEATURES:

• Real-time color detection
• Automatic sorting mechanism
• Three separate bins
• Low-cost industrial prototype
• Serial Monitor debugging

APPLICATIONS:

• Industrial sorting systems
• Recycling plants
• Packaging industries
• Educational robotics labs
