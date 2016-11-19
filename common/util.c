int scanByte(bool skipDelay){
  int result = 0;

  // skip START BIT
  if (!skipDelay){
    delayMicroseconds(bitDuration*1.5);
  }

  // read byte of data
  result += digitalRead(inputPin) << 0;
  delayMicroseconds(readBitDuration);
  result += digitalRead(inputPin) << 1;
  delayMicroseconds(readBitDuration);
  result += digitalRead(inputPin) << 2;
  delayMicroseconds(readBitDuration);
  result += digitalRead(inputPin) << 3;
  delayMicroseconds(readBitDuration);
  result += digitalRead(inputPin) << 4;
  delayMicroseconds(readBitDuration);
  result += digitalRead(inputPin) << 5;
  delayMicroseconds(readBitDuration);
  result += digitalRead(inputPin) << 6;
  delayMicroseconds(readBitDuration);
  result += digitalRead(inputPin) << 7;

  // shift into stop 'bit'
  delayMicroseconds(10); 

  return result ^ 0xFF;
}

void writeByte(int data){
    //0 after long pause means the START bit of Byte 0 is here
    delayMicroseconds(bitDuration);  //wait START bit duration

    //Write the 8 bits of byte 0 
    //Note that the command bits have to be put out in reverse order with the least significant, right-most bit (bit 0) first
    for (int i=7; i>-1; i--) {
      digitalWrite(outputPin, data >> i);  //Write bits. 
      delayMicroseconds(writeBitDuration); 
    }
}

