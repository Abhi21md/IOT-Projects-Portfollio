# Smart Vehicle Lock & Ignition Security System

Board: NodeMCU (ESP8266)
GPS Module: NEO-6M
Cloud: Blynk IoT
Alert System: Telegram Bot
Display: 16x2 I2C LCD + Blynk LCD Widget

--------------------------------------------------

## Project Overview

This project is a smart vehicle security system that:

• Controls lock/unlock remotely via Blynk
• Controls ignition via cloud or physical button
• Blocks ignition if vehicle is locked
• Sends live GPS location to Telegram
• Displays status on LCD + Cloud widget

--------------------------------------------------

## Core Security Logic

1. If vehicle is UNLOCKED:
   → Ignition allowed
   → Relay ON
   → Single confirmation buzzer

2. If vehicle is LOCKED:
   → Ignition blocked
   → Warning buzzer activated
   → GPS location captured
   → Telegram alert sent
   → LCD displays alert message

--------------------------------------------------

## Unique Feature: Smart GPS Fallback

If GPS fails to get satellite fix within 10 seconds:
• System generates realistic fallback coordinates
• Alert is still sent
• Prevents location failure during emergency

--------------------------------------------------

## System Components

✔ Cloud Lock Control (V1)
✔ Cloud Ignition Control (V2)
✔ Physical Ignition Button
✔ Relay Engine Control
✔ GPS Live Location
✔ Telegram Alert Integration
✔ Dual LCD Display (Hardware + App)
✔ Anti-Theft Ignition Block
