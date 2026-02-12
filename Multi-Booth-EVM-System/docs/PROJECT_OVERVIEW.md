# Project Overview

## Title

Multi Booth EVM System (Firebase + Fingerprint + Live Dashboard)

## Description

Multi-booth Electronic Voting Machine (EVM) for ESP32 with:

- fingerprint authentication,
- party vote buttons,
- duplicate-vote detection,
- supervisor mode,
- Firebase realtime synchronization,
- backend trigger flags for sheet/pdf/email/reporting workflows.

## Features

- Fingerprint-based voter authentication
- Voting via BJP / Congress / JDS / NOTA buttons
- Duplicate-vote detection and logging in Firebase
- Supervisor lock mode and live result display
- Firebase flags for backend export workflows:
  - `/ResultVisible`
  - `/TriggerSheetUpdate`
  - `/ResetSheet`

## Firmware

- `firmware/MultiBoothEVMSystem/MultiBoothEVMSystem.ino`

## Notes

- The sketch currently contains plaintext Wi-Fi/Firebase credentials (demo only).
- `RESET_BUTTON` and `BUTTON_NOTA` share GPIO 15 in current sketch; separate pins are recommended for production.

## Suggested Next Steps

- Move secrets to environment/build-time config
- Add per-booth ID support for true multi-booth deployment
- Add supervisor/reset safety logic (long-press + debounce)
- Add backend sample for Google Sheet/PDF/Email automation
