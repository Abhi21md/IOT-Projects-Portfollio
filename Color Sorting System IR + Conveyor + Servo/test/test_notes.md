TESTING PROCEDURE
-----------------

1. Upload firmware to ESP32.
2. Open Serial Monitor at 115200 baud.

STEP 1: IR SENSOR TEST
• Place object in front of IR sensor.
• Confirm object detection message appears.

STEP 2: COLOR SENSOR TEST
• Place Red object.
• Check frequency values.
• Confirm "Detected: RED".

Repeat for:
• Green object
• Blue object

STEP 3: SERVO TEST
• On detection, corresponding servo should rotate.
• Servo must return to neutral position after 500 ms.

EXPECTED RESULT:
Each colored object is diverted into correct bin.
