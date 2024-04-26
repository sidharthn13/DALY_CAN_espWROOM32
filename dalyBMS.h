#include<Arduino.h>
#ifndef STRUCT_DEFINITION
#define STRUCT_DEFINITION
typedef struct RxBuffers{
  uint8_t packetData[8];
  uint8_t bufferIndex;
}RxBuffers;

typedef struct bmsData{
  uint8_t batteryString;

  uint16_t cellVoltages[48];
  uint8_t cellVoltagesIndex;

  float monomerTemps[16];
  uint8_t monomerTempsIndex;
}bmsData;
#endif
