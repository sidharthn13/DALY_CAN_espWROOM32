
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

  //setting up the filter to receive messages:
  // CAN.filterExtended(0x18000140, 0x1F00FFFF);

  //setting calback function to be called when packet is received
  CAN.onReceive(onReceive);

  //populating the Tx buffer
  buffer[0] = 0x00;
  buffer[1] = 0x01;
  buffer[2] = 0x02;
  buffer[3] = 0x03;
  buffer[4] = 0x04;
  buffer[5] = 0x05;
  buffer[6] = 0x06;
  buffer[7] = 0x07;



}

void onReceive(int packetSize) {
  
  //receiving sent packet
  // Serial.print("Num bytes received in packet: ");
  // Serial.println(packetSize);
  // uint32_t id = CAN.packetId();
  // Serial.print("packet ID: ");
  // Serial.println(id);

  // Serial.println(CAN.available());

  Serial.print("data in packet: ");
  for(int i = 0; i < 8; i++){
    int data = CAN.read();
    Serial.print(data);
    Serial.print(", ");
  }
  Serial.println();
}

void loop(){
  //sending packet
  delay(100);
  CAN.beginExtendedPacket(0x18900140,8);
  int numBytesWritten = CAN.write(buffer, 8);
  Serial.print("Num bytes written to CAN frame: ");
  Serial.println(numBytesWritten);
  CAN.endPacket();
  
}