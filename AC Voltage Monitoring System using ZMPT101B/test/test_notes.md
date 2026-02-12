TEST PROCEDURE
----------------

1) Upload firmware.
2) Keep AC OFF.
   - Voltage should show 0V.
   - Status should show "AC OFF".

3) Turn AC ON (Normal supply).
   - Voltage should display around 220-240V.
   - Status should show "AC ON - SAFE".

4) Simulate low/high voltage (if possible).
   - System should display "UNSAFE".

5) Verify Serial Monitor output.

6) Verify Blynk Dashboard updates every 500ms.
