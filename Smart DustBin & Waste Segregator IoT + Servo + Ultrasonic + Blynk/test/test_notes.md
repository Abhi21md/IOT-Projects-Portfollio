Smart Dustbin – Testing Notes

Board: NodeMCU (ESP8266)
Platform: Blynk IoT
LCD: 16x2 I2C
Servo: SG90
Sensors:
- 3x Ultrasonic Sensors
- Soil Moisture Sensor

--------------------------------------
1. WiFi & Blynk Connection
--------------------------------------
✔ WiFi connects successfully
✔ Blynk cloud connection established
✔ Waste type updates on V4
✔ Dry level updates on V1
✔ Wet level updates on V2
✔ Full alert message updates on V3
✔ Email + notification triggered using Blynk event (fullbin)

--------------------------------------
2. Waste Detection Test
--------------------------------------
✔ Presence detected under 15 cm
✔ Soil moisture correctly differentiates:
   - Above threshold → WET
   - Below threshold → DRY
✔ Servo rotates to:
   - 180° for wet
   - 0° for dry
✔ Servo returns to 90° neutral position

--------------------------------------
3. Level Monitoring Test
--------------------------------------
✔ Dry ultrasonic measures correctly
✔ Wet ultrasonic measures correctly
✔ Levels mapped to:
   0% / 25% / 50% / 75% / 100%
✔ LCD displays level properly
✔ Alert triggers at
