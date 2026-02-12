TESTING PROCEDURE
-----------------

1. Upload code to Arduino UNO.
2. Open Serial Monitor (9600 baud).
3. Observe LDR values.

DAY TEST:
• Provide light to LDR.
• Lamps should remain OFF.

NIGHT TEST:
• Cover LDR to simulate darkness.
• Move object in front of IR sensor.
• Corresponding lamp should turn ON.
• Lamp should turn OFF after 2 seconds.

SERIAL DEBUG:
• Monitor LDR value.
• Monitor IR sensor status.
• Monitor lamp states.

EXPECTED RESULT:
System activates lights only in darkness and only when motion is detected.
