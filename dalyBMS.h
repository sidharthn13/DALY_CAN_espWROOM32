#include<Arduino.h>
#ifndef STRUCT_DEFINITION
#define STRUCT_DEFINITION

typedef enum{
  s0,
  s0_idle,
  s1,
  s1_idle,
  s2,
  s2_idle,
  s3,
  s3_idle,
  s4,
  s4_idle,
  s5,
  s5_idle,
  s6,
  s6_idle,
  s7,
  s7_idle,
  s8,
  s8_idle,
  s9,
}STATE;

typedef struct RxBuffers{
  uint8_t packetData[8];
  uint8_t bufferIndex;
  uint8_t multiPacketData[96];
  uint8_t multiPacketBufferIndex;
}RxBuffers;

typedef struct bmsData{

  //0x90 data:
  float pressureVoltage;
  float acquisitionVoltage;
  float totalCurrent;
  float SOC;

  //0x91 data:
  float maxCellVoltage;
  uint8_t maxCellVoltNum;
  float minCellVoltage;
  uint8_t minCellVoltNum;

  //0x92 data:
  float maxMonomerTemp;
  float minMonomerTemp;
  uint8_t maxMonomerTempNum;
  uint8_t minMonomerTempNum;

  //0x93 data:
  uint8_t chargeDischargeStatus;
  uint8_t chargingMosTubeState;
  uint8_t dischargeMosTubeState;
  uint8_t bmsLife;
  uint32_t residualCapacity;

  //0x94 data:
  uint8_t batteryString;
  uint8_t temperatures;

  //0x95 data:
  uint16_t cellVoltages[48];
  uint8_t cellVoltagesIndex;

  //0x96 data:
  int32_t monomerTemps[16];
  uint8_t monomerTempsIndex;
}bmsData;

#endif
