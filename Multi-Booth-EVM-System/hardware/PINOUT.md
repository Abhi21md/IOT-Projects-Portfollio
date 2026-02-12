# Pinout

As currently used in firmware:

- Fingerprint RX/TX: GPIO 16 / GPIO 17
- OLED SDA/SCL: GPIO 21 / GPIO 22
- BJP button: GPIO 12
- Congress button: GPIO 13
- JDS button: GPIO 14
- NOTA button: GPIO 15
- Reset button: GPIO 15
- Buzzer: GPIO 23

## Important

GPIO 15 is shared by both `NOTA` and `RESET` in the current sketch.
Assign one of them to a different GPIO to avoid input conflicts.
