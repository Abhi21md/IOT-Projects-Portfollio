# RFID Door Lock Security System

Board: Arduino UNO
Authentication: MFRC522 RFID Module
Display: 16x2 I2C LCD
Lock Mechanism: Servo Motor
Indicators: Green LED, Red LED
Alert: Buzzer

--------------------------------------------------

## Working Principle

1. User places RFID card near reader.
2. UID is read and compared with stored authorized UIDs.
3. If match found:
   • Green LED ON
   • Access Granted message
   • Servo unlocks door
   • Buzzer success tone
4. If not authorized:
   • Red LED ON
   • Access Denied message
   • Long buzzer alert

After operation, system resets to standby mode.

--------------------------------------------------

## Features

✔ UID based authentication
✔ Multiple authorized card support
✔ Visual feedback via LCD
✔ Audio feedback via buzzer
✔ Servo-based locking mechanism
✔ LED status indication
✔ Secure access logic
✔ Expandable UID list

--------------------------------------------------

## Security Logic

• Only predefined UIDs allowed
• Automatic relock after 3 seconds
• Invalid access alert
