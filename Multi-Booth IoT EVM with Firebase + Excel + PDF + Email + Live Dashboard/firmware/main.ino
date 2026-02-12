#include <WiFi.h>
#include <FirebaseESP32.h>
#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ================== CONFIG ==================
#define WIFI_SSID "Smart Bin"
#define WIFI_PASSWORD "7899262658"

#define FIREBASE_HOST "https://evmm-c0da2-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "9diiEfWJxPDHJMLl2ePBHI7QXvoM6CHrHBFsSmgu"

#define RESET_BUTTON 15
#define RX_PIN 16
#define TX_PIN 17

#define OLED_SDA 21
#define OLED_SCL 22
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define BUTTON_BJP 12
#define BUTTON_CONGRESS 13
#define BUTTON_JDS 14
#define BUTTON_NOTA 15

#define BUZZER 23

// ================== OBJECTS ==================
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ================== VARIABLES ==================
int totalVotes = 0, bjpVotes = 0, congressVotes = 0, jdsVotes = 0, notaVotes = 0;
String votedVoters = "", duplicateVoters = "";
bool supervisorLock = false;
bool safeReset = false;

// ================== BUZZER ==================
void normalBeep() { tone(BUZZER, 2000, 120); }
void voteRecordedTone() { tone(BUZZER, 1500, 120); delay(120); tone(BUZZER, 2000, 120); }
void alreadyVotedTone() { tone(BUZZER, 500, 200); delay(100); tone(BUZZER, 400, 200); }
void supervisorTone() { tone(BUZZER, 2500, 150); delay(120); tone(BUZZER, 2500, 150); }
void invalidTone() { tone(BUZZER, 300, 250); }

// ================== OLED PRINT ==================
void showOLEDMessage(String msg) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println(msg);
  display.display();
}

// ================== VOTE CONFIRMATION ==================
void showVoteConfirmation(String partyName) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Voted");
  if (partyName == "CONGRESS") partyName = "CONG";

  display.setTextSize(2);
  int16_t x, y; uint16_t w, h;
  display.getTextBounds(partyName, 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 12);
  display.println(partyName);
  display.display();
}

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  pinMode(BUZZER, OUTPUT);
  pinMode(RESET_BUTTON, INPUT_PULLUP);

  pinMode(BUTTON_BJP, INPUT_PULLUP);
  pinMode(BUTTON_CONGRESS, INPUT_PULLUP);
  pinMode(BUTTON_JDS, INPUT_PULLUP);
  pinMode(BUTTON_NOTA, INPUT_PULLUP);

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  mySerial.begin(57600, SERIAL_8N1, RX_PIN, TX_PIN);
  finger.begin(57600);

  if (finger.verifyPassword()) {
    lcd.print("Sensor Found");
    normalBeep();
  } else {
    lcd.print("Sensor Error!");
    invalidTone();
    while (1);
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lcd.clear();
  lcd.print("Connecting WiFi...");
  while (WiFi.status() != WL_CONNECTED) delay(400);

  lcd.clear();
  lcd.print("WiFi Connected");
  normalBeep();

  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);

  Firebase.getString(firebaseData, "/VotedVoters") ?
      votedVoters = firebaseData.stringData() : votedVoters = "";

  Firebase.getString(firebaseData, "/DuplicateVoters") ?
      duplicateVoters = firebaseData.stringData() : duplicateVoters = "";

  delay(700);
  lcd.clear();
  lcd.print("EVM Ready");
  lcd.setCursor(0, 1);
  lcd.print("Place Finger");
  showOLEDMessage("Place Finger");
}

// ================== STABLE RESET ==================
void resetElection() {

  safeReset = true;
  supervisorLock = false;

  // CLEAR VOTES
  Firebase.setInt(firebaseData, "/TotalVotes", 0);
  Firebase.setInt(firebaseData, "/BJPVotes", 0);
  Firebase.setInt(firebaseData, "/CongressVotes", 0);
  Firebase.setInt(firebaseData, "/JDSVotes", 0);
  Firebase.setInt(firebaseData, "/NOTAVotes", 0);

  Firebase.deleteNode(firebaseData, "/VotedVoters");
  Firebase.deleteNode(firebaseData, "/DuplicateVoters");

  // ========== RESET THE 3 FLAGS ==========
  Firebase.setBool(firebaseData, "/ResultVisible", false);
  Firebase.setBool(firebaseData, "/TriggerSheetUpdate", false);
  Firebase.setBool(firebaseData, "/ResetSheet", false);

  votedVoters = "";
  duplicateVoters = "";
  totalVotes = bjpVotes = congressVotes = jdsVotes = notaVotes = 0;

  lcd.clear();
  lcd.print("Election Reset!");
  showOLEDMessage("Election Reset!");

  invalidTone();
  delay(400);

  // BACK TO NORMAL MODE
  lcd.clear();
  lcd.print("EVM Ready");
  lcd.setCursor(0, 1);
  lcd.print("Place Finger");
  showOLEDMessage("Place Finger");

  safeReset = false;
}

