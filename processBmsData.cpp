#include "processBmsData.h"

void processBmsData(uint8_t dataID){
  resetRxBuffers(); //sets buffer index to 0
  for(int i = 0; i < 8; i++){
  rxBuffers.packetData[rxBuffers.bufferIndex] = CAN.read();
  rxBuffers.bufferIndex += 1;
  }
  switch(dataID){
    case 0x90:
      {
        float pressure = (float)( (rxBuffers.packetData[0] << 8 ) | rxBuffers.packetData[1] )/ 10 ;
        float acquisition = (float)( (rxBuffers.packetData[2] << 8 ) | rxBuffers.packetData[3] )/ 10 ;
        float totalCurrent = ( (float)( (rxBuffers.packetData[4] << 8 ) | rxBuffers.packetData[5] ) - 30000) / 10 ;
        float SOC = (float)( (rxBuffers.packetData[6] << 8) | rxBuffers.packetData[7] ) / 10 ;

        //writing data to structure holding BMS stats:
        bmsStats.pressureVoltage = pressure;
        bmsStats.acquisitionVoltage = acquisition;
        bmsStats.totalCurrent = totalCurrent;
        bmsStats.SOC = SOC;

        //logging the collected info:
        // Serial.print("Pressure: ");
        // Serial.print(pressure);
        // Serial.print("V");
        // Serial.print(", Acquisition: ");
        // Serial.print(acquisition);
        // Serial.print("V");
        // Serial.print(", Total Current: ");
        // Serial.print(totalCurrent);
        // Serial.print("A");
        // Serial.print(", SOC: ");
        // Serial.print(SOC);
        // Serial.print("%");
      break;
      }
    case 0x91:
      {
      float maxVoltage = (float)((rxBuffers.packetData[0] << 8 ) | rxBuffers.packetData[1]);
      uint8_t maxVoltageCell = rxBuffers.packetData[2];
      float minVoltage = (float)((rxBuffers.packetData[3] << 8 ) | rxBuffers.packetData[4]);
      uint8_t minVoltageCell = rxBuffers.packetData[5];

      //writing data to structure holding BMS stats:
      bmsStats.maxCellVoltage = maxVoltage;
      bmsStats.maxCellVoltNum = maxVoltageCell;
      bmsStats.minCellVoltage = minVoltage;
      bmsStats.minCellVoltNum = minVoltageCell;

      // logging the collected info:
      // Serial.print(", Max voltage: ");
      // Serial.print(maxVoltage);
      // Serial.print("mV");
      // Serial.print(", Max V cell no: ");
      // Serial.print(maxVoltageCell);
      // Serial.print(", Min voltage: ");
      // Serial.print(minVoltage);
      // Serial.print("mV");
      // Serial.print(", Min V cell no: ");
      // Serial.print(minVoltageCell);

      break;
      }
    case 0x92:
      {
        float maxTemperature = (float)rxBuffers.packetData[0] - 40; 
        uint8_t maxTempMonomer = rxBuffers.packetData[1];
        float minTemperature = (float)rxBuffers.packetData[2] - 40;
        uint8_t minTempMonomer = rxBuffers.packetData[3];

        //writing data to structure holding BMS stats:
        bmsStats.maxMonomerTemp = maxTemperature;
        bmsStats.minMonomerTemp = minTemperature;
        bmsStats.maxMonomerTempNum = maxTempMonomer;
        bmsStats.minMonomerTempNum = minTempMonomer;

        //logging the collected info:
        // Serial.print(", Maximum monomer temperature: ");
        // Serial.print(maxTemperature);
        // Serial.print(" deg C");
        // Serial.print(", Max temp monomer no: ");
        // Serial.print(maxTempMonomer); 
        // Serial.print(", Minimumum monomer temperature: ");
        // Serial.print(minTemperature);
        // Serial.print(" deg C");
        // Serial.print(", Min temp monomer no: ");
        // Serial.print(minTempMonomer); 
        
        break;
      }
    case 0x93:
      {
        uint8_t chargeDischargeStatus = rxBuffers.packetData[0];
        uint8_t chargingMosTubeState = rxBuffers.packetData[1];
        uint8_t dischargeMosTubeState = rxBuffers.packetData[2];
        uint8_t bmsLife = rxBuffers.packetData[3];
        uint32_t residualCapacity = (rxBuffers.packetData[4] << 24) | (rxBuffers.packetData[5] << 16) |
        (rxBuffers.packetData[6] << 8) | rxBuffers.packetData[7];

        //writing data to structure holding BMS stats:
        bmsStats.chargeDischargeStatus = chargeDischargeStatus;
        bmsStats.chargingMosTubeState = chargingMosTubeState;
        bmsStats.dischargeMosTubeState = dischargeMosTubeState;
        bmsStats.bmsLife = bmsLife;
        bmsStats.residualCapacity = residualCapacity;

        //logging the collected info:
        // Serial.print(", Charge-Discharge status: ");
        // Serial.print(chargeDischargeStatus);
        // Serial.print(", Charging MOS tube status: ");
        // Serial.print(chargingMosTubeState);
        // Serial.print(", Discharging MOS tube status");
        // Serial.print(dischargeMosTubeState);
        // Serial.print(", BMS life cycles: ");
        // Serial.print(bmsLife);
        // Serial.print(", Residual capacity: ");
        // Serial.print(residualCapacity);
        // Serial.print("mAH");

        break;
      }
      case 0x94:
      {
        uint8_t batteryString = rxBuffers.packetData[0];
        uint8_t temperature = rxBuffers.packetData[1];
        uint8_t chargerStatus = rxBuffers.packetData[2];
        uint8_t loadStatus = rxBuffers.packetData[3];
        uint16_t chargeDischargeCycles = (rxBuffers.packetData[5] << 8) | rxBuffers.packetData[6];
        uint8_t DI_DO_states = rxBuffers.packetData[4];  //each bit in the first byte corresponds to a state

        bmsStats.batteryString = (uint8_t)batteryString;

        // logging the collected info:
        // Serial.print(", Battery String: ");
        // Serial.print(batteryString);
        // Serial.println((uint8_t)DI_DO_states);

        break;
      }
      case 0x95:
      {
        Serial.print(", Cell voltages: ");
        Serial.print("[ ");
        for(int i = 0; i < 16; i++){
          Serial.print(bmsStats.cellVoltages[i]);
          Serial.print("mV, ");
        }
        Serial.print("]");
        break;
      }
      case 0x96:
      {
        Serial.print(", Monomer Temperatures: ");
        Serial.print("[ ");
        for(int i = 0; i < 16; i++){
          Serial.print(bmsStats.monomerTemps[i]);
          Serial.print(" deg C, ");
        }
        Serial.println("]");
        break;
      }
      case 0x97:
      {
        //each byte of data consists of equilibrium states for 8 monomers(each bit mapped to equilibrium state of one monomer)
        uint8_t state_0_to_7 = rxBuffers.packetData[0];
        uint8_t state_8_to_15 = rxBuffers.packetData[1];
        uint8_t state_16_to_23 = rxBuffers.packetData[2];
        uint8_t state_24_to_31 = rxBuffers.packetData[3];
        uint8_t state_32_to_39 = rxBuffers.packetData[4];
        uint8_t state_40_to_47 = rxBuffers.packetData[5];

        break;
      }
      case 0x98:
      {
        //each byte contains status flags for one parameter(each bit in a byte is treated as a flag where 0 ->no error and 1->error)
        uint8_t batteryFailureStatus0 = rxBuffers.packetData[0];
        uint8_t batteryFailureStatus1 = rxBuffers.packetData[1];
        uint8_t batteryFailureStatus2 = rxBuffers.packetData[2];
        uint8_t batteryFailureStatus3 = rxBuffers.packetData[3];
        uint8_t batteryFailureStatus4 = rxBuffers.packetData[4];
        uint8_t batteryFailureStatus5 = rxBuffers.packetData[5];
        uint8_t batteryFailureStatus6 = rxBuffers.packetData[6];
        uint8_t faultCode = rxBuffers.packetData[7];

        break;
      }

  }
}

