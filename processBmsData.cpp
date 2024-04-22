#include "processBmsData.h"

void processBmsData(uint8_t dataID){
  switch(dataID){
    case 0x90:
      {
        int pressure = ( (rxBuffers.singlePacketData[0] << 8 ) | rxBuffers.singlePacketData[1] )/ 10 ;
        int acquisition = ( (rxBuffers.singlePacketData[2] << 8 ) | rxBuffers.singlePacketData[3] )/ 10 ;
        int totalCurrent = ( ( (rxBuffers.singlePacketData[4] << 8 ) | rxBuffers.singlePacketData[5] ) - 30000) / 10 ;
        int SOC = ( (rxBuffers.singlePacketData[6] << 8) | rxBuffers.singlePacketData[7] ) / 10 ;

        //logging the collected info:
        Serial.print("Pressure: ");
        Serial.print(pressure);
        Serial.print(" Acquisition: ");
        Serial.print(acquisition);
        Serial.print(" Total Current: ");
        Serial.print(totalCurrent);
        Serial.print(" SOC: ");
        Serial.println(SOC);
      break;
      }
    case 0x91:
      {
      int maxVoltage = (rxBuffers.singlePacketData[0] << 8 ) | rxBuffers.singlePacketData[1];
      int maxVoltageCell = rxBuffers.singlePacketData[2];
      int minVoltage = (rxBuffers.singlePacketData[3] << 8 ) | rxBuffers.singlePacketData[4];
      int minVoltageCell = rxBuffers.singlePacketData[5];
      
      //logging the collected info:
      // Serial.print("Max voltage: ");
      // Serial.print(maxVoltage);
      // Serial.print(" Max V cell no: ");
      // Serial.print(maxVoltageCell);
      // Serial.print(" Min voltage: ");
      // Serial.print(minVoltage);
      // Serial.print(" Min V cell no: ");
      // Serial.println(minVoltageCell);

      break;
      }
    case 0x93:
      {
      
      break;
      }
  }
}

void resetRxBuffers(){
  rxBuffers.bufferIndex = 0;
}
