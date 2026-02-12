# IoT-Based Smart Medicine Reminder System

Board: NodeMCU (ESP8266)
RTC: DS3231
Cloud: Blynk IoT
Display: 16x2 I2C LCD

------------------------------------------------------------

## Project Overview

This system reminds users to take medicine at a specific time
using real-time clock synchronization and IoT notification.

It includes:

• RTC-based accurate time tracking
• Morning / Afternoon / Evening slot detection
• LCD alert display
• Buzzer notification
• Blynk mobile notification
• Manual stop button (physical + cloud)
• Anti-notification spam logic

------------------------------------------------------------

## Core Features

• DS3231 Real Time Clock based scheduling
• Custom alarm hour and minute setting
• Slot-based LED indication:
  - Morning (6AM–12PM)
  - Afternoon (12PM–6PM)
  - Evening (6PM–10PM)
• 5-second alert freeze screen
• 30-second Blynk notification cooldown
• Manual alarm stop via:
  - Physical SET button
  - Blynk V3 button
• Setup mode (Long press SET button)

------------------------------------------------------------

## Blynk Virtual Pins

V0 → Current Time  
V1 → Alarm Time  
V2 → Slot Name  
V3 → Stop Alert Button  

------------------------------------------------------------

## Why This Project Is Advanced

• RTC hardware integration
• Time-slot intelligent classification
• Cloud + Hardware hybrid control
• Anti-spam notification logic
• LCD freeze logic control
• Dual alert stop mechanism

------------------------------------------------------------

Applications

• Elderly care system
• Hospital medicine scheduler
• Home patient monitoring
• Smart healthcare IoT
