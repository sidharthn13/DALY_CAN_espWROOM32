
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

  //populating the buffer
  buffer[0] = 0x00;
  buffer[1] = 0x01;
  buffer[2] = 0x02;
  buffer[3] = 0x03;
  buffer[4] = 0x04;
  buffer[5] = 0x05;
  buffer[6] = 0x06;
  buffer[7] = 0x07;
}

void loop(){
  CAN.beginExtendedPacket(0x18900140,8);
  int numBytesWritten = CAN.write(buffer, 8);
  Serial.print("Num bytes written to CAN frame: ");
  Serial.println(numBytesWritten);
  CAN.endPacket();

}