// ================== LOCKED RESULT SCREEN ==================
void displayResultsLocked() {

  while (supervisorLock && !safeReset) {

    Firebase.getInt(firebaseData, "/TotalVotes") ? totalVotes = firebaseData.intData() : totalVotes = 0;
    Firebase.getInt(firebaseData, "/BJPVotes") ? bjpVotes = firebaseData.intData() : bjpVotes = 0;
    Firebase.getInt(firebaseData, "/CongressVotes") ? congressVotes = firebaseData.intData() : congressVotes = 0;
    Firebase.getInt(firebaseData, "/JDSVotes") ? jdsVotes = firebaseData.intData() : jdsVotes = 0;
    Firebase.getInt(firebaseData, "/NOTAVotes") ? notaVotes = firebaseData.intData() : notaVotes = 0;

    lcd.clear();
    lcd.print("T:" + String(totalVotes) + " B:" + String(bjpVotes));
    lcd.setCursor(0, 1);
    lcd.print("C:" + String(congressVotes) + " J:" + String(jdsVotes));

    showOLEDMessage("Results Visible");

    // RESET BUTTON HANDLES FLAGS
    if (digitalRead(RESET_BUTTON) == LOW) {
      resetElection();
      return;
    }

    delay(300);
  }
}

// ================== LOOP ==================
void loop() {

  if (supervisorLock) {
    displayResultsLocked();
    return;
  }

  int id = getFingerprintID();

if (id == 0) {
  lcd.clear();
  lcd.print("Not Registered");
  showOLEDMessage("Invalid / Not Reg");
  invalidTone();
  delay(800);
  return;
}

if (id < 0) return;


  if (id == 6) {
    supervisorTone();
    handleSupervisor();
    return;
  }

  if (id >= 1 && id <= 50) {
    handleVoter(id);
  } else {
    lcd.clear();
    lcd.print("Invalid Voter!");
    showOLEDMessage("Invalid Voter");
    invalidTone();
    delay(800);
  }

  lcd.clear();
  lcd.print("Place Finger");
  showOLEDMessage("Place Finger");
}

// ================== FINGERPRINT ID ==================
int getFingerprintID() {
  if (finger.getImage() != FINGERPRINT_OK) return -1;
  if (finger.image2Tz() != FINGERPRINT_OK) return -1;
  if (finger.fingerFastSearch() != FINGERPRINT_OK) return 0;

  normalBeep();
  return finger.fingerID;
}

