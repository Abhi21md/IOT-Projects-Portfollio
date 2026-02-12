TEST PROCEDURE
----------------

1) Power ON
   - LCD should display:
     SMART GRAIN ATM
     Scan Card...

2) RFID Test
   - Scan valid card → shows name and balance
   - Scan invalid card → shows "Invalid Card"

3) Quantity Selection
   - Press 100g or 200g button
   - System should proceed if balance sufficient

4) Dispensing Test
   - Servo opens gate
   - Weight increases on LCD
   - Stops exactly at selected weight

5) Balance Deduction
   - After dispensing, new balance displayed

6) Low Balance Test
   - If balance < cost → show "Low Balance"

7) Tare Button
   - Press tare button
   - Scale resets to zero
