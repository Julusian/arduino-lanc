#define bitDuration 104

//Reading to the digital port takes about 4 microseconds so only 100 microseconds are left for each bit
#define digitalReadDuration 4
#define readBitDuration (bitDuration-digitalReadDuration)

//Writing to the digital port takes about 8 microseconds so only 96 microseconds are left for each bit
#define digitalWriteDuration 8
#define writeBitDuration (bitDuration-digitalWriteDuration)

// NOTE: micros takes about 4us
