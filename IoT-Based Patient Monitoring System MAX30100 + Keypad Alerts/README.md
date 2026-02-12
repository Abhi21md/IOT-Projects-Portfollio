# 🏥 IoT Patient Monitoring & Smart Assistance System

NodeMCU-based real-time patient monitoring system with heart rate, SpO₂ tracking, keypad emergency messaging, LCD display, buzzer alerts, and Blynk IoT cloud integration.

---

## 📌 Overview

This project monitors a patient’s vital signs using the MAX30100 pulse oximeter sensor and sends live data to the Blynk IoT dashboard.

In addition to health monitoring, the system allows the patient to send predefined assistance messages using a 4x4 keypad.

It combines:

- Real-time BPM monitoring
- Real-time SpO₂ monitoring
- Cloud dashboard integration
- Keypad-based assistance system
- Emergency alert feature
- LCD live display

This makes it a complete assistive healthcare IoT solution.

---

## 🔥 Features

### ❤️ Vital Monitoring
- Heart Rate (BPM) measurement
- Oxygen Saturation (SpO₂) monitoring
- 1-second live updates
- Automatic invalid reading handling
- Data sent to Blynk cloud

### 📟 Patient Assistance Messages

| Key | Message |
|-----|----------|
| 1 | I want water |
| 2 | I need medicine |
| 3 | I am in pain |
| 4 | Help me |
| 5 | I feel sick |
| 6 | I need assistance |

Messages are:
- Displayed on LCD
- Sent to Blynk app
- Logged on Serial Monitor

---

### 🚨 Emergency Alert

Press `*` key:
- Triggers buzzer
- Displays alert on LCD
- Sends Blynk event notification

---

## 🛠 Hardware Components

- NodeMCU (ESP8266)
- MAX30100 Pulse Oximeter Sensor
- 16x2 I2C LCD
- 4x4 Matrix Keypad
- Buzzer
- Jumper wires
- Power supply

---

## 🔌 Pin Configuration

### MAX30100 (I2C)
- SDA → D2
- SCL → D1

### LCD (I2C)
- Address → 0x27
- Shared I2C bus

### Keypad
- Rows → D3, D4, D5, D6
- Columns → D7, D8, D0, A0

### Buzzer
- D0

---

## 📲 Blynk Virtual Pins

- V0 → Patient Message
- V1 → Heart Rate (BPM)
- V2 → SpO₂

---

## ⚙ Working Principle

1. MAX30100 continuously measures BPM and SpO₂.
2. Data updates every second.
3. Values are displayed on LCD and sent to Blynk.
4. Patient can send predefined messages using keypad.
5. Emergency key triggers notification and buzzer alert.
6. System handles invalid readings automatically.

---

## 🧪 System Testing

✔ Stable heart rate detection  
✔ Accurate SpO₂ readings  
✔ Emergency notification working  
✔ Keypad message system verified  
✔ Continuous runtime stability tested  

---

## 🚀 Applications

- Hospital patient monitoring
- Home healthcare systems
- Assisted living environments
- Remote patient monitoring
- IoT healthcare research projects

---

## 👨‍💻 Author

Abhishek Madiwal  
IoT & Embedded Systems Developer
