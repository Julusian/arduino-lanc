/*
SIMPLE LANC REMOTE
Version 1.0
Sends LANC commands to the LANC port of a video camera.
Tested with a Canon XF300 camcorder
For the interface circuit interface see 
http://controlyourcamera.blogspot.com/2011/02/arduino-controlled-video-recording-over.html
Feel free to use this code in any way you want.
2011, Martin Koch

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
    Serial.print(b1);
    Serial.print("-");
    Serial.println(b2);
}

