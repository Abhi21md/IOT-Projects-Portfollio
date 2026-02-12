TEST PROCEDURE
----------------

1) Power ON the system.
2) LCD should display "Mine Monitoring System Ready".

3) Test Gas Sensor:
   - Bring lighter gas near MQ2.
   - Gas value should increase.
   - Buzzer + LED should turn ON.
   - Blynk should trigger gas_alert event.

4) Test RFID:
   - Scan authorized card.
   - Worker name should display.
   - Blynk should log worker_entry.

5) Scan unknown card:
   - LCD shows Unknown Worker.
   - worker_alert event triggered.

6) Verify temperature & humidity values on Blynk dashboard.
