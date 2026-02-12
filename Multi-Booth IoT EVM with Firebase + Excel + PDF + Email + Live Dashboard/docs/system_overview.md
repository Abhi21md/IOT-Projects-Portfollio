SYSTEM OVERVIEW
-------------------------------

This project is a Multi-Booth IoT Based Electronic Voting Machine.

Two or more EVM units run the SAME firmware and connect to
a centralized Firebase Realtime Database.

Voters can cast vote at any nearby polling booth.
System prevents duplicate voting across booths.

---------------------------------------------

KEY FEATURES
---------------------------------------------

✔ Multi-Booth Voting Support
✔ Real-Time Firebase Sync
✔ Duplicate Vote Detection
✔ Supervisor Lock Mode
✔ Excel Auto Update
✔ PDF Result Generation
✔ Email Notification
✔ Live Turnout Dashboard
✔ Secure Reset System

---------------------------------------------

WORKING PRINCIPLE
---------------------------------------------

1. Voter scans fingerprint
2. Firebase checks if voter already voted
3. If not voted → Vote allowed
4. If already voted → Blocked across ALL booths
5. Vote instantly updated to Firebase
6. Dashboard updates in real time
7. Supervisor fingerprint unlocks result screen
8. Reset button safely resets entire election

---------------------------------------------

DATABASE STRUCTURE
---------------------------------------------

/TotalVotes
/BJPVotes
/CongressVotes
/JDSVotes
/NOTAVotes
/VotedVoters
/DuplicateVoters
/ResultVisible
/TriggerSheetUpdate
/ResetSheet

---------------------------------------------

BENEFITS
---------------------------------------------

• No need to travel to native place to vote
• Increases voter turnout
• Prevents duplicate voting across booths
• Transparent cloud-based result monitoring
