HARDWARE REQUIRED PER BOOTH
-------------------------------------

• ESP32 Dev Board
• Fingerprint Sensor (R307 / AS608)
• 16x2 I2C LCD
• 0.96 OLED Display (SSD1306)
• 4 Push Buttons (Party Selection)
• Buzzer
• Reset Button
• Power Supply (5V regulated)

-------------------------------------

CONNECTION SUMMARY
-------------------------------------

Fingerprint:
TX → GPIO16
RX → GPIO17

OLED:
SDA → GPIO21
SCL → GPIO22

Buttons:
BJP → GPIO12
CONGRESS → GPIO13
JDS → GPIO14
NOTA → GPIO15

Reset Button:
GPIO15

Buzzer:
GPIO23

LCD:
I2C Address 0x27

-------------------------------------

MULTI BOOTH SETUP
-------------------------------------

Booth 1 → Same hardware
Booth 2 → Same hardware
Both connected to same Firebase project

No extra code changes required.
