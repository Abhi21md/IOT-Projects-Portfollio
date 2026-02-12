# Paralyzed Patient Monitoring & Assistance System

Board: Arduino Nano
Sensors: 5 Flex Sensors
Audio: DFPlayer Mini + Speaker
Display: 16x2 I2C LCD

--------------------------------------------------

## System Purpose

This system helps paralyzed or bedridden patients communicate
basic needs using hand gestures.

Each finger bend represents a predefined message.

When a gesture is detected:
• LCD displays the message
• Voice message is played through speaker

--------------------------------------------------

## Gesture Mapping

Thumb Bent        → Need Water
Index Bent        → Need Help
Middle Bent       → I am Hungry
Ring Bent         → Call Nurse
Little Finger Bent → Need Medicine

--------------------------------------------------

## Working Principle

1. Flex sensors measure finger bending.
2. If sensor value exceeds threshold:
   - System identifies gesture
   - Displays message on LCD
   - Plays corresponding voice file
3. System waits for next gesture.

--------------------------------------------------

## Core Features

✔ Real-time gesture detection
✔ LCD message display
✔ Voice output via DFPlayer
✔ Simple threshold-based logic
✔ Low cost hardware
✔ Standalone operation
✔ Easy customization of messages