// ================== HANDLE VOTER ==================
void handleVoter(int voterID) {

  if (supervisorLock) return;

  Firebase.getString(firebaseData, "/VotedVoters") ?
      votedVoters = firebaseData.stringData() : votedVoters = "";

  Firebase.getString(firebaseData, "/DuplicateVoters") ?
      duplicateVoters = firebaseData.stringData() : duplicateVoters = "";

  String idStr = String(voterID);

  // ===== DUPLICATE VOTE DETECTED =====
  if (votedVoters.indexOf(idStr) != -1) {

    // ---- ADD DUPLICATE ID TO FIREBASE ----
    if (duplicateVoters.indexOf(idStr) == -1) {
      duplicateVoters += (duplicateVoters.length() > 0 ? "," : "") + idStr;
      Firebase.setString(firebaseData, "/DuplicateVoters", duplicateVoters);
    }

    lcd.clear();
    lcd.print("Already Voted");
    showOLEDMessage("Already Voted");
    alreadyVotedTone();
    delay(800);
    return;
  }

  // -------- rest of your code stays EXACTLY the same ----------

  lcd.clear();
  lcd.print("Cast Vote...");
  showOLEDMessage("Press Button");
  normalBeep();

  char vote = 0;
  while (vote == 0 && !supervisorLock) {
    if (digitalRead(BUTTON_BJP) == LOW) vote = 'B';
    else if (digitalRead(BUTTON_CONGRESS) == LOW) vote = 'C';
    else if (digitalRead(BUTTON_JDS) == LOW) vote = 'J';
    else if (digitalRead(BUTTON_NOTA) == LOW) vote = 'N';
    delay(40);
  }

  Firebase.getInt(firebaseData, "/BJPVotes") ? bjpVotes = firebaseData.intData() : bjpVotes = 0;
  Firebase.getInt(firebaseData, "/CongressVotes") ? congressVotes = firebaseData.intData() : congressVotes = 0;
  Firebase.getInt(firebaseData, "/JDSVotes") ? jdsVotes = firebaseData.intData() : jdsVotes = 0;
  Firebase.getInt(firebaseData, "/NOTAVotes") ? notaVotes = firebaseData.intData() : notaVotes = 0;
  Firebase.getInt(firebaseData, "/TotalVotes") ? totalVotes = firebaseData.intData() : totalVotes = 0;

  if (vote == 'B') showVoteConfirmation("BJP"), bjpVotes++;
  else if (vote == 'C') showVoteConfirmation("CONGRESS"), congressVotes++;
  else if (vote == 'J') showVoteConfirmation("JDS"), jdsVotes++;
  else if (vote == 'N') showVoteConfirmation("NOTA"), notaVotes++;

  voteRecordedTone();
  delay(2000);

  totalVotes++;
  votedVoters += (votedVoters.length() > 0 ? "," : "") + idStr;

  Firebase.setString(firebaseData, "/VotedVoters", votedVoters);
  Firebase.setInt(firebaseData, "/TotalVotes", totalVotes);
  Firebase.setInt(firebaseData, "/BJPVotes", bjpVotes);
  Firebase.setInt(firebaseData, "/CongressVotes", congressVotes);
  Firebase.setInt(firebaseData, "/JDSVotes", jdsVotes);
  Firebase.setInt(firebaseData, "/NOTAVotes", notaVotes);

  lcd.clear();
  lcd.print("Vote Recorded");
  showOLEDMessage("Vote Registered");
  delay(800);
}

// ================== SUPERVISOR ==================
void handleSupervisor() {

  supervisorLock = true;

  lcd.clear();
  lcd.print("Supervisor Mode");
  showOLEDMessage("Supervisor Mode");
  supervisorTone();

  // ========== SET ALL 3 FLAGS TRUE ==========
  Firebase.setBool(firebaseData, "/ResultVisible", true);
  Firebase.setBool(firebaseData, "/TriggerSheetUpdate", true);
  Firebase.setBool(firebaseData, "/ResetSheet", true);

  displayResultsLocked();
}    #include <WiFi.h>
#include <FirebaseESP32.h>
#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ================== CONFIG ==================
#define WIFI_SSID "Smart Bin"
#define WIFI_PASSWORD "7899262658"

#define FIREBASE_HOST "https://evmm-c0da2-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "9diiEfWJxPDHJMLl2ePBHI7QXvoM6CHrHBFsSmgu"

#define RESET_BUTTON 15
#define RX_PIN 16
#define TX_PIN 17

#define OLED_SDA 21
#define OLED_SCL 22
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define BUTTON_BJP 12
#define BUTTON_CONGRESS 13
#define BUTTON_JDS 14
#define BUTTON_NOTA 15

#define BUZZER 23

// ================== OBJECTS ==================
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ================== VARIABLES ==================
int totalVotes = 0, bjpVotes = 0, congressVotes = 0, jdsVotes = 0, notaVotes = 0;
String votedVoters = "", duplicateVoters = "";
bool supervisorLock = false;
bool safeReset = false;

// ================== BUZZER ==================
void normalBeep() { tone(BUZZER, 2000, 120); }
void voteRecordedTone() { tone(BUZZER, 1500, 120); delay(120); tone(BUZZER, 2000, 120); }
void alreadyVotedTone() { tone(BUZZER, 500, 200); delay(100); tone(BUZZER, 400, 200); }
void supervisorTone() { tone(BUZZER, 2500, 150); delay(120); tone(BUZZER, 2500, 150); }
void invalidTone() { tone(BUZZER, 300, 250); }

// ================== OLED PRINT ==================
void showOLEDMessage(String msg) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println(msg);
  display.display();
}

