#include <EEPROM.h>

// Interstellar Operating System
// Written by Annabel Sandford

// Declarations
uint8_t eead = 130; // EEPROM Address we write to
int v, u, usronce, knwonce; // Declare int for line and UI optimization
int usrnm_limit = 4; // Declare username char limit (4)
int usrnm_set = 0; // Whether a custom username had been set or not (later replaced by EEPROM value)
// No I cannot check whether the username is empty or not because strings spit out a value of infinite question marks
// making it impossible to check for an empty username. Thus a value of 1 is set when one exists & 0 if not.

String readSerial, readUsername, readNew, usrnm;
String sysname = "Interstellar OS"; // Name of this system
String sysver = "0.0.1"; // Version number
String br = "-"; // Line characters (only UI)
String del = "userdelete"; // sys-code: Delete the username
String inf = "info"; // sys-code: info
String hi = "hi"; // sys-code: greeting
String help = "help"; // sys-code: help
String echo = "echo"; // sys-code: echo
String cls_str = "cls"; // sys-code: Call cls(); function to clear screen

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

void cls() { // Create 100 linebreaks to clear monitor
  u = 0;
  while (u < 100)
  {
    nwln();
    u++;
  }
}

void boot() { // Literally what it says
Serial.println("     ____      __                 __       ____ ");
Serial.println("    /  _/___  / /____  __________/ /____  / / /___ ______");
Serial.println("    / // __ \\/ __/ _ \\/ ___/ ___/ __/ _ \\/ / / __ `/ ___/");
Serial.println("  _/ // / / / /_/  __/ /  (__  ) /_/  __/ / / /_/ / /    ");
Serial.println(" /___/_/ /_/\\__/\\___/_/  /____/\\__/\\___/_/_/\\__,_/_/  ");
nwln();  // How can 5 println's take up 20% of the dynamic memory????? what??? this needs adjustment 
ln();
Serial.print("System Name : ");
Serial.print(sysname);
nwln();
Serial.print("System Version : ");
Serial.print(sysver);
nwln(); // Now 23%??? HOW???                                                                                                                                                
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
  Serial.println(cls);
  Serial.println(echo);
  ln();
 }

int inf_func() {
  ret_input();
  boot();
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

int echo_func() {
  ret_input();
  Serial.print("Hey, ");
  Serial.print(usrnm);
  Serial.print("!");
  nwln();
  ln();
}

int amogus_func() {
  ret_input();
}

// Actual software starts here

void setup() {
  delay(200);
  usronce = 0;
  knwonce = 0;
  Serial.begin(9600);
  cls();
  Serial.println("CHIPSET INITIALIZED"); // so I can keep track of what is loaded
  ln();
  delay(2500);
  cls();
  boot();
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
    } else if (readNew.equals(cls_str)) {
      cls();
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
