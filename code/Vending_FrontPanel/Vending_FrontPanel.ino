#include "SPI.h"
#include "Adafruit_WS2801.h"
#include <Wire.h>
#include "Adafruit_MCP23017.h"

#define STATE_WAITING 0
#define STATE_LEDNUM 1
#define STATE_RED_HIGHNIBBLE 2
#define STATE_RED_LOWNIBBLE 3
#define STATE_GREEN_HIGHNIBBLE 4
#define STATE_GREEN_LOWNIBBLE 5
#define STATE_BLUE_HIGHNIBBLE 6
#define STATE_BLUE_LOWNIBBLE 7

Adafruit_WS2801 strip = Adafruit_WS2801(16);
uint8_t LEDMap[] = {12, 13, 14, 15, 11, 10, 9, 8, 4, 5, 6, 7, 3, 2, 1, 0};
Adafruit_MCP23017 ioexp;
//uint8_t IOMap[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
uint8_t IOMap[] = {7, 6, 9, 8, 5, 4, 11, 10, 3, 2, 13, 12, 1, 0, 15, 14};
uint8_t IOOnStates[] = {')', '!', '@', '#', '$', '%', '^', '&', '*', '(', 'A', 'B', 'C', 'D', 'E', 'F'};
uint8_t IOOffStates[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

void setLED(uint8_t LEDNum, uint8_t Red, uint8_t Green, uint8_t Blue);

void setup() {
  Serial.begin(9600);
  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();
  
  pinMode(12, INPUT);
  
  ioexp.begin();
  
  for(uint8_t i = 0; i < 16; i++) {
    ioexp.pinMode(i, INPUT);
    ioexp.pullUp(i, HIGH);
  }
}


void loop() {
  char recChar;
  static char serialState = STATE_WAITING;
  static char redVal = 0, greenVal = 0, blueVal = 0, ledNum = 0;
  
  if(Serial.available() > 0) {
    recChar = Serial.read();
    
    if(recChar == 'R') {
      // Reset the state machine
      serialState = STATE_WAITING;
    } else {
      switch(serialState) {
        case STATE_WAITING:
          switch(recChar) {
            case 'S':
              sendSwitchState();
              serialState = STATE_WAITING;
              break;
            case 'P':
              sendPushbuttonState();
              serialState = STATE_WAITING;
              break;
            case 'L':
              serialState = STATE_LEDNUM;
              break;
            case 'I':
              sendVersionInfo();
              serialState = STATE_WAITING;
              break;
            default:
              serialState = STATE_WAITING;
              break;
          }
          break;
        case STATE_LEDNUM:
          // this is the LED number
          ledNum = ConvertCharToHex(recChar);
          serialState = STATE_RED_HIGHNIBBLE;
          break;
        case STATE_RED_HIGHNIBBLE:
          redVal = ConvertCharToHex(recChar) * 16;
          serialState = STATE_RED_LOWNIBBLE;
          break;
        case STATE_RED_LOWNIBBLE:
          redVal += ConvertCharToHex(recChar);
          serialState = STATE_GREEN_HIGHNIBBLE;
          break;
        case STATE_GREEN_HIGHNIBBLE:
          greenVal = ConvertCharToHex(recChar) * 16;
          serialState = STATE_GREEN_LOWNIBBLE;
          break;
        case STATE_GREEN_LOWNIBBLE:
          greenVal += ConvertCharToHex(recChar);
          serialState = STATE_BLUE_HIGHNIBBLE;
          break;
        case STATE_BLUE_HIGHNIBBLE:
          blueVal = ConvertCharToHex(recChar) * 16;
          serialState = STATE_BLUE_LOWNIBBLE;
          break;
        case STATE_BLUE_LOWNIBBLE:
          blueVal += ConvertCharToHex(recChar);
          setLED(ledNum, redVal, greenVal, blueVal);
          serialState = STATE_WAITING;
          break;
        default:
          serialState = STATE_WAITING;
          break;
      }
    } 
  }
}


void setLED(uint8_t LEDNum, uint8_t Red, uint8_t Green, uint8_t Blue) {
  strip.setPixelColor(LEDMap[LEDNum], Red, Green, Blue);
  strip.show();
  Serial.println('L');
}

void sendSwitchState() {
  Serial.print('S');
  for(uint8_t i = 0; i< 16; i++) {
    if(ioexp.digitalRead(IOMap[i])) {
      Serial.write(IOOnStates[i]);
    } else {
      Serial.write(IOOffStates[i]);
    }
  }
  Serial.print("\n");
}

void sendPushbuttonState() {
  if(digitalRead(12)) {
    Serial.println('P');
  } else {
    Serial.println('p');
  }
}

void sendVersionInfo() {
  Serial.println(F("VendingPanelV1.0"));
}

uint8_t ConvertCharToHex(char c) {
  if((c >= 'A') && (c <= 'F')) {
    return (c - 'A') + 10;
  } else if((c >= '0') && (c <= '9')) {
    return (c - '0');
  } else {
    return 0;
  }
}
