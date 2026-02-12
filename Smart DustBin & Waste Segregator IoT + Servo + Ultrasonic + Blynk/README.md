# Smart Dustbin & Waste Segregator (IoT Based)

An IoT-enabled automatic smart dustbin that detects garbage, segregates wet and dry waste, monitors bin levels, and sends real-time alerts using Blynk.

------------------------------------------------------

## 🔹 Features

• Automatic garbage detection using ultrasonic sensor  
• Wet/Dry waste segregation using soil moisture sensor  
• Servo-controlled compartment switching  
• Dual ultrasonic level monitoring (Wet + Dry bins)  
• LCD real-time status display  
• IoT monitoring using Blynk Cloud  
• Email + notification alert when bin is 75% full  
• Real-time dashboard monitoring  

------------------------------------------------------

## 🔹 Hardware Used

- NodeMCU (ESP8266)
- 16x2 I2C LCD
- SG90 Servo Motor
- 3x Ultrasonic Sensors
- Soil Moisture Sensor
- Power Supply

------------------------------------------------------

## 🔹 Blynk Dashboard Pins

V1 → Dry Waste Level (%)  
V2 → Wet Waste Level (%)  
V3 → Alert Message  
V4 → Waste Type (WET / DRY)

Event Name Used:
"fullbin" → Email + Push Notification

------------------------------------------------------

## 🔹 Working Principle

1. Garbage presence detected under 15 cm.
2. Soil moisture sensor identifies waste type.
3. Servo rotates to appropriate compartment.
4. Servo resets to neutral position.
5. Every 5 seconds bin levels are checked.
6. If bin reaches 75% or more:
   - Blynk alert triggered
   - Email + notification sent.

------------------------------------------------------

## 🔹 Applications

• Smart Cities  
• Public Waste Management  
• Campus Automation  
• Industrial Waste Monitoring  
• IoT Demonstration Projects  

------------------------------------------------------

## 🔹 Project Status

✔ Fully Functional  
✔ IoT Integrated  
✔ Real-Time Monitoring  
✔ Alert System Verified  
