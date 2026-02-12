TEST PROCEDURE
----------------

1. Power ON ESP32.
2. Serial monitor should show:
   "Density Based Traffic Control Started"

3. Place object in front of any ultrasonic sensor.
4. Ensure distance becomes < 20 cm.

Expected Result:
→ That road gets YELLOW (1 sec)
→ Then GREEN (5 sec)
→ Then back to RED

5. If no object within threshold:
→ All roads stay RED
→ Serial prints "No Traffic Detected"

6. Test each road individually.
7. Test multiple roads simultaneously.
→ Closest one should get priority.
