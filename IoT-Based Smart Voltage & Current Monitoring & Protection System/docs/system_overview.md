PROJECT DOCUMENTATION

System Type:
IoT-Based Electrical Protection System

Working Principle:
1. Voltage sensor reads AC waveform.
2. RMS calculation is performed.
3. Current sensor measures load current.
4. Protection logic checks limits.
5. If fault detected:
   - Relay OFF
   - Buzzer ON
   - Blynk Event Triggered
   - LCD shows fault

Fault Conditions:
- Over Voltage
- Under Voltage
- Over Current

Auto Recovery:
System restores automatically when values return to safe range.
