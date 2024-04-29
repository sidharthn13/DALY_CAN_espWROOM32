#include<CAN.h>
#include "dalyBMS.h"
#include "processBmsData.h"

//initializing requestTimer
long unsigned int requestTimer;

//initializing FSM 
STATE state;
uint8_t processFlag;

//initializing buffers to hold receiver data
RxBuffers rxBuffers;

//initializing struct to hold collected BMS data
bmsData bmsStats;

//initializing buffer to write to the CAN packet
uint8_t buffer[8];

void setup(){

  //intializing Finite State Machine:
  state = s0;
  processFlag = 0; //processing of data frame should not happen when flag value set to 0

  //intializing request timer to keep track of how long it has been since req for data was sent:
  requestTimer = 0;

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
      resetRxBuffers(); //sets buffer index to 0
      for(int i = 0; i < 8; i++){
      rxBuffers.packetData[rxBuffers.bufferIndex] = CAN.read();
      rxBuffers.bufferIndex += 1;
        }
      break;
      }
    case 0x95:
      {  
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
  
  if ( millis() - requestTimer > 20 ){ processFlag = 1; }

  switch(state){
    case s0:
    case s0_idle:
    {
      if(state == s0){ 
        requestData(0x90); 
        state = s0_idle;
        }
      if(processFlag == 1){
        processBmsData(0x90);
        processFlag = 0;
        state = s1;
        requestTimer = millis();
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
      if(processFlag == 1){
        processBmsData(0x91);
        processFlag = 0;
        state = s2;
        requestTimer = millis();
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
      if(processFlag == 1){
        processBmsData(0x92);
        processFlag = 0;
        state = s3;
        requestTimer = millis();
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
      if(processFlag == 1){
        processBmsData(0x93);
        processFlag = 0;
        state = s4;
        requestTimer = millis();
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
      if(processFlag == 1){
        processBmsData(0x94);
        processFlag = 0;
        state = s5;
        requestTimer = millis();
      }
      break;
    }

    case s5:
    case s5_idle:
    {
      if(state == s5){
        resetMultiPacketBuffer();
        bmsStats.cellVoltagesIndex = 0;
        requestData(0x95);
        state = s5_idle;
      }
      if( millis() - requestTimer > 100 ){
        processBmsData(0x95);
        state = s6;
        requestTimer = millis();
      }
      break;
    }

    case s6:
    case s6_idle:
    {
      if(state == s6){
        resetMultiPacketBuffer();
        bmsStats.monomerTempsIndex = 0;
        requestData(0x96);
        state = s6_idle;
      }
      if( millis() - requestTimer > 100 ){
        processBmsData(0x96);
        state = s9;
        requestTimer = millis();
      }
      break;
    }

    case s9:
    {
      printProcessedData();
      state = s0;
      requestTimer = millis();
      break;
    }
  }

  
  //////req data for SOC total voltage and current and then process it////////
  // requestData(0x90);
  // delay(50); //To wait for relevant data packet to be written to the buffer.(prevents reading old data)
  // processBmsData(0x90);
  ////////////////////////////////////////////////////////////////////////////

  ////req data for Maximum, Minimum Voltage of Monomer and then process it////
  // requestData(0x91);
  // delay(50);
  // processBmsData(0x91);
  ////////////////////////////////////////////////////////////////////////////

  //req data for Maximum, Minimum temperature of Monomer and then process it//
  // requestData(0x92);
  // delay(50);
  // processBmsData(0x92);
  ////////////////////////////////////////////////////////////////////////////

  /////////req data charge/discharge MOS status and then process it//////////
  // requestData(0x93);
  // delay(50);
  // processBmsData(0x93);
  ///////////////////////////////////////////////////////////////////////////

  //////////req data for status information 1 and then process it////////////
  // requestData(0x94);
  // delay(50);
  // processBmsData(0x94);
  ///////////////////////////////////////////////////////////////////////////

  ////////req data for individual cell voltages and then process it//////////
  // bmsStats.cellVoltagesIndex = 0;
  // requestData(0x95);
  // delay(50); //to give enough time for all packets to be received before processing the data
  // processBmsData(0x95);
  ///////////////////////////////////////////////////////////////////////////

  /////req data for individual monomer temperature and then process it///////
  // bmsStats.monomerTempsIndex = 0;
  // requestData(0x96);
  // delay(50);
  // processBmsData(0x96);
  ///////////////////////////////////////////////////////////////////////////

  ///////req data for monomer equilibrium states and then process it/////////
  // requestData(0x97);
  // delay(50);
  // processBmsData(0x97);
  ///////////////////////////////////////////////////////////////////////////

  /////////req data for battery failure status and then process it///////////
  // requestData(0x98);
  // delay(50);
  // processBmsData(0x98);
  ///////////////////////////////////////////////////////////////////////////

  // printProcessedData();

}