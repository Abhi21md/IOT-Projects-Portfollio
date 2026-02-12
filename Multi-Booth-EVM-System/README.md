# Multi Booth EVM System (Firebase + Fingerprint + Live Dashboard)

Multi-booth Electronic Voting Machine (EVM) for ESP32 with fingerprint authentication, vote casting buttons, Firebase sync, duplicate vote detection, supervisor mode, and backend trigger flags for Excel/PDF/email/reporting workflows.

## Project Structure

```text
Multi-Booth-EVM-System/
├── config/
│   ├── env.example
│   └── firebase-schema.json
├── docs/
│   ├── ARCHITECTURE.md
│   └── SETUP.md
├── firmware/
│   └── MultiBoothEVMSystem/
│       └── MultiBoothEVMSystem.ino
├── hardware/
│   ├── BOM.md
│   └── PINOUT.md
├── test/
│   └── TEST_PLAN.md
└── README.md
```

## Core Features

- Fingerprint-based voter authentication
- Voting via BJP / Congress / JDS / NOTA buttons
- Duplicate-vote detection and logging to Firebase
- Supervisor lock mode and live result display
- Firebase flags for backend export workflows:
  - `/ResultVisible`
  - `/TriggerSheetUpdate`
  - `/ResetSheet`

## Firmware

Main sketch:

- `firmware/MultiBoothEVMSystem/MultiBoothEVMSystem.ino`

## Notes

- The current sketch includes plaintext Wi-Fi and Firebase credentials (for demonstration).
- `RESET_BUTTON` and `BUTTON_NOTA` currently share GPIO 15 in the provided firmware and should be separated in production.

## Next Improvements

- Split secrets to runtime config and avoid committing real credentials
- Add per-booth identifier for true multi-booth aggregation
- Add debouncing and long-press safety for supervisor/reset actions
- Add backend service example for sheet/pdf/email automation
