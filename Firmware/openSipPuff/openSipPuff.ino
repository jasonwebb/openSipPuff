/*******************************************************************************************
 openSip+Puff main sketch
 
 Last revision: 9/16/2013
 Author: Jason Webb
 Author's website: http://jason-webb.info
 Project wiki: http://jason-webb.info/wiki/index.php?title=OpenSip%2BPuff
 
 Description:
 ============
 Highly expressive, reconfigurable breath interface that performs native USB keyboard
 and mouse actions based on user-defined associations with "sipping" and "puffing"
 detected with a pressure transducer.
 
 Mode flag byte:
 ===============
 The very first byte stored in EEPROM (address 0) sets the mode of the device, and can 
 be any of the following:
 
 [B] = basic mode - sip and puff
 [F] = fine mode - soft and hard sips and puffs
 [P] = pattern mode - pattern of sips and puffs
 
 Command packet structure:
 =========================
 Keyboard command: [P|S],[S|H],K,[number of keys],[key #1],...,[key n],255
 Mouse command: [P|S],[S|H],M,[C|M|D],[parameter #1],...,[parameter n],255
 
 Parameter legend:
 =================
 [P|S] = puff or sip
 [S|H] = soft or hard
 [K] = keyboard action
 [M] = mouse action
 [C|M|D] = click, move, drag
 [key] = ASCII value of key to press (keyboard modifiers are also valid: http://arduino.cc/en/Reference/KeyboardModifiers)
 [parameter] = interpretation depends on selected action type
 255 = indicates end of command

 Examples:
 =========
 1) Press "return" key on soft sip = S,S,K,1,KEY_RETURN,255
 2) Press "Win + R" on hard puff = P,H,K,2,KEY_GUI_LEFT,r,255
 3) Click left mouse button on soft puff = P,S,M,C,1,255
 4) Move mouse position up 5 units on hard sip = S,H,M,M,0,5,0,255
 5) Click left mouse button and drag left 5 units on soft sip = S,S,M,D,1,-5,0,0,255
 
 Uses
 =====
 1) FPS controls = crouch (CTRL), jump (space), reload (r), fire (left mouse)
 2) Rotate 3D model = click mouse button and drag along one or more axes
 3) Launch programs = install AutoHotKey, assign programs to special keyboard combinations
 4) Turn volume up/down = install AutoHotKey, assign OS volume to special keyboard combinations
 
*******************************************************************************************/

#include <EEPROM.h>    // EEPROM library used for storing configuration between uses

// Sensor pin and readings
int sensorPin = 5;              // Analog pin 5 (PF0/ADC0)
const int numReadings = 10;     // Number of readings to buffer
int sensorValues[numReadings];  // Buffer of readings
int sensorAverage;              // Average of all readings in buffer
int sensorBaseValue = 457;      // Value of sensor with no input

// Sip and puff detection variables
int basicThreshold = 150;              // Threshold used for binary sip and puff
int softThreshold = 150;               // Threshold used for "soft" sips and puffs
int hardThreshold = 250;               // Threshold used for "hard" sips and puffs
boolean sipThresholdCrossed = false;   // Data crossed lower basic threshold
boolean puffThresholdCrossed = false;  // Data crossed upper basic threshold
int currentPeak = 0;                   // Records peaks

// Action detection flags
boolean sipDetected = false;    // Goes "true" when sip is detected
boolean puffDetected = false;   // Goes "false" when puff is detected

// General program behavior
boolean debug = false;

// EEPROM command variables
const int maxCommandLength = 10;
byte incomingCommand[maxCommandLength];  // Command buffer from EEPROM
byte commandDelimiter = 255;             // End of command delimiter
byte commandIndex = 0;                   // Current index within command being read
byte value = -1;                         // Most recent byte read from EEPROM
byte nextValue = -1;                     // Next byte found in EEPROM after value
byte currentAddress = 1;                 // Always start at 1, mode flag is at 0

// Commands (basic mode)
byte commandSip[maxCommandLength];    // Command to execute on sip
byte commandPuff[maxCommandLength];   // Command to execute on puff

