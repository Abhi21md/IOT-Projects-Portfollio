SYSTEM OVERVIEW
----------------

The Smart Village IoT System integrates:

1) Smart Street Light Automation
2) Garbage Level Monitoring with Alert

-------------------------------
SMART STREET LIGHT WORKING
-------------------------------

• LDR detects darkness.
• If dark:
   - IR sensors detect movement.
   - Only detected street lamps turn ON.
   - Auto OFF after 3 seconds of no movement.
• If daylight:
   - All lamps remain OFF.

This saves electricity by lighting only required zones.

-------------------------------
GARBAGE MONITORING WORKING
-------------------------------

• Ultrasonic sensor measures bin distance.
• Distance mapped to fill percentage.
• Data displayed on:
   - LCD
   - Blynk App (V0)
• If level ≥ 80%:
   - Buzzer activates
   - Blynk event triggered

-------------------------------
EXTRA FEATURE
-------------------------------

• Servo gate example included for future
  automatic waste control expansion.
