#include<CAN.h>

//initializing buffers to hold receiver data
typedef struct RxBuffers{
  uint8_t SocVoltage[8];
  uint8_t monomerVoltage[8];
  uint8_t MosStatus[8];
}RxBuffers;
RxBuffers rxBuffers;

//initializing buffer to write to the CAN packet
uint8_t buffer[8];

//initializing array to hold sample data
uint8_t sampleData[4][8];

void setup(){
  Serial.begin(115200);

  if(!CAN.begin(250E3)){
    Serial.println("Failed to intialize CAN");
  }
  else{
    Serial.println("Initialized CAN");
  }

  //settign the CAN ports in loopback mode
  CAN.loopback();

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

  //populating sample data array
  uint8_t socVoltage[8] = {0x01, 0x57, 0x00, 0x00, 0x75, 0x4E, 0x03, 0xE8};
  memcpy(sampleData[0], socVoltage, 8);

  uint8_t monomerVoltage[8] = {0x0C, 0xEE, 0x04, 0x00, 0x00, 0x02, 0x03, 0xE8};
  memcpy(sampleData[1], monomerVoltage, 8);

  uint8_t MosStatus[8] = {0x42, 0x01, 0x42, 0x01, 0x00, 0x02, 0x03, 0xE8};
  memcpy(sampleData[2], MosStatus, 8);
}

//ISR handler that is fired when data is received
void onReceive(int packetSize) {
  switch( (CAN.packetId()>>16) & 0xFF ){
    case 0x90:
      memcpy(rxBuffers.SocVoltage, sampleData[0], 8);
      break;
    case 0x91:
      memcpy(rxBuffers.monomerVoltage, sampleData[0], 8);
      break;
    case 0x92:
      memcpy(rxBuffers.MosStatus, sampleData[0], 8);
      break;
  }
}
//pass data ID of the desired battery data for requesting as well as processing
void requestData(uint8_t dataID){
  uint32_t ID = 0x18000140 | (dataID<<16);
  CAN.beginExtendedPacket(ID, 8);
  CAN.write(buffer, 8);
  CAN.endPacket();
}
void processBmsData(uint8_t dataID){
  switch(dataID){
    case 0x90:
      break;
    case 0x91:
      int maxVoltage = rxBuffers.monomerVoltage[7] | (rxBuffers.monomerVoltage[6] << 8);
      int maxVoltageCell = rxBuffers.monomerVoltage[5];
      int minVoltage = rxBuffers.monomerVoltage[4] | (rxBuffers.monomerVoltage[3] << 8);
      int minVoltageCell = rxBuffers.monomerVoltage[2];
      Serial.print("Max voltage : ");
      Serial.print(maxVoltage);
      Serial.print("Cell number : ");
      Serial.print(maxVoltageCell);
      Serial.print(", Min voltage : ");
      Serial.print(minVoltage);
      Serial.print("Cell number : ");
      Serial.println(minVoltageCell);
  }
}

void loop(){
  delay(100);
  //sending data requests
  requestData(0x91);
  processBmsData(0x91);
}