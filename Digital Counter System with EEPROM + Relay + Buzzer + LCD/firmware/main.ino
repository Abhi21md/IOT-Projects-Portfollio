#include <EEPROM.h> 
#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd(0x27,16,2);

// Sensor and Button Pins
#define sensor_pin 2
#define bt_set   A0
#define bt_up_reset A1
#define bt_start_stop  A2
#define relay 8
#define buzzer 13

// Variables
int sensor=1;  // (PNP=1 or NPN=0)
int flag1=0, flag2=0, Check=0, Set=0;
long set_value=0, counter=0;
long d4=1, d3=2, d2=3, d1=4;
unsigned long lastPulseTime = 0; 
const unsigned long debounceDelay = 200;  // Adjust this value as needed

// Custom Characters for LCD Display
byte bar1[8]={B11100,B11110,B11110,B11110,B11110,B11110,B11110,B11100};
byte bar2[8]={B00111,B01111,B01111,B01111,B01111,B01111,B01111,B00111};

void setup() {
  pinMode(sensor_pin, INPUT); 
  attachInterrupt(digitalPinToInterrupt(sensor_pin), read_pulse, RISING);
  
  pinMode(bt_set, INPUT_PULLUP);
  pinMode(bt_up_reset, INPUT_PULLUP);
  pinMode(bt_start_stop, INPUT_PULLUP);
  pinMode(relay, OUTPUT); digitalWrite(relay, HIGH);
  pinMode(buzzer, OUTPUT); 

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("   Welcome To   ");
  lcd.setCursor(0,1);
  lcd.print("Digital  Counter");
  delay(2000);
  lcd.clear(); 

  if(EEPROM.read(0)==0){}
  else{
    Write_set();
    EEPROM.write(0, 0);
  } 
  Read_set();
}

void loop() {  
  // Button Handling
  if(digitalRead(bt_set)==LOW && flag1==0) {
    flag1 = 1;
    Set++;
    if(Set > 4) Set = 0;
    flash();
    delay(400);  
  } else { flag1 = 0; }

  if(digitalRead(bt_up_reset)==LOW) {
    if(Set > 0) {
      if(Set == 4) d1++;
      if(Set == 3) d2++;
      if(Set == 2) d3++;
      if(Set == 1) d4++;
      
      if(d1 > 9) d1 = 0;
      if(d2 > 9) d2 = 0;
      if(d3 > 9) d3 = 0;
      if(d4 > 9) d4 = 0;

      Write_set();
      Read_set();
    } else {
      Check = 0; counter = 0; 
      digitalWrite(relay, HIGH); 
    }
    delay(200);
  }

  if(digitalRead(bt_start_stop)==LOW && flag2==0) {
    flag2 = 1;
    if(Set > 0) Set = 0;
    else {
      if(counter >= set_value) {
        counter = 0; Check = 1; digitalWrite(relay, LOW);
      } else {
        Check = !Check;
        digitalWrite(relay, Check ? LOW : HIGH); 
      }
    }  
  } else { flag2 = 0; }

  if(counter >= set_value && Check == 1) {
    Check = 0; digitalWrite(relay, HIGH);
    for(int x = 0; x < 10; x++) {
      value_display();  
      digitalWrite(buzzer, HIGH);
      delay(300);
      digitalWrite(buzzer, LOW);
      delay(200);
    }
  }

  value_display();        
  delay(10); 
  digitalWrite(buzzer, LOW); 
}

void read_pulse() {
  unsigned long currentTime = millis();
  if(currentTime - lastPulseTime >= debounceDelay) {
    counter++;
    lastPulseTime = currentTime;
    if(counter >= set_value) {
      digitalWrite(relay, HIGH);
    }
  }
}

// Function Definitions
void value_display() {
  if(counter >= 1000) printNumber((counter / 1000) % 10, 4);
  if(counter >= 100)  printNumber((counter / 100) % 10, 7);
  if(counter >= 10)   printNumber((counter / 10) % 10, 10);
  printNumber(counter % 10, 13);
  set_display();
}

void set_display() {
  lcd.setCursor(0, 0);
  lcd.print((Set > 0) ? "SET " : (Check ? "<R>" : "<S>"));
  lcd.setCursor(0, 1);
  lcd.print((set_value/1000)%10);
  lcd.print((set_value/100)%10);
  lcd.print((set_value/10)%10);
  lcd.print(set_value%10);  
}

void flash() {
  set_display();
  lcd.setCursor(Set-1, 1);
  lcd.print(" ");
}

void printNumber(int value, int col) {
  lcd.setCursor(col, 0); lcd.print(value);
}

void Write_set() {
  EEPROM.write(11, d1);
  EEPROM.write(12, d2);
  EEPROM.write(13, d3);
  EEPROM.write(14, d4);
}

void Read_set() {
  d4 = EEPROM.read(14);
  d3 = EEPROM.read(13);
  d2 = EEPROM.read(12);
  d1 = EEPROM.read(11); 
  set_value = d4 * 1000 + d3 * 100 + d2 * 10 + d1;
}