# Smart Plant Monitoring System

Board: NodeMCU (ESP8266)
Platform: Blynk IoT Cloud
Sensors: Soil Moisture + DHT11 + PIR Motion
Output: Relay Controlled Device
Display: 16x2 I2C LCD

------------------------------------------------------

## Project Overview

This IoT-based plant monitoring system tracks:

• Soil moisture percentage
• Temperature
• Humidity
• Motion detection near plant
• Relay control (manual + physical button)

All parameters are displayed locally on LCD
and remotely on Blynk IoT dashboard.

------------------------------------------------------

## Working Logic

### 🌡 Temperature & Humidity

• DHT11 reads temperature and humidity
• Updates Blynk only when change is significant
• LCD displays:
  T: Temperature (°C)
  H: Humidity (%)

------------------------------------------------------

### 🌱 Soil Moisture Monitoring

• Analog soil sensor mapped to 0–100%
• Inverted logic for correct moisture %
• Sent to Blynk gauge (V12)
• Displayed on LCD

------------------------------------------------------

### 🚶 PIR Motion Detection

• PIR detects motion near plant
• If enabled from Blynk:
   - Sends cloud event
   - Turns ON Blynk LED widget
• Can be enabled/disabled remotely

------------------------------------------------------

### 💡 Relay Control

• Controlled via:
   - Physical push button
   - Blynk virtual button (V14)

• State sync on reconnect
• LCD shows W:ON / W:OFF

------------------------------------------------------

## Smart Features

✔ Change-based data update (reduces cloud load)
✔ Motion event logging
✔ Cloud + physical control sync
✔ PIR remote enable/disable
✔ LCD multi-parameter display
✔ Stable timer-based sensor polling
✔ Dual control architecture (Cloud + Hardware)

------------------------------------------------------

Applications

• Smart garden monitoring
• Indoor plant automation
• Balcony plant protection
• Motion-based plant security
• IoT environmental monitoring
