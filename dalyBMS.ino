#include<CAN.h>
  
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
}

void onReceive(int packetSize) {
  Serial.print("ID : ");
  Serial.println(CAN.packetId(), HEX);
  // Serial.print("data in packet: ");
  // for(int i = 0; i < 8; i++){
  //   int data = CAN.read();
  //   Serial.print(data);
  //   Serial.print(", ");
  // }
  // Serial.println();
}

void requestData(uint8_t dataID){
  uint32_t ID = 0x18000140 | (dataID<<16);
  CAN.beginExtendedPacket(ID, 8);
  CAN.write(buffer, 8);
  CAN.endPacket();
}

void loop(){
  delay(100);
  //sending packet
  requestData(0x90);
}