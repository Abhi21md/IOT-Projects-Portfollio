TEST PROCEDURE
----------------

1. Power ON system.
   LCD should display:
   "Fingerprint Initializing..."
   Then:
   "Sensor Ready"

2. Place registered fingerprint.
   Expected:
   - LCD: Access Granted
   - Relay ON for 2 seconds
   - Buzzer ON

3. Place unregistered fingerprint.
   Expected:
   - LCD: Access Denied
   - Short buzzer sound

4. Verify relay switching using LED or multimeter.

5. Ensure stable 5V supply to fingerprint sensor.
