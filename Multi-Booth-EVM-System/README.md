# Multi Booth EVM System

This project has been organized in standard IoT portfolio format.

## Structure

```text
Multi-Booth-EVM-System/
├── config/
├── docs/
├── firmware/
├── hardware/
├── test/
└── README.md
```

## Start Here

- Overview: `docs/PROJECT_OVERVIEW.md`
- Setup: `docs/SETUP.md`
- Architecture: `docs/ARCHITECTURE.md`
- Firmware: `firmware/MultiBoothEVMSystem/MultiBoothEVMSystem.ino`
# Multi Booth EVM System (Firebase + Fingerprint + Live Dashboard)

This project is a **multi-booth Electronic Voting Machine (EVM)** system built for ESP32 with:

- Fingerprint-based voter verification
- Party voting buttons (BJP, Congress, JDS, NOTA)
- Duplicate vote detection
- Supervisor lock mode
- Firebase real-time vote sync
- Trigger flags for Excel/PDF/Email automation and live turnout dashboard

## Included Files

- `Multi_Booth_EVM_System.ino` → Main ESP32 firmware
- `README.md` → Project setup and integration notes

## Hardware Used

- ESP32
- R307/AS608 (or compatible) fingerprint sensor
- I2C 16x2 LCD
- 128x64 OLED (SSD1306)
- 4 vote push buttons
- 1 reset/supervisor control button
- Buzzer

## Pin Mapping (as used in sketch)

- Fingerprint RX/TX: `16`, `17`
- OLED SDA/SCL: `21`, `22`
- Vote buttons: `12`, `13`, `14`, `15`
- Reset button: `15`
- Buzzer: `23`

> Note: `RESET_BUTTON` and `BUTTON_NOTA` both use pin `15` in the provided code. Use separate pins in production hardware.

## Firebase Realtime Database Keys

The firmware reads/writes these keys:

- `/TotalVotes`
- `/BJPVotes`
- `/CongressVotes`
- `/JDSVotes`
- `/NOTAVotes`
- `/VotedVoters`
- `/DuplicateVoters`
- `/ResultVisible`
- `/TriggerSheetUpdate`
- `/ResetSheet`

## Excel / PDF / Email / Live Dashboard Workflow

Use Firebase triggers from backend automation (Apps Script / Cloud Functions / Node.js):

1. ESP32 sets `/TriggerSheetUpdate = true` when supervisor logs in.
2. Backend reads latest counts from Firebase.
3. Backend appends data into Google Sheet (or Excel-compatible sheet).
4. Backend generates PDF result report.
5. Backend emails PDF to supervisor.
6. Backend resets `/TriggerSheetUpdate` back to `false`.
7. Dashboard listens to vote keys for live turnout visualization.

## Arduino Libraries Needed

Install these libraries from Arduino Library Manager:

- `Firebase ESP32 Client` (or equivalent `FirebaseESP32` package used in your IDE)
- `Adafruit Fingerprint Sensor Library`
- `LiquidCrystal I2C`
- `Adafruit GFX Library`
- `Adafruit SSD1306`

## Upload Notes

1. Open `Multi_Booth_EVM_System.ino` in Arduino IDE.
2. Select board: **ESP32 Dev Module**.
3. Select correct COM port.
4. Install required libraries.
5. Upload sketch.

## Security Recommendation

The current sketch contains plaintext Wi-Fi and Firebase credentials. Before production deployment:

- rotate Firebase tokens,
- move credentials to secure storage,
- use per-device authentication,
- avoid committing secret keys to public repositories.