// ================== VOTE CONFIRMATION ==================
void showVoteConfirmation(String partyName) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Voted");
  if (partyName == "CONGRESS") partyName = "CONG";

  display.setTextSize(2);
  int16_t x, y; uint16_t w, h;
  display.getTextBounds(partyName, 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 12);
  display.println(partyName);
  display.display();
}

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  pinMode(BUZZER, OUTPUT);
  pinMode(RESET_BUTTON, INPUT_PULLUP);

  pinMode(BUTTON_BJP, INPUT_PULLUP);
  pinMode(BUTTON_CONGRESS, INPUT_PULLUP);
  pinMode(BUTTON_JDS, INPUT_PULLUP);
  pinMode(BUTTON_NOTA, INPUT_PULLUP);

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  mySerial.begin(57600, SERIAL_8N1, RX_PIN, TX_PIN);
  finger.begin(57600);

  if (finger.verifyPassword()) {
    lcd.print("Sensor Found");
    normalBeep();
  } else {
    lcd.print("Sensor Error!");
    invalidTone();
    while (1);
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lcd.clear();
  lcd.print("Connecting WiFi...");
  while (WiFi.status() != WL_CONNECTED) delay(400);

  lcd.clear();
  lcd.print("WiFi Connected");
  normalBeep();

  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);

  Firebase.getString(firebaseData, "/VotedVoters") ?
      votedVoters = firebaseData.stringData() : votedVoters = "";

  Firebase.getString(firebaseData, "/DuplicateVoters") ?
      duplicateVoters = firebaseData.stringData() : duplicateVoters = "";

  delay(700);
  lcd.clear();
  lcd.print("EVM Ready");
  lcd.setCursor(0, 1);
  lcd.print("Place Finger");
  showOLEDMessage("Place Finger");
}

// ================== STABLE RESET ==================
void resetElection() {

  safeReset = true;
  supervisorLock = false;

  // CLEAR VOTES
  Firebase.setInt(firebaseData, "/TotalVotes", 0);
  Firebase.setInt(firebaseData, "/BJPVotes", 0);
  Firebase.setInt(firebaseData, "/CongressVotes", 0);
  Firebase.setInt(firebaseData, "/JDSVotes", 0);
  Firebase.setInt(firebaseData, "/NOTAVotes", 0);

  Firebase.deleteNode(firebaseData, "/VotedVoters");
  Firebase.deleteNode(firebaseData, "/DuplicateVoters");

  // ========== RESET THE 3 FLAGS ==========
  Firebase.setBool(firebaseData, "/ResultVisible", false);
  Firebase.setBool(firebaseData, "/TriggerSheetUpdate", false);
  Firebase.setBool(firebaseData, "/ResetSheet", false);

  votedVoters = "";
  duplicateVoters = "";
  totalVotes = bjpVotes = congressVotes = jdsVotes = notaVotes = 0;

  lcd.clear();
  lcd.print("Election Reset!");
  showOLEDMessage("Election Reset!");

  invalidTone();
  delay(400);

  // BACK TO NORMAL MODE
  lcd.clear();
  lcd.print("EVM Ready");
  lcd.setCursor(0, 1);
  lcd.print("Place Finger");
  showOLEDMessage("Place Finger");

  safeReset = false;
}

// ================== LOCKED RESULT SCREEN ==================
void displayResultsLocked() {

  while (supervisorLock && !safeReset) {

    Firebase.getInt(firebaseData, "/TotalVotes") ? totalVotes = firebaseData.intData() : totalVotes = 0;
    Firebase.getInt(firebaseData, "/BJPVotes") ? bjpVotes = firebaseData.intData() : bjpVotes = 0;
    Firebase.getInt(firebaseData, "/CongressVotes") ? congressVotes = firebaseData.intData() : congressVotes = 0;
    Firebase.getInt(firebaseData, "/JDSVotes") ? jdsVotes = firebaseData.intData() : jdsVotes = 0;
    Firebase.getInt(firebaseData, "/NOTAVotes") ? notaVotes = firebaseData.intData() : notaVotes = 0;

    lcd.clear();
    lcd.print("T:" + String(totalVotes) + " B:" + String(bjpVotes));
    lcd.setCursor(0, 1);
    lcd.print("C:" + String(congressVotes) + " J:" + String(jdsVotes));

    showOLEDMessage("Results Visible");

    // RESET BUTTON HANDLES FLAGS
    if (digitalRead(RESET_BUTTON) == LOW) {
      resetElection();
      return;
    }

    delay(300);
  }
}

// ================== LOOP ==================
void loop() {

  if (supervisorLock) {
    displayResultsLocked();
    return;
  }

  int id = getFingerprintID();

if (id == 0) {
  lcd.clear();
  lcd.print("Not Registered");
  showOLEDMessage("Invalid / Not Reg");
  invalidTone();
  delay(800);
  return;
}

if (id < 0) return;


  if (id == 6) {
    supervisorTone();
    handleSupervisor();
    return;
  }

  if (id >= 1 && id <= 50) {
    handleVoter(id);
  } else {
    lcd.clear();
    lcd.print("Invalid Voter!");
    showOLEDMessage("Invalid Voter");
    invalidTone();
    delay(800);
  }

  lcd.clear();
  lcd.print("Place Finger");
  showOLEDMessage("Place Finger");
}

