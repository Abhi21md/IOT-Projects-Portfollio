SYSTEM OVERVIEW
----------------

This project monitors AC voltage using a ZMPT101B voltage sensor module
connected to an ESP32. The RMS voltage value is calculated and sent
to the Blynk IoT cloud for real-time monitoring.

-----------------------------------
WORKING PRINCIPLE
-----------------------------------

1) ZMPT101B senses AC voltage.
2) ESP32 reads analog waveform samples.
3) DC offset is removed.
4) RMS voltage is calculated.
5) Voltage is sent to Blynk dashboard.
6) System displays AC status:
     - AC OFF
     - AC ON - SAFE
     - AC ON - UNSAFE

-----------------------------------
STATUS LOGIC
-----------------------------------

Voltage < 15V → AC OFF
180V – 260V → SAFE
Outside Range → UNSAFE

-----------------------------------
FEATURES
-----------------------------------

• RMS voltage calculation
• Offset auto-calibration at startup
• Real-time Blynk dashboard display
• Serial monitor debugging
• Noise filtering below 15V
