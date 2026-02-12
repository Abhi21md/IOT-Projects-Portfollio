TEST PROCEDURE
----------------

1) Power ON the system.
   LCD should show:
   "Gas Detection"
   "System Ready"

2) Normal Condition:
   - No gas
   - No flame
   LCD should display:
   SAFE ENVIRONMENT

3) Gas Test:
   - Bring lighter gas near MQ2
   - Gas value should increase
   - If value > 400:
       Buzzer ON
       LED ON
       LCD: GAS LEAK ALERT

4) Flame Test:
   - Bring candle flame near flame sensor
   - LCD should display:
       FLAME ALERT
   - Buzzer + LED ON

5) Remove hazard:
   System returns to SAFE ENVIRONMENT
