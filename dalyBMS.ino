#include<CAN.h>
#include "dalyBMS.h"
#include "processBmsData.h"

//timer to check if BMS is asleep:
unsigned long int stallTimer;

//initializing FSM 
STATE state;
uint8_t singlePacketDataAvailable;
uint8_t cellVoltPacketCount;
uint8_t monomerTempPacketCount;

//initializing buffers to hold receiver data
RxBuffers rxBuffers;

//initializing struct to hold collected BMS data
bmsData bmsStats;

//initializing buffer to write to the CAN packet
uint8_t buffer[8];

void setup(){

  //initializing stall timer:
  stallTimer = 0;

  //intializing Finite State Machine:
  state = s0;
  singlePacketDataAvailable = 0;
  cellVoltPacketCount = 0;
  monomerTempPacketCount = 0;

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

void processMonomerTempsFrame( uint8_t monomerNum ){
  for(int i = 0 ; i < 7; i++){
    bmsStats.monomerTemps[monomerNum + i] = (float)rxBuffers.packetData[i+1] - 40;
    bmsStats.monomerTempsIndex += 1;
  }
}

//ISR handler that is fired when data is received
void onReceive(int packetSize) {
  //implement logic to move frame data to buffer
  switch( (CAN.packetId()>>16) & 0xFF ){
    case 0x90:
    case 0x91:
    case 0x92:
    case 0x93:
    case 0x94:
    case 0x97:
    case 0x98:
      {
      singlePacketDataAvailable = 1;
      resetRxBuffers(); //sets buffer index to 0
      for(int i = 0; i < 8; i++){
      rxBuffers.packetData[rxBuffers.bufferIndex] = CAN.read();
      rxBuffers.bufferIndex += 1;
        }
      break;
      }
    case 0x95:
      {  
      cellVoltPacketCount += 1;
      for(int i = 0; i < 8; i++){
        uint8_t data = CAN.read();
        if( i != 0 && i != 7 ){
          rxBuffers.multiPacketData[rxBuffers.multiPacketBufferIndex] = data;
          rxBuffers.multiPacketBufferIndex += 1;
        }     
      }
      break;
      }
    case 0x96:
      {
        monomerTempPacketCount += 1;
        for(int i = 0; i < 8; i++){
          uint8_t data = CAN.read();
          if(i!=0){
            rxBuffers.multiPacketData[rxBuffers.multiPacketBufferIndex] = data;
            rxBuffers.multiPacketBufferIndex += 1;
          }
        }
        break;
      }
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

  if( !singlePacketDataAvailable ){
    if( millis() - stallTimer > 1000 ){
      Serial.println("BMS is asleep. Sending data request again...");
      state = s0;
      stallTimer = millis();
    }
  }

  switch(state){
    case s0:
    case s0_idle:
    {
      if(state == s0){ 
        requestData(0x90); 
        state = s0_idle;
        }
      if(singlePacketDataAvailable){
        processBmsData(0x90);
        state = s1;
        singlePacketDataAvailable = 0;
        stallTimer = millis();
      }
      break;
    }

    case s1:
    case s1_idle:
    {
      if(state == s1){
      requestData(0x91);
      state = s1_idle;
      }
      if(singlePacketDataAvailable){
        processBmsData(0x91);
        state = s2;
        singlePacketDataAvailable = 0;
        stallTimer = millis();
      }
      break;
    }

    case s2:
    case s2_idle:
    {
      if(state == s2){
        requestData(0x92);
        state = s2_idle;
      }
      if(singlePacketDataAvailable){
        processBmsData(0x92);
        state = s3;
        singlePacketDataAvailable = 0;
        stallTimer = millis();
      }
      break;
    }

    case s3:
    case s3_idle:
    {
      if(state == s3){
        requestData(0x93);
        state = s3_idle;
      }
      if(singlePacketDataAvailable){
        processBmsData(0x93);
        state = s4;
        singlePacketDataAvailable = 0;
        stallTimer = millis();
      }
      break;
    }

    case s4:
    case s4_idle:
    {
      if(state == s4){
        requestData(0x94);
        state = s4_idle;
      }
      if(singlePacketDataAvailable){
        processBmsData(0x94);
        state = s5;
        singlePacketDataAvailable = 0;
        stallTimer = millis();
      }
      break;
    }

    case s5:
    case s5_idle:
    {
      if(state == s5){
        resetMultiPacketBuffer();
        bmsStats.cellVoltagesIndex = 0;
        cellVoltPacketCount = 0;
        requestData(0x95);
        state = s5_idle;
      }
      if( cellVoltPacketCount > ceil(bmsStats.batteryString/3)){
        processBmsData(0x95);
        state = s6;
        cellVoltPacketCount = 0;
        stallTimer = millis();
      }
      break;
    }

    case s6:
    case s6_idle:
    {
      if(state == s6){
        resetMultiPacketBuffer();
        bmsStats.monomerTempsIndex = 0;
        monomerTempPacketCount = 0;
        requestData(0x96);
        state = s6_idle;
      }
      if( monomerTempPacketCount > ceil(bmsStats.temperatures/7) ){
        processBmsData(0x96);
        state = s7;
        monomerTempPacketCount = 0;
        stallTimer = millis();
      }
      break;
    }

    case s7:
    case s7_idle:
    {
      if(state == s7){
        requestData(0x97);
        state = s7_idle;
      }
      if(singlePacketDataAvailable){
        processBmsData(0x97);
        state = s8;
        singlePacketDataAvailable = 0;
        stallTimer = millis();
      }
      break;
    }

    case s8:
    case s8_idle:
    {
      if(state == s8){
        requestData(0x98);
        state = s8_idle;
      }
      if(singlePacketDataAvailable){
        processBmsData(0x98);
        state = s9;
        singlePacketDataAvailable = 0;
        stallTimer = millis();
      }
      break;
    }

    case s9:
    {
      printProcessedData();
      state = s0;
      stallTimer = millis();
      break;
    }
  }
  
}