// Commands (fine mode)
byte commandSipSoft[maxCommandLength];   // Command to execute on sip (soft)
byte commandSipHard[maxCommandLength];   // Command to execute on sip (hard)
byte commandPuffSoft[maxCommandLength];  // Command to execute on puff (soft)
byte commandPuffHard[maxCommandLength];  // Command to execute on puff (hard)

// Input mode
const byte MODE_BASIC = 0;  // Basic mode = only sip and puff
const byte MODE_FINE = 1;   // Fine mode = hard and soft sips/puffs
byte mode;                  // Current mode;

// Serial communication variables
byte etIn[3];               // Incoming handshake buffer
byte et[] = {'E','T','?'};  // Key to check handshake against

void setup() {
  // Set up the sensor
  pinMode(sensorPin, INPUT);
  
  // Initialize the various serial streams
  Serial.begin(9600);
  Keyboard.begin();
  Mouse.begin();
  
  // Write some fake commands to EEPROM
  // Address 0 = mode flag
  EEPROM.write(0,'F');  // Basic mode
  
  // Sip (hard) = press return key
  // S,H,K,1,32,255
  EEPROM.write(1,'S');  // Sip
  EEPROM.write(2,'H');  // Hard
  EEPROM.write(3,'K');  // Keyboard
  EEPROM.write(4,1);    // 1 parameter
  EEPROM.write(5,KEY_RETURN);  // h
  EEPROM.write(6,255);  // End
  
  // Sip (soft) = Alt + Tab
  // S,S,K,1,50,255
  EEPROM.write(7,'S');           // Sip
  EEPROM.write(8,'S');           // Soft
  EEPROM.write(9,'K');           // Keyboard
  EEPROM.write(10,2);             // 1 parameter
  EEPROM.write(11,KEY_LEFT_ALT); // Left alt
  EEPROM.write(12,KEY_TAB);      // Tab
  EEPROM.write(13,255);          // End

  // Puff (hard) = click and drag
  // P,H,M,D,10,0,0,255
  EEPROM.write(14,'P');  // Puff
  EEPROM.write(15,'H');  // Hard
  EEPROM.write(16,'M');  // Mouse
  EEPROM.write(17,'D');  // Click
  EEPROM.write(18,1);    // Left click
  EEPROM.write(19,10);   // x = 40
  EEPROM.write(20,0);    // y = 0
  EEPROM.write(21,0);    // scroll = 0
  EEPROM.write(22,255);  // End
  
  // Puff (soft) = Alt + F4
  // P,S,K,2,KEY_LEFT_ALT,KEY_F4
  EEPROM.write(23,'P');
  EEPROM.write(24,'S');
  EEPROM.write(25,'K');
  EEPROM.write(26,1);
//  EEPROM.write(27,KEY_LEFT_ALT);
//  EEPROM.write(28,KEY_F4);
  EEPROM.write(27,'a');
  EEPROM.write(28,255);
  
  delay(4000); 
  loadCommands();
}

void loop() {
  processSerial();   // Process incoming serial communication
  readSensor();      // Take readings from the sensor
  processSensor();   // Look for "sips" and "puffs" in the sensor readings
  
  // Sip has been detected
  if(sipDetected) {
    // Hard sip detected
    if(currentPeak <= sensorBaseValue - hardThreshold) {
      Serial.print("# Sip (hard) detected - ");
      
      switch(mode) {
        case MODE_BASIC:
          executeCommand(commandSip);
          break;
        case MODE_FINE:
          executeCommand(commandSipHard);
          break;
      }
      
    // Soft sip detected
    } else if(currentPeak <= sensorBaseValue - softThreshold) {
      Serial.print("# Sip (soft) detected - ");
      
      switch(mode) {
        case MODE_BASIC:
          executeCommand(commandSip);
          break;
        case MODE_FINE:
          executeCommand(commandSipSoft);
          break;
      }
    }
      
    Serial.println(currentPeak);
    currentPeak = sensorBaseValue;
    sipDetected = false;
  }
  
  // Puff has been detected
  if(puffDetected) {
    // Hard puff detected
    if(currentPeak >= sensorBaseValue + hardThreshold) {
      Serial.print("# Puff (hard) detected - ");
      
      switch(mode) {
        case MODE_BASIC:
          executeCommand(commandPuff);
          break;
        case MODE_FINE:
          executeCommand(commandPuffHard);
          break;
      }
      
    // Soft puff detected
    } else if(currentPeak >= sensorBaseValue + softThreshold) {
      Serial.print("# Puff (soft) detected - ");
      
      switch(mode) {
        case MODE_BASIC:
          executeCommand(commandPuff);
          break;
        case MODE_FINE:
          executeCommand(commandPuffSoft);
          break;
      }
    }
    
    Serial.println(currentPeak);
    currentPeak = sensorBaseValue;
    puffDetected = false;
  }
  
  // Output the raw sensor readings
  if(debug)
    Serial.println(sensorAverage);
}

