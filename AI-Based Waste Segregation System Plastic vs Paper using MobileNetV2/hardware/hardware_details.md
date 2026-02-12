HARDWARE DETAILS
----------------

Controller:
ESP32

Sensors:
1. IR Sensor (Object detection)
2. Inductive Proximity Sensor (Metal detection)
3. Soil Moisture Sensor (Wet detection)
4. USB Camera (AI classification)

Actuators:
1. Stepper Motor (Bin rotation)
2. Servo Motor (Gate control)
3. Buzzer

Pin Mapping:

IR Sensor        → GPIO 5
Metal Sensor     → GPIO 18
Moisture Sensor  → GPIO 34
Servo            → GPIO 13
Stepper IN1      → GPIO 25
Stepper IN2      → GPIO 26
Stepper IN3      → GPIO 27
Stepper IN4      → GPIO 14
Buzzer           → GPIO 23

Power:
5V external supply for motors
USB for ESP32
