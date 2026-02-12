TEST PROCEDURE
----------------

1. Power ON the system.
2. Open Serial Monitor (9600 baud).
3. Enter slot number (1–10).
4. Robot should start moving forward.
5. Each rack crossing should print:
   "Rack Passed: X"

6. When rack count equals target slot:
   → Robot stops.
   → LED ON.
   → Buzzer ON for 3 seconds.

7. After alert:
   → Serial asks for next slot.

Edge Case Tests:

• Enter 0 or >10 → Should not move.
• Rapid rack detection → Debounce prevents double count.
• IR sensor failure → Robot continues until manually stopped.
