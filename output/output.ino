/*
ARDUINO-LANC Output (to camera)
https://github.com/Julusian/arduino-lanc

Accepts commands in the format: #28-0E
Responds with OK after command has been fully written to the camera

For a list of commands see http://www.boehmel.de/lanc.htm

Based on the work by Martin Koch
http://controlyourcamera.blogspot.com/2011/02/arduino-controlled-video-recording-over.html

"LANC" is a registered trademark of SONY.
CANON calls their LANC compatible port "REMOTE".
*/

#include <stdlib.h>

#include "config.h"
#include "../common/util.c"

void setup() {

  pinMode(inputPin, INPUT); //listens to the LANC line
  pinMode(outputPin, OUTPUT); //writes to the LANC line
  
  digitalWrite(outputPin, 0); //set LANC line to +5V
//   delay(5000); //Wait for camera to power up completly

  Serial.begin(115200);
  Serial.println("READY");
}


void lancCommand(int mode, int cmd) {
  int cmdRepeatCount = 0;

  while (cmdRepeatCount < cmdRepeatTimes) {  //repeat to make sure the camera accepts the command

    while (pulseIn(inputPin, 1) < 5000) {   
      //"pulseIn, 1" catches any 0V TO +5V TRANSITION and waits until the LANC line goes back to 0V 
      //"pulseIn" also returns the pulse duration so we can check if the previous +5V duration was long enough (>5ms) to be the pause before a new 8 byte data packet
      //Loop till pulse duration is >5ms
    }

    writeByte(mode);
   
    //Byte 0 is written now put LANC line back to +5V
    digitalWrite(outputPin, 0);
    delayMicroseconds(10); //make sure to be in the stop bit before byte 1
  
    while (digitalRead(inputPin)) { 
      //Loop as long as the LANC line is +5V during the stop bit
    }

    writeByte(cmd);

    //Byte 1 is written now put LANC line back to +5V
    digitalWrite(outputPin, 0); 

    cmdRepeatCount++;

    // TODO - read remaining 6 bytes and send back to host 
  }
}

unsigned char h2d(unsigned char hex)
{
    if(hex > 0x39) hex -= 7; // adjust for hex letters upper or lower case
    return(hex & 0xf);
}

void loop() {
  if (Serial.available() >= 6) {
    // read the incoming byte:
    char header = Serial.read();
    if (header != '#')
      return;

    char b1 = Serial.read();
    char b2 = Serial.read();
    
    char spacer = Serial.read(); // spacer
    if (spacer != '-')
      return;

    char b3 = Serial.read();
    char b4 = Serial.read(); 

    int mode = (h2d(b1) << 4) | h2d(b2);
    int cmd = (h2d(b3) << 4) | h2d(b4);

    if (mode < 0 || mode > 255 || cmd < 0 || cmd > 255)
      return;
    
    // send command
    lancCommand(mode, cmd);

    // TODO - report response, rather than OK
    Serial.println("OK");
  }
}