#include<Arduino.h>
#ifndef STRUCT_DEFINITION
#define STRUCT_DEFINITION
typedef struct RxBuffers{
  uint8_t singlePacketData[8];
  uint8_t multiPacketData[130];
  uint8_t bufferIndex;
}RxBuffers;
#endif