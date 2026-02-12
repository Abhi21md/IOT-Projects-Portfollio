# Smart Irrigation System (Auto + Manual Mode)

Board: NodeMCU (ESP8266)
Platform: Blynk IoT
Sensors: Soil Moisture + DHT11
Output: Relay Controlled Water Pump
Display: 16x2 I2C LCD

------------------------------------------------------

## Project Overview

This project is an IoT-based smart irrigation system that
automatically controls a water pump based on soil moisture level.

It also supports:
• Manual pump control from Blynk
• Auto/Manual mode switching
• Temperature and humidity monitoring
• LCD real-time display
• Live cloud dashboard monitoring

------------------------------------------------------

## Working Logic

### 🌱 Soil Moisture Control

• Soil sensor gives percentage (0–100%)
• If soil ≥ 90% → Pump ON
• If soil < 90% → Pump OFF
• Works automatically in AUTO mode

------------------------------------------------------

### 🔁 Auto + Manual Mode

AUTO MODE:
• Pump controlled by soil condition
• Manual button disabled

MANUAL MODE:
• Pump controlled from Blynk button
• Soil reading displayed but does not control motor

------------------------------------------------------

### 🌡 Environmental Monitoring

• DHT11 reads temperature and humidity
• Temperature and humidity displayed on LCD
• Sent to Blynk gauges (V1 & V2)

------------------------------------------------------

### 📟 LCD Display

Line 1:
T:Temperature  H:Humidity

Line 2:
Soil:XX%  Pump ON/OFF  Mode (A/M)

------------------------------------------------------

## Smart Features

✔ Soil percentage mapping  
✔ Auto + Manual switching logic  
✔ Cloud sync with Blynk  
✔ LCD real-time monitoring  
✔ HIGH relay logic control  
✔ Stable non-blocking design  
