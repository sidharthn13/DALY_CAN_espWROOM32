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

//ISR handler that is fired when data is received
void onReceive(int packetSize) {

  //implement logic to move frame data to buffer

  switch( (CAN.packetId()>>16) & 0xFF ){
    case 0x90:
      Serial.println("packet with data ID : 0x90 received");
      break;
    case 0x91:
      Serial.println("packet with data ID : 0x91 received");
      break;
    case 0x93:
      Serial.println("packet with data ID : 0x93 received");
      break;
  }
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
  //sending data requests
  requestData(0x91);
  // processBmsData(0x91);

  requestData(0x93);
  // processBmsData(0x93);
}