/* ========================================================================================
   SENSOR FUNCTIONS
============================================================================================ */

/*************************************************
 readSensor()
 
 Read sensor value several times, store average 
 in globally-accessible variable.
**************************************************/
void readSensor() {
  // Take a few readings
  for(int i=0; i<numReadings; i++) {
    int v = analogRead(sensorPin);
    sensorValues[i] = v;
  }
  
  // Find average of all readings
  int total = 0;
  for(int i=0; i<numReadings; i++)
    total += sensorValues[i];
    
  sensorAverage = total/numReadings;
}

/*****************************************************
 processSensor()
 
 Derive sip and puff activity from sensor data
******************************************************/
void processSensor() {
  // While data is hovering in safe zone ...
  if(sipThresholdCrossed == false && puffThresholdCrossed == false) {
    
    // Wait for the data to cross the basic threshold
    if(sensorAverage <= sensorBaseValue - basicThreshold) {
      sipThresholdCrossed = true;
      currentPeak = sensorAverage;
    } else if(sensorAverage >= sensorBaseValue + basicThreshold) {
      puffThresholdCrossed = true;
      currentPeak = sensorAverage;
    }
    
  // If data has drifted above or below safe zone
  } else {
    
    // Record peaks
    if(sipThresholdCrossed)
      if(sensorAverage < currentPeak)
        currentPeak = sensorAverage;
        
    if(puffThresholdCrossed)
      if(sensorAverage > currentPeak)
        currentPeak = sensorAverage;
    
    // Check if the data has returned to safe zone, then trigger a sip or puff
    if(sensorAverage <= sensorBaseValue + basicThreshold && sensorAverage >= sensorBaseValue - basicThreshold) {
      // Sip detected
      if(sipThresholdCrossed) {
        sipDetected = true;
        sipThresholdCrossed = false;
      }
      
      // Puff detected
      if(puffThresholdCrossed) {
        puffDetected = true;
        puffThresholdCrossed = false;
      }
    }
  }
}

/* ========================================================================================
   COMMAND FUNCTIONS
============================================================================================ */

