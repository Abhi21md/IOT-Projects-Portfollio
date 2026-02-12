TEST PROCEDURE
----------------

1. Power ON system.
2. Check Serial Monitor for readings.

Expected Output:
TDS: ___ ppm
Temp: ___ °C

3. Verify Blynk dashboard updates:
   - V0 → TDS
   - V1 → Temperature

4. Trigger test alert:
   - Dip TDS probe in salty water
   OR
   - Heat temperature sensor

Expected:
   - Buzzer ON
   - Blynk alert notification

5. When values return normal:
   - Buzzer OFF
