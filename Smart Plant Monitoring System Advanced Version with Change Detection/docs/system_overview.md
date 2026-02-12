# System Architecture

NodeMCU ESP8266
        │
        ├── DHT11 → Temperature & Humidity
        ├── Soil Sensor → Moisture %
        ├── PIR Sensor → Motion detection
        ├── Relay → Water pump or grow light
        ├── Push Button → Manual toggle
        ├── LCD → Real-time status
        └── Blynk Cloud → Remote monitoring

------------------------------------------------------------

Event Flow

Temperature/Humidity:
• Checked every 15 seconds
• Sent only if change > threshold

Soil Moisture:
• Checked every 10 seconds
• Sent only if value changed

Motion:
• If enabled → sends event log
• LED widget activated
• Blynk notification triggered

Relay:
• Can be controlled from:
    - Mobile app
    - Physical push button
