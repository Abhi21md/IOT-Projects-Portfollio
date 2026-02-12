SYSTEM OVERVIEW
----------------

This project detects:

• LPG / Methane / Smoke (via MQ2)
• Fire / Flame (via IR Flame Sensor)

The system continuously monitors gas concentration and flame presence.

If any dangerous condition is detected:
→ Buzzer activates
→ LED turns ON
→ LCD displays alert message

-------------------------------------

WORKING PRINCIPLE
-------------------------------------

1) MQ2 reads analog gas value.
2) Flame sensor detects IR flame (LOW = flame detected).
3) If gas > threshold OR flame detected:
     → Alert triggered
4) Otherwise:
     → Safe Environment message shown

-------------------------------------

SAFETY LOGIC
-------------------------------------

Condition 1:
Gas Value > 400 → GAS LEAK ALERT

Condition 2:
Flame Detected → FLAME ALERT

Condition 3:
Normal Range → SAFE ENVIRONMENT

-------------------------------------

FEATURES
-------------------------------------

✔ Real-time Gas Monitoring
✔ Flame Detection
✔ LCD Display Status
✔ Buzzer Alert
✔ LED Warning Indicator
✔ Simple & Reliable Design
