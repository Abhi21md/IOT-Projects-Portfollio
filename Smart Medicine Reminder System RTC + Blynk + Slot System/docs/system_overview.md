# System Architecture

NodeMCU (ESP8266)
        │
        ├── DS3231 RTC → Time accuracy
        ├── 16x2 LCD → Display current & next alarm
        ├── Buzzer → Audible alert
        ├── 3 Slot LEDs → Morning / Afternoon / Evening
        └── Blynk Cloud → Remote notification & control

------------------------------------------------------------

Alarm Workflow

1. RTC checks time every 1.5 seconds
2. If current time == alarm time:
      → Buzzer ON
      → Slot LED ON
      → LCD alert message
      → Blynk event sent
3. Alert stays active until:
      → Stop button pressed
      OR
      → Blynk V3 pressed

------------------------------------------------------------

Anti-Spam Logic

• Blynk notifications minimum 30 seconds apart
• LCD alert screen frozen for 5 seconds
• alertGiven flag prevents repeat triggering