/************************************************************
 loadCommands()
 
 Loads all commands stored in EEPROM
*************************************************************/
void loadCommands() {
  // Read mode flag (always at address 0)
  value = EEPROM.read(0);
  switch(value) {
    case 'B':
      mode = MODE_BASIC;
      Serial.println("## Current mode: basic");
      break;
    case 'F':
      mode = MODE_FINE;
      Serial.println("## Current mode: fine");
      break;
  }
  
  // Read all commands from EEPROM
  do {
    Serial.print("## Command: ");
    
    // Read one command from EEPROM
    do {
      // Read first value
      value = EEPROM.read(currentAddress);
      incomingCommand[commandIndex] = value;
      
      // Output appropriate chars for readability
      if(commandIndex == 0 || commandIndex == 1 || commandIndex == 2) {
        Serial.print((char)value);
      } else {
        if(commandIndex == 3 && incomingCommand[commandIndex-1] == 'M')
          Serial.print((char)value);
        else
          Serial.print(value);
      }
      
      // Add end of line or delimiter
      if(value == 255)
        Serial.println();
      else
        Serial.print(", ");
    
      // Advance the array index and the EEPROM address    
      commandIndex++;
      currentAddress++;
    } while(value != 255);
    
    // Assign command to actionable trigger
    switch(incomingCommand[0]) {
      // S = sip
      case 'S':
        switch(incomingCommand[1]) {
          // S = soft
          case 'S':
            for(int i=0; i<maxCommandLength; i++)
              commandSipSoft[i] = incomingCommand[i];
              
            // Also assign to basic mode sip in case of mode switch
            for(int i=0; i<maxCommandLength; i++)
              commandSip[i] = commandSipSoft[i];
              
            break;
            
          // H = hard
          case 'H':
            for(int i=0; i<maxCommandLength; i++)
              commandSipHard[i] = incomingCommand[i];            
            break;
        }
        break;
        
      // P = puff
      case 'P':
        switch(incomingCommand[1]) {
          // S = soft
          case 'S':
            for(int i=0; i<maxCommandLength; i++)
              commandPuffSoft[i] = incomingCommand[i];
              
            // Also assign to basic mode puff in case of mode switch
            for(int i=0; i<maxCommandLength; i++)
              commandPuff[i] = commandPuffSoft[i];
              
            break;
            
          // H = hard
          case 'H':
            for(int i=0; i<maxCommandLength; i++)
              commandPuffHard[i] = incomingCommand[i];
            break;
        }
        break;
    }
    
    // Look ahead for other commands
    nextValue = EEPROM.read(currentAddress);
    commandIndex = 0;
  } while(nextValue == 'S' || nextValue == 'P');
  
  // Reset currentAddress to beginning (skip mode flag at 0)
  currentAddress = 1;
}

/**************************************************************
 executeCommand(byte command[])
 
 Execute actionable command. Called via triggers in loop()
***************************************************************/
void executeCommand(byte command[]) {
  switch(command[2]) {
    // K = keyboard
    case 'K':
      // Only one key to press = write it immediately
      if(command[3] == 1) {
        Keyboard.write(command[4]);
        
      // Multiple keys = press each, then release all
      } else if(command[3] >= 2) {
        for(int i=0; i<=command[3]; i++)
          Keyboard.press(command[4 + i]);
        
        delay(500);
        Keyboard.releaseAll();
      }
      break;
      
    // M = mouse
    case 'M':
      switch(command[3]) {
        // C = click
        case 'C':
          Mouse.click(command[4]);
          break;
          
        // M = move
        case 'M':
          Mouse.move(command[4], command[5], command[6]);
          break;
          
        // D = drag
        case 'D':
          Mouse.press(command[4]);
          Mouse.move(command[5], command[6], command[7]);
          Mouse.release();
          break;
      }
      break;
  }
}

/* ========================================================================================
   SERIAL FUNCTIONS
============================================================================================ */

/*********************************************
 processSerial()
 
 Listen for serial messages like handshakes 
 and commmands
**********************************************/
void processSerial() {
  // Listen for handshake
  while(Serial.available() > 0) {
    etIn[0] = Serial.read();
    
    // E = beginning of handshake
    switch(etIn[0]) {
      case 'E':
        etIn[1] = Serial.read();  // T
        etIn[2] = Serial.read();  // ?
        Serial.read();            // throw away space
        etIn[3] = Serial.read();  // COM index (single digit)
  
        // Check that "ET?" has been sent, send acknowledgement if so
        if(etIn[0] == et[0] && etIn[1] == et[1] && etIn[2] == et[2]) {
          Serial.print("Phone home|");
          Serial.println((char)etIn[3]);  // COM port index
        }
    }
  }
}

/* ========================================================================================
   EEPROM FUNCTIONS
============================================================================================ */
// Remove everything currently stored in EEPROM
void clearEEPROM() {
  for(int i=0; i<1024; i++)
    EEPROM.write(i, 0);
}

void readEEPROM() {
  
}

void setEEPROM() {
  
}