void resetRxBuffers(){
  rxBuffers.bufferIndex = 0;
}

void printProcessedData(){
  //printing 0x90 data:
  Serial.print("Pressure: ");
  Serial.print(bmsStats.pressureVoltage);
  Serial.print("V");
  Serial.print(", Acquisition: ");
  Serial.print(bmsStats.acquisitionVoltage);
  Serial.print("V");
  Serial.print(", Total Current: ");
  Serial.print(bmsStats.totalCurrent);
  Serial.print("A");
  Serial.print(", SOC: ");
  Serial.print(bmsStats.SOC);
  Serial.print("%");

  //printing 0x91 data:
  Serial.print(", Max voltage: ");
  Serial.print(bmsStats.maxCellVoltage);
  Serial.print("mV");
  Serial.print(", Max V cell no: ");
  Serial.print(bmsStats.maxCellVoltNum);
  Serial.print(", Min voltage: ");
  Serial.print(bmsStats.minCellVoltage);
  Serial.print("mV");
  Serial.print(", Min V cell no: ");
  Serial.print(bmsStats.minCellVoltNum);

  //printing 0x92 data:
  Serial.print(", Maximum monomer temperature: ");
  Serial.print(bmsStats.maxMonomerTemp);
  Serial.print(" deg C");
  Serial.print(", Max temp monomer no: ");
  Serial.print(bmsStats.maxMonomerTempNum); 
  Serial.print(", Minimumum monomer temperature: ");
  Serial.print(bmsStats.minMonomerTemp);
  Serial.print(" deg C");
  Serial.print(", Min temp monomer no: ");
  Serial.print(bmsStats.minMonomerTempNum); 

  //printing 0x93 data:
  Serial.println();
}
