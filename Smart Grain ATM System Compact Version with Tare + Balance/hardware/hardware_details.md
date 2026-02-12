HARDWARE CONNECTION DETAILS
----------------------------

Board: ESP32

---------------- RFID ----------------
SS  → GPIO 5
RST → GPIO 4
SCK → 18
MOSI → 23
MISO → 19
3.3V → VCC
GND → GND

---------------- HX711 ----------------
DOUT → GPIO 32
CLK  → GPIO 33
VCC  → 5V
GND  → GND

---------------- SERVO ----------------
Signal → GPIO 25
VCC → External 5V Recommended
GND → Common Ground

---------------- BUTTONS ----------------
100g → GPIO 27
200g → GPIO 26
TARE → GPIO 13

---------------- BUZZER ----------------
GPIO 12

---------------- LCD ----------------
SDA → GPIO 21
SCL → GPIO 22
Address → 0x27

IMPORTANT:
Use external regulated 5V supply for servo and load cell.
Ensure common ground between ESP32 and all modules.
