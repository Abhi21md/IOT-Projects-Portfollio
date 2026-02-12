TEST CASES
---------------------------------------

1. Valid Voter Vote
   → Fingerprint accepted
   → Button press recorded
   → Firebase updated
   → Vote confirmation shown

2. Duplicate Vote Attempt
   → Already Voted message
   → Duplicate list updated
   → Vote blocked

3. Multi Booth Sync Test
   → Vote at Booth 1
   → Try voting at Booth 2
   → System blocks duplicate

4. Supervisor Mode
   → Fingerprint ID 6
   → Results visible
   → Excel flag set true
   → PDF generation triggered

5. Reset Test
   → Press Reset button
   → All counts cleared
   → Flags reset
   → System returns to voting mode
