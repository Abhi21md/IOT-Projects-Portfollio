SYSTEM OVERVIEW
----------------

This project implements a standalone biometric authentication system
using an Arduino board and an optical fingerprint sensor.

WORKING FLOW:

1. System initializes fingerprint sensor.
2. LCD displays system status.
3. User places finger on sensor.
4. Fingerprint image is captured.
5. Template is generated and matched.
6. If match found:
      - LCD shows "Access Granted"
      - Relay activates
      - Buzzer sounds
7. If match not found:
      - LCD shows "Access Denied"
      - Short buzzer alert

KEY FEATURES:

• Fast fingerprint matching
• ID-based authentication
• Relay-controlled output
• LCD status display
• Fully standalone system

APPLICATIONS:

• Smart door lock
• Biometric attendance
• Locker system
• Secure access control
