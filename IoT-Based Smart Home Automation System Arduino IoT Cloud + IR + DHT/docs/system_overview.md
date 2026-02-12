# Smart Home Automation System (Arduino IoT Cloud)

Board: ESP32  
Platform: Arduino IoT Cloud  
Sensors: IR Motion Sensor + DHT11  
Control: 5 Relays (Lights + Temperature Control)

------------------------------------------------------

## Project Overview

This project is a cloud-connected smart home automation system
that supports:

1. IR-based automatic hall light control
2. Manual cloud control of 4 relays
3. Temperature-based automatic relay switching
4. Real-time cloud monitoring

------------------------------------------------------

## Functional Features

### 1️⃣ IR Motion Detection (Hall Light)

• IR sensor detects motion
• Automatically turns ON Hall Light
• Turns OFF when disabled from cloud
• Prevents repeated triggering using logic flag

------------------------------------------------------

### 2️⃣ Multi Relay Cloud Control

Relay 1 → Hall Light (IR + Cloud)
Relay 2 → Appliance 2
Relay 3 → Appliance 3
Relay 4 → Appliance 4
Relay 5 → Temperature Control Relay

All relays can be controlled via Arduino IoT Cloud dashboard.

------------------------------------------------------

### 3️⃣ Temperature-Based Automation

• DHT11 sensor reads temperature
• If temperature > 30°C:
    → Relay 5 turns ON automatically
• If temperature < 30°C:
    → Relay 5 turns OFF

Fully automatic climate-triggered control.

------------------------------------------------------

## Smart Features

✔ Motion-triggered lighting  
✔ Cloud-based manual override  
✔ Temperature auto-control  
✔ Real-time cloud sync  
✔ Multi-relay expansion ready  
✔ IoT-enabled remote monitoring  
