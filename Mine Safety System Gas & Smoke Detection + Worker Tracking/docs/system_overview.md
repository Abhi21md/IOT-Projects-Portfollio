SYSTEM OVERVIEW
----------------

This project monitors mine worker safety using:

• MQ2 Gas Sensor (Hazardous Gas Detection)
• DHT11 (Temperature & Humidity)
• RFID Module (Worker Identification)
• Buzzer + Alert LED (Emergency Alert)
• LCD Display (Local Monitoring)
• Blynk IoT Dashboard (Remote Monitoring)

-------------------------------------

WORKING PRINCIPLE
-------------------------------------

1) MQ2 continuously monitors gas levels.
2) DHT11 monitors temperature & humidity.
3) If gas exceeds threshold → alert triggered.
4) RFID detects worker entry.
5) Worker name displayed on LCD.
6) Data sent to Blynk Cloud.
7) Alerts generated in dangerous conditions.

-------------------------------------

SAFETY LOGIC
-------------------------------------

Gas > 400 → Danger Alert
Unknown RFID → Worker Alert
Normal Condition → Monitoring Mode

-------------------------------------

FEATURES
-------------------------------------

✔ Real-time Gas Monitoring
✔ Temperature & Humidity Monitoring
✔ Worker Entry Logging
✔ Cloud Alert Notification
✔ LCD Display Output
✔ Emergency Buzzer + LED
