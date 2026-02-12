# Smart EV Wireless Charging Station

Board: NodeMCU (ESP8266)
Platform: Blynk IoT
Display: 16x2 I2C LCD
Sensor: Ultrasonic (HC-SR04)
Control: Relay module
Alert: Active Buzzer (non-blocking logic)

--------------------------------------------------

## Working Concept

The system detects vehicle position using an ultrasonic sensor.

Distance Logic:
> 15 cm → No car detected
10–15 cm → Move closer
5–10 cm → Stop and ready
≤ 5 cm → Perfect parking → Auto charging starts

User sets charging time from Blynk (V1).
Manual ON/OFF control from Blynk (V2).
Charging LED indicator via V3.

Auto cut-off when timer completes.

--------------------------------------------------

## Advanced Features

✔ Multi-sample ultrasonic averaging
✔ Non-blocking buzzer scheduler (no delay blocking)
✔ Automatic start at perfect parking
✔ Auto cut-off after time limit
✔ Manual override
✔ Parking guidance beeps
✔ LCD live time display
✔ Car removal auto stop
✔ Cloud monitoring via Blynk

--------------------------------------------------

## Blynk Mapping

V1 → Charging time (minutes)
V2 → Manual ON/OFF
V3 → Charging status LED

--------------------------------------------------

## Safety Logic

• Stops charging if car moves away
• Stops automatically after timer
• Relay controlled safely
• Echo pin must use voltage divider (5V → 3.3V safe)

--------------------------------------------------

Project Type: IoT + EV Infrastructure
Status: Fully Functional
