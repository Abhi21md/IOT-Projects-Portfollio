SMART STREET LIGHT SYSTEM – SYSTEM OVERVIEW
--------------------------------------------

This project implements an automatic street lighting system using:

• 1 LDR sensor (ambient light detection)
• 3 IR motion sensors (vehicle detection)
• 3 street lamp outputs (LEDs or relays)
• Arduino UNO controller

WORKING PRINCIPLE:

1. During daytime:
   - LDR detects bright light.
   - All lamps remain OFF.

2. During nighttime:
   - LDR detects darkness.
   - IR sensors monitor vehicle movement.

3. When motion is detected:
   - Corresponding lamp turns ON.
   - Lamp stays ON for 2 seconds.
   - Turns OFF automatically if no further motion.

FEATURES:

• Energy efficient lighting
• Motion-based activation
• Auto shut-off timer
• Serial Monitor debugging
• Expandable for multiple street sections

APPLICATIONS:

• Smart highways
• Village roads
• Campus roads
• Smart city infrastructure
