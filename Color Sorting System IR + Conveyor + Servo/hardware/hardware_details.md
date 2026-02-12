HARDWARE DETAILS
----------------

1. ESP32 Development Board

2. TCS3200 Color Sensor
   S0  → GPIO 4
   S1  → GPIO 5
   S2  → GPIO 18
   S3  → GPIO 19
   OUT → GPIO 21

3. IR Sensor
   OUT → GPIO 34

4. Servo Motors
   Red Servo   → GPIO 25
   Green Servo → GPIO 26
   Blue Servo  → GPIO 27

5. Power Supply
   5V external supply recommended for servos
   Common GND between ESP32 and servo supply

NOTE:
Do NOT power all servos directly from ESP32 5V pin.
Use external regulated supply to avoid brownout reset.
