# Advanced Smart Plant Monitoring & Protection System

Board: NodeMCU (ESP8266)
Cloud: Blynk IoT
Sensors: DHT11 + Soil Moisture + PIR Motion
Display: 16x2 I2C LCD (PCF8574)

------------------------------------------------------------

## Project Overview

This system monitors plant health and adds intelligent protection features.

Unlike a basic monitoring system, this version includes:

• Change-detection logic to reduce cloud traffic
• PIR-based intrusion detection
• Physical + Cloud relay control
• Motion event logging to Blynk
• LCD multi-status display
• Manual override button

------------------------------------------------------------

## Core Features

• Temperature & Humidity Monitoring (DHT11)
• Soil Moisture Percentage Monitoring
• PIR Motion Detection for Plant Protection
• Relay control (Water pump / Light)
• Physical Push Button Override
• Blynk LED status indicators
• Event-based cloud updates (no spam updates)
• LCD live status dashboard

------------------------------------------------------------

## Intelligent Design Elements

• Sends sensor data only when values change significantly
• Prevents unnecessary Blynk data flooding
• Motion toggle enable/disable from mobile app
• Syncs relay state on reconnection
• Dual control (Physical + IoT)

------------------------------------------------------------

## Blynk Virtual Pins

V10 → Temperature  
V11 → Humidity  
V12 → Soil Moisture  
V13 → Motion LED Indicator  
V14 → Relay Control  
V15 → PIR Enable Toggle  

------------------------------------------------------------

## Why This Version Is Advanced

This is not just a sensor reader.

It implements:

• Data optimization logic
• Event-driven updates
• Intrusion alert system
• Cloud synchronization
• Hybrid hardware + IoT control

------------------------------------------------------------

Applications

• Smart greenhouse
• Balcony plant protection
• Farm intrusion monitoring
• Smart home gardening
