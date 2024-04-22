#include<CAN.h>
#include "dalyBMS.h"
#include "processBmsData.h"

//initializing buffers to hold receiver data
RxBuffers rxBuffers;

//initializing buffer to write to the CAN packet
uint8_t buffer[8];

void setup(){
  Serial.begin(115200);
  if(!CAN.begin(250E3)){
    Serial.println("Failed to intialize CAN");
  }
  else{
    Serial.println("Initialized CAN");
  }

  //setting calback function to be called when packet is received
  CAN.onReceive(onReceive);

  //populating the Tx buffer
  buffer[0] = 0x00;
  buffer[1] = 0x00;
  buffer[2] = 0x00;
  buffer[3] = 0x00;
  buffer[4] = 0x00;
  buffer[5] = 0x00;
  buffer[6] = 0x00;
  buffer[7] = 0x00;
}

//used for debugging
void logBufferData(){
  for(int i = 0; i < 8; i++){
    Serial.print(rxBuffers.packetData[i], HEX);
    Serial.print(", ");
  }
  Serial.println();
}

//ISR handler that is fired when data is received
void onReceive(int packetSize) {
  //implement logic to move frame data to buffer
  resetRxBuffers(); //sets buffer index to 0
  for(int i = 0; i < 8; i++){
  rxBuffers.packetData[rxBuffers.bufferIndex] = CAN.read();
  rxBuffers.bufferIndex += 1;
    }
  resetRxBuffers(); 
  // logBufferData();    
}
//pass data ID of the desired battery data 
void requestData(uint8_t dataID){
  uint32_t ID = 0x18000140 | (dataID<<16);
  CAN.beginExtendedPacket(ID, 8);
  CAN.write(buffer, 8);
  CAN.endPacket();
}

void loop(){
  delay(100);

  //////req data for SOC total voltage and current and then process it////////
  // requestData(0x90);
  // processBmsData(0x90);
  ////////////////////////////////////////////////////////////////////////////

  ////req data for Maximum, Minimum Voltage of Monomer and then process it////
  // requestData(0x91);
  // processBmsData(0x91);
  ////////////////////////////////////////////////////////////////////////////

  //req data for Maximum, Minimum temperature of Monomer and then process it//
  // requestData(0x92);
  // processBmsData(0x92);
  ////////////////////////////////////////////////////////////////////////////

  /////////req data charge/discharge MOS status and then process it//////////
  // requestData(0x93);
  // processBmsData(0x93);
  ///////////////////////////////////////////////////////////////////////////

  //////////req data for status information 1 and then process it////////////
  // requestData(0x94);
  // processBmsData(0x94);
  ///////////////////////////////////////////////////////////////////////////

  //////////req data for individual voltages and then process it/////////////
  // requestData(0x95);
  // processBmsData(0x95);
  ///////////////////////////////////////////////////////////////////////////

  /////////req data for battery failure status and then process it///////////
  // requestData(0x98);
  // processBmsData(0x98);
  ///////////////////////////////////////////////////////////////////////////

}