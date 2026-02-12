# Architecture

## Device Layer (ESP32)

- Captures fingerprint
- Allows vote casting through hardware buttons
- Writes counts and state into Firebase Realtime Database
- Enables supervisor mode using special fingerprint ID

## Cloud Layer (Firebase Realtime Database)

Stores live counters, voted IDs, duplicate IDs, and export trigger flags.

Suggested paths:

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

## Automation Layer (Optional)

Any backend (Google Apps Script / Cloud Functions / Node.js) can:

1. Watch `/TriggerSheetUpdate`
2. Pull latest vote totals
3. Append to sheet/excel-like store
4. Generate PDF result
5. Email supervisor
6. Reset trigger flag

## Dashboard Layer

A web dashboard can subscribe to Firebase keys for live turnout/results.
