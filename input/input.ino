/*
ARDUINO-LANC Input (from controller)
https://github.com/Julusian/arduino-lanc

Outputs commands in the format: #28-0E

For a list of commands see http://www.boehmel.de/lanc.htm

Based on the work by Martin Koch
http://controlyourcamera.blogspot.com/2011/02/arduino-controlled-video-recording-over.html

"LANC" is a registered trademark of SONY.
CANON calls their LANC compatible port "REMOTE".
*/

#include "config.h"
#include "../common/util.c"

#define outputBitCount 6
int outputBytes[outputBitCount];

void setup() {
  pinMode(inputPin, INPUT); //listens to the LANC line
  pinMode(outputPin, OUTPUT); //writes to the LANC line
  
  digitalWrite(outputPin, 0); //set LANC line to +5V

  Serial.begin(115200);
}

void writeStopBit(){
  delayMicroseconds(bitDuration*3);
}

void writeStartBit(){
  digitalWrite(outputPin, 1);
  delayMicroseconds(writeBitDuration);
  digitalWrite(outputPin, 0);
}

int readByte(){
  writeStartBit();
  // Wait until halfway through the bits lifetime on the wire
  delayMicroseconds(readBitDuration/2);
  int b1 = scanByte(true);
  writeStopBit();
  return b1;
}

void loop() {
    // TODO - read outputBytes from serial

    // TODO - add a rate limit to ensure timing is correct for lanc protocol
    delay(7);

    int b1 = readByte();
    int b2 = readByte();

    // Write the 'camera' response
    for(int i=0; i<outputBitCount; i++){
      writeStartBit();
      writeByte(outputBytes[i]);
      writeStopBit();
    }

    // Send retrieved data
    Serial.print("#");
    printHex(b1, 2);
    Serial.print("-");
    printHex(b2, 2);
    Serial.println("");
}

