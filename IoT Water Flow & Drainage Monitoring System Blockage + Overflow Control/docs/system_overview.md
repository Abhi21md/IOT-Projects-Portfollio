# IoT Water Flow & Drainage Monitoring System

Board: NodeMCU ESP8266
Cloud Platform: Blynk IoT
Relay Type: Active LOW
Sensors: Flow Sensor + Soil Moisture
Actuator: Water Pump

--------------------------------------------------

## System Purpose

This system monitors:

• Real-time water flow rate
• Total water consumption
• Soil moisture level
• Drain blockage condition
• Overflow detection

It automatically controls a pump based on moisture and flow conditions,
while allowing manual override from Blynk.

--------------------------------------------------

## Core Features

✔ Real-time flow rate monitoring (L/min)
✔ Total water usage calculation
✔ Soil moisture filtering
✔ Auto pump control
✔ Manual override via Blynk
✔ Active LOW relay logic
✔ Blockage detection with hysteresis
✔ Auto-unblock mechanism
✔ Overflow detection
✔ Flow smoothing algorithm
✔ Calibration slider via Blynk
✔ Alert cooldown system
✔ Non-false triggering logic

--------------------------------------------------

## Smart Logic Used

• Weighted moving average filtering
• Flow hysteresis (Low + High threshold)
• Moisture averaging filter
• Auto pump cooldown timer
• Anti-spam alert logic
• Interrupt-based pulse counting
