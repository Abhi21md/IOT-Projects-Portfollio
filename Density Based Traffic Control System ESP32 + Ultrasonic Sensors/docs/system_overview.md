SYSTEM OVERVIEW
----------------

This project is a smart traffic management system
based on real-time vehicle density detection.

Each road is monitored using an ultrasonic sensor.

WORKING PRINCIPLE:

1. Ultrasonic sensors measure vehicle distance.
2. Shorter distance = higher vehicle density.
3. System compares all 4 roads.
4. Road with minimum distance gets green signal.
5. Others remain red.
6. Yellow signal shown before green transition.

If no vehicle is detected within threshold:
→ All signals remain RED.

FEATURES:

• Fully automatic traffic control
• Density-based signal allocation
• 4-road intersection model
• Dynamic priority switching
• Serial monitoring for debugging
