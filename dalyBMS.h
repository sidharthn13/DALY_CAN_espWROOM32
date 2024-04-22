#include<Arduino.h>
#ifndef STRUCT_DEFINITION
#define STRUCT_DEFINITION
typedef struct RxBuffers{
  uint8_t packetData[130];
  uint8_t bufferIndex;
}RxBuffers;
#endif