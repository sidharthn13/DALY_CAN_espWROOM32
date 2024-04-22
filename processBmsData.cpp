#include "esp_spi_flash.h"
#include "processBmsData.h"

void processBmsData(uint8_t dataID){
  switch(dataID){
    case 0x90:
      {
        float pressure = (float)( (rxBuffers.packetData[0] << 8 ) | rxBuffers.packetData[1] )/ 10 ;
        float acquisition = (float)( (rxBuffers.packetData[2] << 8 ) | rxBuffers.packetData[3] )/ 10 ;
        float totalCurrent = ( (float)( (rxBuffers.packetData[4] << 8 ) | rxBuffers.packetData[5] ) - 30000) / 10 ;
        float SOC = (float)( (rxBuffers.packetData[6] << 8) | rxBuffers.packetData[7] ) / 10 ;

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
      float maxVoltage = (float)((rxBuffers.packetData[0] << 8 ) | rxBuffers.packetData[1]);
      float maxVoltageCell = (float)rxBuffers.packetData[2];
      float minVoltage = (float)((rxBuffers.packetData[3] << 8 ) | rxBuffers.packetData[4]);
      float minVoltageCell = (float)rxBuffers.packetData[5];
      
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
        int maxTemperature = rxBuffers.packetData[0] - 40; 
        int maxTempMonomer = rxBuffers.packetData[1];
        int minTemperature = rxBuffers.packetData[2] - 40;
        int minTempMonomer = rxBuffers.packetData[3];

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
        int chargeDischargeStatus = rxBuffers.packetData[0];
        int chargingMosTubeState = rxBuffers.packetData[1];
        int dischargeMosTubeState = rxBuffers.packetData[2];
        int bmsLife = rxBuffers.packetData[3];
        int residualCapacity = (rxBuffers.packetData[4] << 24) | (rxBuffers.packetData[5] << 16) |
        (rxBuffers.packetData[6] << 8) | rxBuffers.packetData[7];

        //logging the collected info:
        // Serial.print("Charge-Discharge status: ");
        // Serial.print(chargeDischargeStatus);
        // Serial.print(" Charging MOS tube status: ");
        // Serial.print(chargingMosTubeState);
        // Serial.print(" Discharging MOS tube status");
        // Serial.print(dischargeMosTubeState);
        // Serial.print(" BMS life cycles: ");
        // Serial.print(bmsLife);
        // Serial.print(" Residual capacity: ");
        // Serial.println(residualCapacity);

        break;
      }
      case 0x94:
      {
        uint16_t batteryString = rxBuffers.packetData[0];
        uint16_t temperature = rxBuffers.packetData[1];
        uint16_t chargerStatus = rxBuffers.packetData[2];
        uint16_t loadStatus = rxBuffers.packetData[3];
        uint16_t chargeDischargeCycles = (rxBuffers.packetData[5] << 8) | rxBuffers.packetData[6];
        uint16_t DI_DO_states = rxBuffers.packetData[4];  //each bit in the first byte corresponds to a state

        //logging the collected info:
        // Serial.println(batteryString);
        // Serial.println((uint8_t)DI_DO_states);

        break;
      }
  }
}

void resetRxBuffers(){
  rxBuffers.bufferIndex = 0;
}
