#include "processBmsData.h"

void processBmsData(uint8_t dataID){
  switch(dataID){
    case 0x90:
      break;
    case 0x91:
      {
      int maxVoltage = rxBuffers.monomerVoltage[7] | (rxBuffers.monomerVoltage[6] << 8);
      int maxVoltageCell = rxBuffers.monomerVoltage[5];
      int minVoltage = rxBuffers.monomerVoltage[4] | (rxBuffers.monomerVoltage[3] << 8);
      int minVoltageCell = rxBuffers.monomerVoltage[2];
      Serial.print("Max voltage : ");
      Serial.print(maxVoltage);
      Serial.print(" Cell number : ");
      Serial.print(maxVoltageCell);
      Serial.print(", Min voltage : ");
      Serial.print(minVoltage);
      Serial.print(" Cell number : ");
      Serial.println(minVoltageCell);
      break;
      }
    case 0x93:
      {
      int chargeDischargeStatus = rxBuffers.MosStatus[7];
      int chargingMosTubeStatus = rxBuffers.MosStatus[6];
      int dischargingMosTubeStatus = rxBuffers.MosStatus[5];
      int bmsLife = rxBuffers.MosStatus[4];
      int residualCapacity = rxBuffers.MosStatus[3] | (rxBuffers.MosStatus[2] << 8 ) | (rxBuffers.MosStatus[1] << 16 );
      Serial.print("Charge/Discharge status : ");
      Serial.print(chargeDischargeStatus);
      Serial.print(", Charging MOS tube status : ");
      Serial.print(chargingMosTubeStatus);
      Serial.print(", Discharging MOS tube status : ");
      Serial.print(dischargingMosTubeStatus);
      Serial.print(", BMS life cycles : ");
      Serial.print(bmsLife);
      Serial.print(", Residual Capacity : ");
      Serial.println(residualCapacity);
      break;
      }
  }
}