DUAL MCU NIGHT SURVEILLANCE ROBO SYSTEM
---------------------------------------

This project is an advanced surveillance and defense robotic system using multiple microcontrollers.

SYSTEM MODULES:

1) MOBILE ROBO CAR (NodeMCU ESP8266)
   - Manual direction control via Blynk
   - Adjustable speed control
   - Auto patrol mode (Forward ↔ Backward)
   - Metal detection safety lock
   - Forward movement blocked if metal detected

2) RADAR DEFENSE MODULE
   - Dual ultrasonic sensors (Left + Right)
   - Horizontal and vertical servo scanning
   - Laser beam activation on detection
   - Buzzer alert
   - Lock-on mode while object present

3) ESP32-CAM MODULE
   - Live video streaming
   - Night surveillance monitoring
   - Remote access through browser

WORKING PRINCIPLE:

• Car patrols manually or automatically.
• Metal detector prevents forward/back motion for safety.
• Radar continuously scans left and right.
• If object detected within threshold:
  - Laser activates
  - Buzzer sounds
  - Servo locks on target
• Live feed monitored via ESP32-CAM.

APPLICATIONS:
- Defense surveillance robot
- Border security prototype
- Night monitoring system
- Hazard detection rover
