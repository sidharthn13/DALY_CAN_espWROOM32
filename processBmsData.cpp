#include "processBmsData.h"

void processBmsData(uint8_t dataID){
  switch(dataID){
    case 0x90:
      {
        float pressure = (float)( (rxBuffers.singlePacketData[0] << 8 ) | rxBuffers.singlePacketData[1] )/ 10 ;
        float acquisition = (float)( (rxBuffers.singlePacketData[2] << 8 ) | rxBuffers.singlePacketData[3] )/ 10 ;
        float totalCurrent = ( (float)( (rxBuffers.singlePacketData[4] << 8 ) | rxBuffers.singlePacketData[5] ) - 30000) / 10 ;
        float SOC = (float)( (rxBuffers.singlePacketData[6] << 8) | rxBuffers.singlePacketData[7] ) / 10 ;

        //logging the collected info:
        // Serial.print("Pressure: ");
        // Serial.print(pressure);
        // Serial.print(" Acquisition: ");
        // Serial.print(acquisition);
        // Serial.print(" Total Current: ");
        // Serial.print(totalCurrent);
        // Serial.print(" SOC: ");
        // Serial.println(SOC);
      break;
      }
    case 0x91:
      {
      float maxVoltage = (float)((rxBuffers.singlePacketData[0] << 8 ) | rxBuffers.singlePacketData[1]);
      float maxVoltageCell = (float)rxBuffers.singlePacketData[2];
      float minVoltage = (float)((rxBuffers.singlePacketData[3] << 8 ) | rxBuffers.singlePacketData[4]);
      float minVoltageCell = (float)rxBuffers.singlePacketData[5];
      
      // logging the collected info:
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
    case 0x92:
      {
        int maxTemperature = rxBuffers.singlePacketData[0] - 40; 
        int maxTempMonomer = rxBuffers.singlePacketData[1];
        int minTemperature = rxBuffers.singlePacketData[2] - 40;
        int minTempMonomer = rxBuffers.singlePacketData[3];

        //logging the collected info:
        // Serial.print("Maximum monomer temperature: ");
        // Serial.print(maxTemperature);
        // Serial.print(" Max temp monomer no: ");
        // Serial.print(maxTempMonomer); 
        // Serial.print("Maximum monomer temperature: ");
        // Serial.print(minTemperature);
        // Serial.print(" Max temp monomer no: ");
        // Serial.println(minTempMonomer); 
        
        break;
      }
    case 0x93:
      {
        int chargeDischargeStatus = rxBuffers.singlePacketData[0];
        int chargingMosTubeState = rxBuffers.singlePacketData[1];
        int dischargeMosTubeState = rxBuffers.singlePacketData[2];
        int bmsLife = rxBuffers.singlePacketData[3];
        int residualCapacity = (rxBuffers.singlePacketData[4] << 24) | (rxBuffers.singlePacketData[5] << 16) |
        (rxBuffers.singlePacketData[6] << 8) | rxBuffers.singlePacketData[7];

        //logging the collected info:
        Serial.print("Charge-Discharge status: ");
        Serial.print(chargeDischargeStatus);
        Serial.print(" Charging MOS tube status: ");
        Serial.print(chargingMosTubeState);
        Serial.print(" Discharging MOS tube status");
        Serial.print(dischargeMosTubeState);
        Serial.print(" BMS life cycles: ");
        Serial.print(bmsLife);
        Serial.print(" Residual capacity: ");
        Serial.println(residualCapacity);

        break;
      }
  }
}

void resetRxBuffers(){
  rxBuffers.bufferIndex = 0;
}
