# System Overview

Project Name: Night Surveillance Robo Car  
Board: NodeMCU (ESP8266)  
Platform: Blynk IoT  

--------------------------------------------------

## Objective

To develop a WiFi-controlled surveillance robot with:

• Manual remote control
• Auto patrol mode
• Adjustable speed
• Time-based movement control

--------------------------------------------------

## Operating Modes

1. Manual Mode
   - Controlled via Blynk buttons
   - Real-time directional control

2. Auto Surveillance Mode
   - Forward movement for defined time
   - Stop
   - Backward movement for same time
   - Continuous loop

--------------------------------------------------

## Control Architecture

Blynk App → WiFi → NodeMCU → Motor Driver → DC Motors

--------------------------------------------------

## Core Logic

• Time-based auto patrol
• Mode switching using virtual pins
• PWM-based speed control
• Continuous loop execution

--------------------------------------------------

## Status

✔ Fully functional  
✔ Manual and Auto mode verified  
✔ Stable WiFi connection  
✔ No unexpected resets  
