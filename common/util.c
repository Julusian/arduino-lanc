int scanByte(bool skipDelay){
  int result = 0;

  // skip START BIT
  if (!skipDelay){
    delayMicroseconds(bitDuration*1.5);
  }

  // read byte of data
  for (int i=0; i<7; i++) {
    result += digitalRead(inputPin) << i;
    delayMicroseconds(readBitDuration);
  }
  result += digitalRead(inputPin) << 7;

  // shift into stop 'bit'
  delayMicroseconds(10); 

  return result ^ 0xFF;
}

void writeByte(int data){
    delayMicroseconds(bitDuration);  // wait START bit duration

    for (int i=0; i<8; i++) {
      digitalWrite(outputPin, bitRead(data, i));
      delayMicroseconds(writeBitDuration); 
    }
}

void printHex(int num, int precision) {
     char tmp[16];
     char format[128];

     sprintf(format, "%%.%dX", precision);

     sprintf(tmp, format, num);
     Serial.print(tmp);
}