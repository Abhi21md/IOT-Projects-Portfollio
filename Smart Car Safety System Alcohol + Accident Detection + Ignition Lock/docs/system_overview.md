# LiFi Based Smart Car Safety Communication System

Board:
Car-A → NodeMCU (ESP8266)
Car-B → Arduino / NodeMCU (Receiver)

Communication:
LiFi (Light based pulse communication using LED + LDR)

------------------------------------------------------------

## Project Concept

Two vehicles communicate using light pulses.

Car-A (Rear Vehicle):
• Detects Alcohol using MQ sensor
• Detects Accident using MPU6050
• Sends encoded light pulse pattern

Car-B (Front Vehicle):
• Receives light pulses using LDR
• Decodes frequency pattern
• Displays alert on LCD

No RF.
No Bluetooth.
Pure Light Communication.

------------------------------------------------------------

## Car-A (Transmitter Vehicle)

### Sensors:
• MPU6050 → Accident Detection
• Alcohol Sensor → Drunk driving detection

### Alerts:
• Alcohol → Continuous 8Hz blinking pattern
• Accident → 20Hz fast flicker for 3.5 seconds

### Actions:
• Ignition lock
• Buzzer alert
• Blynk cloud logging
• LED pulse transmission (LiFi)

------------------------------------------------------------

## Car-B (Receiver Vehicle)

### Receiver:
• LDR detects light pulse changes
• Counts pulses per second
• Calculates frequency

### Pattern Decoding:

6–10 Hz  → Alcohol Detected  
10–25 Hz → Accident Alert (2 sec confirmation)  

Displays message on LCD.

------------------------------------------------------------

## Smart Features

✔ Dual vehicle communication  
✔ Light-based emergency warning  
✔ Frequency pattern encoding  
✔ 2-second accident confirmation logic  
✔ 3-second latch display  
✔ Continuous alcohol alert detection  
✔ No RF interference  
✔ Works in line-of-sight  

------------------------------------------------------------

Applications

• Smart highway safety
• Convoy vehicle alerting
• Accident early warning system
• LiFi research demonstration
• Smart transport prototype
