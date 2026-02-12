SYSTEM OVERVIEW
----------------

This project is an interrupt-based digital counter system
designed for conveyor belt item counting.

WORKING PRINCIPLE:

1. IR / Proximity sensor detects object.
2. External interrupt increments count.
3. User sets target value using SET button.
4. Value stored permanently in EEPROM.
5. When count reaches set value:
      → Relay turns OFF
      → Buzzer turns ON
      → System stops automatically.

FEATURES:

• Interrupt-based fast counting
• EEPROM storage for set value
• Start / Stop / Reset control
• Relay motor control
• Automatic stop at target
• LCD live display
