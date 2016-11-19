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

  while (cmdRepeatCount < 50) {  //repeat 5 times to make sure the camera accepts the command

    while (pulseIn(inputPin, 1) < 5000) {   
      //"pulseIn, 1" catches any 0V TO +5V TRANSITION and waits until the LANC line goes back to 0V 
      //"pulseIn" also returns the pulse duration so we can check if the previous +5V duration was long enough (>5ms) to be the pause before a new 8 byte data packet
      //Loop till pulse duration is >5ms
    }

    //0 after long pause means the START bit of Byte 0 is here
    delayMicroseconds(bitDuration);  //wait START bit duration

    //Write the 8 bits of byte 0 
    //Note that the command bits have to be put out in reverse order with the least significant, right-most bit (bit 0) first
    for (int i=0; i<8; i++) {
      digitalWrite(outputPin, bitRead(mode, i));  //Write bits. 
      delayMicroseconds(writeBitDuration); 
    }
   
    //Byte 0 is written now put LANC line back to +5V
    digitalWrite(outputPin, 0);
    delayMicroseconds(10); //make sure to be in the stop bit before byte 1
  
    while (digitalRead(inputPin)) { 
      //Loop as long as the LANC line is +5V during the stop bit
    }

    //0V after the previous stop bit means the START bit of Byte 1 is here
    delayMicroseconds(bitDuration);  //wait START bit duration

    //Write the 8 bits of Byte 1
    //Note that the command bits have to be put out in reverse order with the least significant, right-most bit (bit 0) first
    for (int i=0; i<8; i++) {
      digitalWrite(outputPin, bitRead(cmd, i));  //Write bits 
      delayMicroseconds(writeBitDuration);
    }

    //Byte 1 is written now put LANC line back to +5V
    digitalWrite(outputPin, 0); 

    cmdRepeatCount++;  //increase repeat count by 1
   
    /*Control bytes 0 and 1 are written, now don’t care what happens in Bytes 2 to 7
    and just wait for the next start bit after a long pause to send the first two command bytes again.*/

    // TODO - read remaining 6 bytes and send back to host 
  }
}

unsigned char h2d(unsigned char hex)
{
    if(hex > 0x39) hex -= 7; // adjust for hex letters upper or lower case
    return(hex & 0xf);
}

void loop() {
    // Serial.print(Serial.available());
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

    int i1 = (h2d(b1) << 4) | h2d(b2);
    int i2 = (h2d(b3) << 4) | h2d(b4);

    Serial.print(i1);
    Serial.print("-");
    Serial.println(i2);

    if (i1 < 0 || i1 > 255 || i2 < 0 || i2 > 255)
      return;

    lancCommand(i1, i2);

    // TODO - report response, rather than OK
    Serial.println("OK");
  }
}