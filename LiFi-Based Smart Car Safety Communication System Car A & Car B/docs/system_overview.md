# LiFi-Enabled Smart Car Safety Communication System

System Type: Dual Module IoT Safety Vehicle  
Communication: LiFi + WiFi  
Boards Used: ESP8266 + Arduino/ESP (Receiver)

--------------------------------------------------

## Architecture

Car-A (Transmitter + Controller)
• WiFi Controlled via Blynk
• MQ3 Smoke Detection
• Motor Control
• Safety Lock System
• Sends LiFi signal patterns

Car-B (Receiver)
• LDR Light Receiver
• Frequency Analysis
• Pattern Classification
• LCD Alert Display

--------------------------------------------------

## Features

### Car-A
✔ Manual WiFi control
✔ Speed control
✔ Smoke-based ignition lock
✔ Blynk alert event logging
✔ LiFi pattern transmission

### Car-B
✔ Real-time frequency sampling
✔ Alcohol detection pattern
✔ Accident detection pattern
✔ 2-second confirmation logic
✔ 3-second accident latch display
✔ Idle detection mode

--------------------------------------------------

## LiFi Frequency Patterns

6–10 Hz     → Alcohol Detected  
10–25 Hz    → Accident Alert  
Else        → No Signal / Idle

--------------------------------------------------

## Safety Logic

If smoke > threshold:
• Vehicle locks immediately
• Blynk event triggered
• Movement blocked

If accident pattern confirmed:
• Alert displayed for 3 seconds
• Confirmation required for reliability
