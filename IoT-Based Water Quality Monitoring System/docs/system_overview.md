SYSTEM OVERVIEW
----------------

This project monitors water quality using:

• TDS (Total Dissolved Solids) sensor
• DS18B20 temperature sensor
• Blynk IoT dashboard
• Buzzer alert system

WORKING PRINCIPLE:

1. DS18B20 reads water temperature.
2. TDS sensor reads dissolved solids level.
3. ESP8266 processes sensor values.
4. Data is sent to Blynk cloud.
5. If TDS > 500 ppm OR Temp > 32°C:
      → Buzzer activates
      → Blynk notification sent

DATA FLOW:

Sensors → ESP8266 → Blynk Cloud → Mobile App

APPLICATIONS:

• Drinking water monitoring
• RO water quality check
• Tank water monitoring
• Smart home water system
• Industrial water monitoring
