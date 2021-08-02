#include <EEPROM.h>

// Operating System
// Written by Annabel Sandford

// Declarations
uint8_t eead = 130; // EEPROM Address we write to
int v, usronce, knwonce; // Declare int for line and UI optimization
int usrnm_limit = 4; // Declare username char limit (4)
int usrnm_set = 0; // Whether a custom username had been set or not (later replaced by EEPROM value)
// No I cannot check whether the username is empty or not because strings spit out a value of infinite question marks
// making it impossible to check for an empty username. Thus a value of 1 is set when one exists & 0 if not.

String readSerial, readUsername, readNew, usrnm;
String br = "="; // Line characters (only UI)
String del = "userdelete"; // sys-code: Delete the username
String inf = "info"; // sys-code: info
String hi = "hi"; // sys-code: greeting
String help = "help"; // sys-code: help

// Functions
int nwln() { // New Line
  Serial.println();
}

int res() { // Reset User Input
  readSerial = ""; 
}

int ret_input() { // Return User Input
  Serial.print("> ");
  Serial.print(readSerial);
  nwln();
}

void ln() { // Create line for UI
  v = 0;
  while (v < 40)
  {
    Serial.print(br);
    v++;
  }
  nwln();
}

void writeStringToEEPROM(int addrOffset, const String &strToWrite) // Writes username string to EEPROM
{
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
}

String readStringFromEEPROM(int addrOffset) // Reads username from the EEPROM
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0';
  return String(data);
}

int help_func() {
  ret_input();
  Serial.println(del);
  Serial.println(inf);
  Serial.println(hi);
  Serial.println(help);
  ln();
 }

int inf_func() {
  ret_input();
  Serial.println("OS Information:");
  nwln();
  Serial.print("User: ");
  Serial.print(usrnm);
  nwln();
  Serial.print("EEPROM usr_set: ");
  Serial.print(usrnm_set);
  nwln();
  ln();
}

int hi_func() {
  ret_input();
  Serial.print("Hey, ");
  Serial.print(usrnm);
  Serial.print("!");
  nwln();
  ln();
}

// Actual software starts here

void setup() {
  delay(200);
  usronce = 0;
  knwonce = 0;
  Serial.begin(9600);
  Serial.println("CHIPSET INITIALIZED"); // so I can keep track of what is loaded
  ln();
}

void loop() {
  restart:
  usrnm_set = EEPROM.read(eead);
  if (usrnm_set == 255) {
    Serial.println(">Warning: EEPROM usrnm_set = 0 <");
    EEPROM.write(eead, 0);
    goto restart;
  }
  
  usrnm = readStringFromEEPROM(0);
  if (usrnm_set == 0) {
    if (usronce < 1) {
      res();
      Serial.println("No Username.");
      Serial.println("Please enter a Username c:");
      nwln();
      usronce++;
    }
    delay(10);
    
    while (Serial.available()) {
    delay(10);
    if (Serial.available() >0) {
    char s = Serial.read();
    readSerial += s;
    }
   }

   if (readSerial.length() >0) {
    if (readSerial.length() >= usrnm_limit+2) {
      Serial.print("Username over ");
      Serial.print(usrnm_limit);
      Serial.print(" chars. Try again.");
      nwln();
      res();
      goto restart;
    }
    readSerial.trim();
    
    //Output what the User said
    ln();
    Serial.println("New Username:");
    Serial.println(readSerial);
    writeStringToEEPROM(0, readSerial);
    usrnm_set = 1;
    EEPROM.write(eead, usrnm_set);
    ln();
    res();
   }
  } else {
    textedit();
  }
 }

 void textedit() {
   if (knwonce < 1) {
    Serial.print("Welcome back, ");
    Serial.print(usrnm);
    nwln();
    ln();
    knwonce++;
   }
    while (Serial.available()) {
    delay(10);
  
  if (Serial.available() >0) {
   char c = Serial.read();
    readSerial += c;
    }
   }
   
   if (readSerial.length() >0) {
    readSerial.trim();
    readNew = readSerial;
    readNew.toLowerCase();
    
    if (readNew.equals(inf)) {
      inf_func();
      res();
    } else if (readNew.equals(del)) {
      nwln();
      Serial.print("!> Deleted User ");
      Serial.print(usrnm);
      Serial.println();
      EEPROM.write(eead, 0);
      loop();
    } else if (readNew.equals(help)) {
      help_func();
      res();
    } else if (readNew.equals(hi)) {
      hi_func();
      res();
    } else {
     
    //Output what the User said
    ret_input();
    Serial.println("!> Unknown command");
    ln();
    res();
    }
   }
 }