// ================== FINGERPRINT ID ==================
int getFingerprintID() {
  if (finger.getImage() != FINGERPRINT_OK) return -1;
  if (finger.image2Tz() != FINGERPRINT_OK) return -1;
  if (finger.fingerFastSearch() != FINGERPRINT_OK) return 0;

  normalBeep();
  return finger.fingerID;
}

// ================== HANDLE VOTER ==================
void handleVoter(int voterID) {

  if (supervisorLock) return;

  Firebase.getString(firebaseData, "/VotedVoters") ?
      votedVoters = firebaseData.stringData() : votedVoters = "";

  Firebase.getString(firebaseData, "/DuplicateVoters") ?
      duplicateVoters = firebaseData.stringData() : duplicateVoters = "";

  String idStr = String(voterID);

  // ===== DUPLICATE VOTE DETECTED =====
  if (votedVoters.indexOf(idStr) != -1) {

    // ---- ADD DUPLICATE ID TO FIREBASE ----
    if (duplicateVoters.indexOf(idStr) == -1) {
      duplicateVoters += (duplicateVoters.length() > 0 ? "," : "") + idStr;
      Firebase.setString(firebaseData, "/DuplicateVoters", duplicateVoters);
    }

    lcd.clear();
    lcd.print("Already Voted");
    showOLEDMessage("Already Voted");
    alreadyVotedTone();
    delay(800);
    return;
  }

  // -------- rest of your code stays EXACTLY the same ----------

  lcd.clear();
  lcd.print("Cast Vote...");
  showOLEDMessage("Press Button");
  normalBeep();

  char vote = 0;
  while (vote == 0 && !supervisorLock) {
    if (digitalRead(BUTTON_BJP) == LOW) vote = 'B';
    else if (digitalRead(BUTTON_CONGRESS) == LOW) vote = 'C';
    else if (digitalRead(BUTTON_JDS) == LOW) vote = 'J';
    else if (digitalRead(BUTTON_NOTA) == LOW) vote = 'N';
    delay(40);
  }

  Firebase.getInt(firebaseData, "/BJPVotes") ? bjpVotes = firebaseData.intData() : bjpVotes = 0;
  Firebase.getInt(firebaseData, "/CongressVotes") ? congressVotes = firebaseData.intData() : congressVotes = 0;
  Firebase.getInt(firebaseData, "/JDSVotes") ? jdsVotes = firebaseData.intData() : jdsVotes = 0;
  Firebase.getInt(firebaseData, "/NOTAVotes") ? notaVotes = firebaseData.intData() : notaVotes = 0;
  Firebase.getInt(firebaseData, "/TotalVotes") ? totalVotes = firebaseData.intData() : totalVotes = 0;

  if (vote == 'B') showVoteConfirmation("BJP"), bjpVotes++;
  else if (vote == 'C') showVoteConfirmation("CONGRESS"), congressVotes++;
  else if (vote == 'J') showVoteConfirmation("JDS"), jdsVotes++;
  else if (vote == 'N') showVoteConfirmation("NOTA"), notaVotes++;

  voteRecordedTone();
  delay(2000);

  totalVotes++;
  votedVoters += (votedVoters.length() > 0 ? "," : "") + idStr;

  Firebase.setString(firebaseData, "/VotedVoters", votedVoters);
  Firebase.setInt(firebaseData, "/TotalVotes", totalVotes);
  Firebase.setInt(firebaseData, "/BJPVotes", bjpVotes);
  Firebase.setInt(firebaseData, "/CongressVotes", congressVotes);
  Firebase.setInt(firebaseData, "/JDSVotes", jdsVotes);
  Firebase.setInt(firebaseData, "/NOTAVotes", notaVotes);

  lcd.clear();
  lcd.print("Vote Recorded");
  showOLEDMessage("Vote Registered");
  delay(800);
}

// ================== SUPERVISOR ==================
void handleSupervisor() {

  supervisorLock = true;

  lcd.clear();
  lcd.print("Supervisor Mode");
  showOLEDMessage("Supervisor Mode");
  supervisorTone();

  // ========== SET ALL 3 FLAGS TRUE ==========
  Firebase.setBool(firebaseData, "/ResultVisible", true);
  Firebase.setBool(firebaseData, "/TriggerSheetUpdate", true);
  Firebase.setBool(firebaseData, "/ResetSheet", true);

  displayResultsLocked();
}    