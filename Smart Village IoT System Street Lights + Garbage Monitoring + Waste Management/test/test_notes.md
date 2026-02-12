TEST PROCEDURE
----------------

1) Street Light Test
   - Cover LDR to simulate darkness.
   - Move hand in front of IR sensors.
   - Respective lamp should turn ON.
   - After 3 seconds without motion → OFF.

2) Daylight Test
   - Expose LDR to light.
   - Lamps should remain OFF.

3) Garbage Monitoring Test
   - Place object inside bin.
   - LCD should show updated percentage.
   - Blynk gauge should update.
   - At ≥ 80% → buzzer + Blynk event.

4) Network Test
   - Disconnect WiFi.
   - System should still run locally.

5) Alert Reset
   - Remove object.
   - Buzzer should turn OFF.
