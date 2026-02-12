SYSTEM OVERVIEW
----------------

The Smart Library Book Finder Robot is designed
to automatically locate a selected book rack
based on slot number input from the Serial Monitor.

WORKING PRINCIPLE:

1. User enters rack number (1–10).
2. Robot starts moving forward.
3. IR sensor detects rack markers (black lines).
4. Each detected line increments rack count.
5. When count matches target slot:
   → Robot stops.
   → LED + Buzzer activate.
6. System waits for next input.

FEATURES:

• Serial-based slot selection
• Line/rack detection using IR sensor
• Automatic stopping at target slot
• LED indication for location found
• Audible buzzer alert
• Motor driver speed control
• Debounce protection for accurate counting
