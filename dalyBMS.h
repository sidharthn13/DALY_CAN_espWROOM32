#include<Arduino.h>
#ifndef STRUCT_DEFINITION
#define STRUCT_DEFINITION
typedef struct RxBuffers{
  uint8_t SocVoltage[8];
  uint8_t monomerVoltage[8];
  uint8_t MosStatus[8];
}RxBuffers;
#endif