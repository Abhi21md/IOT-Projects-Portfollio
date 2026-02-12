# Smart LPG Safety System – Fixed Threshold Edition

Board: NodeMCU ESP8266  
Sensor: MQ2 Gas Sensor  
Actuator: Servo-based Regulator Control  
Cloud: Blynk IoT  
Display: 16x2 I2C LCD (Custom SDA/SCL)

--------------------------------------------------

## Project Overview

This system monitors LPG gas levels using a fixed safety threshold.

If gas level crosses the predefined limit:

• Buzzer activates instantly  
• Cylinder regulator closes automatically  
• Red alert LED triggers in Blynk  
• LCD displays emergency warning  

Unlike the advanced version, this system:

✔ Uses a fixed calibrated safety threshold  
✔ Optimized for faster response  
✔ Lightweight cloud communication  
✔ Custom I2C pin configuration  
✔ Clean simplified control logic  

--------------------------------------------------

## Key Differentiators

This version is designed for:

• Low-resource environments  
• Faster response time  
• Reduced cloud data load  
• Simple deploy-and-run safety system  
• Custom I2C wiring flexibility  

--------------------------------------------------

## System Logic

Gas Value > Threshold:
→ Buzzer ON  
→ Servo closes regulator (140°)  
→ Cloud alert triggered  

Gas Value < Threshold:
→ Buzzer OFF  
→ Servo opens regulator (0°)  
→ System returns to safe mode  

Manual override available via Blynk.
