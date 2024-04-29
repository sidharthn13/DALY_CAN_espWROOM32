#include<Arduino.h>
#ifndef STRUCT_DEFINITION
#define STRUCT_DEFINITION
typedef struct RxBuffers{
  uint8_t packetData[8];
  uint8_t bufferIndex;
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

  //0x95 data:
  uint16_t cellVoltages[48];
  uint8_t cellVoltagesIndex;

  //0x96 data:
  float monomerTemps[16];
  uint8_t monomerTempsIndex;
}bmsData;

#endif
