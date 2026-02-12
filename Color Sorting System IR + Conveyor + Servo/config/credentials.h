COLOR SORTING SYSTEM
--------------------

Board: ESP32
Baud Rate: 115200

Sensor: TCS3200 Color Sensor
Object Detection: IR Sensor
Output: 3 Servo Motors (Red, Green, Blue)

No WiFi credentials required.
Standalone embedded system.

Key Adjustable Parameters:

1. neutral = 90
   - Default servo resting position.

2. activePos = 30
   - Servo position when diverting object.

3. Frequency comparison logic:
   - Lowest frequency determines detected color.

If color detection is unstable:
- Adjust lighting conditions.
- Recalibrate frequency readings.
- Add threshold margin in detectColor().
