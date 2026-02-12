# Test Plan

## Functional Tests

1. Fingerprint recognized voter can enter voting flow.
2. Unregistered fingerprint is rejected.
3. Voter can cast exactly one vote.
4. Duplicate vote attempt is blocked and logged in `/DuplicateVoters`.
5. Supervisor fingerprint enters lock/result mode.
6. Reset action clears vote counters and voter lists.

## Firebase Validation

Verify updates for:

- `/TotalVotes`
- `/BJPVotes`, `/CongressVotes`, `/JDSVotes`, `/NOTAVotes`
- `/VotedVoters`
- `/DuplicateVoters`
- `/ResultVisible`, `/TriggerSheetUpdate`, `/ResetSheet`

## Hardware Validation

- OLED and LCD display correct prompts
- Buzzer tones for normal/error/supervisor states
- All vote buttons trigger expected party selections

## Regression Note

After changing GPIO assignments, rerun all button and reset tests.
