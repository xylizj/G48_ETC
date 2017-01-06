#include <hidef.h>      /* common defines and macros */
#include "MLX90363.h"
#include "clock.h"
#include "SPI.h"
#include "ETC_PID.h"


static unsigned char const cba_256_TAB[256] = {
  0x00,  0x2f,  0x5e,  0x71,  0xbc,  0x93,  0xe2,  0xcd,
  0x57,  0x78,  0x09,  0x26,  0xeb,  0xc4,  0xb5,  0x9a,
  0xae,  0x81,  0xf0,  0xdf,  0x12,  0x3d,  0x4c,  0x63,
  0xf9,  0xd6,  0xa7,  0x88,  0x45,  0x6a,  0x1b,  0x34,
  0x73,  0x5c,  0x2d,  0x02,  0xcf,  0xe0,  0x91,  0xbe,
  0x24,  0x0b,  0x7a,  0x55,  0x98,  0xb7,  0xc6,  0xe9,
  0xdd,  0xf2,  0x83,  0xac,  0x61,  0x4e,  0x3f,  0x10,
  0x8a,  0xa5,  0xd4,  0xfb,  0x36,  0x19,  0x68,  0x47,
  0xe6,  0xc9,  0xb8,  0x97,  0x5a,  0x75,  0x04,  0x2b,
  0xb1,  0x9e,  0xef,  0xc0,  0x0d,  0x22,  0x53,  0x7c,
  0x48,  0x67,  0x16,  0x39,  0xf4,  0xdb,  0xaa,  0x85,
  0x1f,  0x30,  0x41,  0x6e,  0xa3,  0x8c,  0xfd,  0xd2,
  0x95,  0xba,  0xcb,  0xe4,  0x29,  0x06,  0x77,  0x58,
  0xc2,  0xed,  0x9c,  0xb3,  0x7e,  0x51,  0x20,  0x0f,
  0x3b,  0x14,  0x65,  0x4a,  0x87,  0xa8,  0xd9,  0xf6,
  0x6c,  0x43,  0x32,  0x1d,  0xd0,  0xff,  0x8e,  0xa1,
  0xe3,  0xcc,  0xbd,  0x92,  0x5f,  0x70,  0x01,  0x2e,
  0xb4,  0x9b,  0xea,  0xc5,  0x08,  0x27,  0x56,  0x79,
  0x4d,  0x62,  0x13,  0x3c,  0xf1,  0xde,  0xaf,  0x80,
  0x1a,  0x35,  0x44,  0x6b,  0xa6,  0x89,  0xf8,  0xd7,
  0x90,  0xbf,  0xce,  0xe1,  0x2c,  0x03,  0x72,  0x5d,
  0xc7,  0xe8,  0x99,  0xb6,  0x7b,  0x54,  0x25,  0x0a,
  0x3e,  0x11,  0x60,  0x4f,  0x82,  0xad,  0xdc,  0xf3,
  0x69,  0x46,  0x37,  0x18,  0xd5,  0xfa,  0x8b,  0xa4,
  0x05,  0x2a,  0x5b,  0x74,  0xb9,  0x96,  0xe7,  0xc8,
  0x52,  0x7d,  0x0c,  0x23,  0xee,  0xc1,  0xb0,  0x9f,
  0xab,  0x84,  0xf5,  0xda,  0x17,  0x38,  0x49,  0x66,
  0xfc,  0xd3,  0xa2,  0x8d,  0x40,  0x6f,  0x1e,  0x31,
  0x76,  0x59,  0x28,  0x07,  0xca,  0xe5,  0x94,  0xbb,
  0x21,  0x0e,  0x7f,  0x50,  0x9d,  0xb2,  0xc3,  0xec,
  0xd8,  0xf7,  0x86,  0xa9,  0x64,  0x4b,  0x3a,  0x15,
  0x8f,  0xa0,  0xd1,  0xfe,  0x33,  0x1c,  0x6d,  0x42,  
};





unsigned char g_msgTx_GET1[8];
unsigned int alpha_zero;

unsigned int alpha;
unsigned int beta;
unsigned int x,y,z;
  unsigned char rxMarker;   
//unsigned int phy_angle;
unsigned char msgTx_DiagDetail[8];
unsigned char ETC_m_st_PosSensorPlau;
unsigned long int MLX90363_DiagnosticDetails;

unsigned char  rc_MLX90363_TrigMode1;
unsigned char  rc_MLX90363_Diag;




static unsigned char MLX90363_ComputeCrc(unsigned char msg[]){
  unsigned char crc;
  
  crc = 0xFF;  
  crc = cba_256_TAB[msg[0] ^ crc];
  crc = cba_256_TAB[msg[1] ^ crc];
  crc = cba_256_TAB[msg[2] ^ crc];
  crc = cba_256_TAB[msg[3] ^ crc];
  crc = cba_256_TAB[msg[4] ^ crc];
  crc = cba_256_TAB[msg[5] ^ crc];
  crc = cba_256_TAB[msg[6] ^ crc];
  crc = ~ crc;
  
  return crc;
}



unsigned char MLX90363_Send1Msg(unsigned char msg[]) {
  unsigned char i;
    
  for(i=0; i<8; i++){    
    if(0 != SPI0_Send_byte(msg[i])) {
      return 1;
    }
  }
  
  return 0;
}


unsigned char MLX90363_Recv1Msg(unsigned char msg[]){
  unsigned char i;
  
  for(i=0; i<8; i++){
    msg[i] = SPI0_Receive_byte();
  }
  
  if(msg[7] != MLX90363_ComputeCrc(msg)){
    return 2;  
  }
  
  return 0;
}

static 
void MLX90363_PackCmdOnlyOpc(unsigned char msgTx_Cmd[], unsigned char opcode){
  unsigned char i;
  
  for(i=0; i<6; i++){
    msgTx_Cmd[i] = 0;
  }
  msgTx_Cmd[6] = Marker3 | opcode;
  msgTx_Cmd[7] = MLX90363_ComputeCrc(msgTx_Cmd);
}

static 
void MLX90363_PackGet1Cmd(void){
  g_msgTx_GET1[0] = 0;
  g_msgTx_GET1[1] = 0;
  g_msgTx_GET1[2] = 0xFF;//value;
  g_msgTx_GET1[3] = 0xFF;//timeout us
  g_msgTx_GET1[4] = 0;
  g_msgTx_GET1[5] = 0;
  g_msgTx_GET1[6] = Marker0 | OPCODE_GET1;
  g_msgTx_GET1[7] = MLX90363_ComputeCrc(g_msgTx_GET1);
}


static 
void MLX90363_PackNopCmd(unsigned char msgTx_Nop[], unsigned char key_byte2, unsigned char key_byte3){
  msgTx_Nop[0] = 0;
  msgTx_Nop[1] = 0;
  msgTx_Nop[2] = key_byte2;
  msgTx_Nop[3] = key_byte3;
  msgTx_Nop[4] = 0;
  msgTx_Nop[5] = 0;
  msgTx_Nop[6] = Marker3 | OPCODE_NOP;
  msgTx_Nop[7] = MLX90363_ComputeCrc(msgTx_Nop);
}

static //Nop cmd answer
unsigned char MLX90363_ChallengeEcho(unsigned char msg[], unsigned char key_byte2, unsigned char key_byte3){
  unsigned char i;
  unsigned char msgRx_Echo[8];
  
  for(i=0; i<8; i++){
     msgRx_Echo[i] = SPI0_SendRecv_byte(msg[i]);
     if(0xFF == msgRx_Echo[i])
       return 2;
  }
   
  if(msgRx_Echo[7] != MLX90363_ComputeCrc(msgRx_Echo)){
    return 3;  
  }

  if(   (key_byte2 != msgRx_Echo[2])   || (key_byte3 != msgRx_Echo[3])
     || (key_byte2 != ~ msgRx_Echo[4]) || (key_byte3 != ~ msgRx_Echo[5]))
  {
    return 4; 
  }
  
  return 0;  
}


void MLX90363_InitMisc(void){
  MLX90363_PackGet1Cmd();
  MLX90363_PackCmdOnlyOpc(msgTx_DiagDetail, OPCODE_DIAGDETAIL);
}
  


unsigned char failsafe;

unsigned char MLX90363_Reboot(void){//followed by start-up
  unsigned char msgTx_Reboot[8];
  
  MLX90363_PackCmdOnlyOpc(msgTx_Reboot, OPCODE_REBOOT);    
  if(0 != MLX90363_Send1Msg(msgTx_Reboot)){ // 
      return 1;    
  }    
 
  return 0;
}






unsigned char MLX90363_TrigMode1_FirstTime(void){
  unsigned char i;
  unsigned char msgRx_ReguPacket[8];
    
  for(i=0; i<8; i++){
    SPI0_Send_byte(g_msgTx_GET1[i]);
  }

  delay_us(1050);
  
  DisableInterrupts;
  (void)SPI0_Receive_byte();
  for(i=0; i<8; i++){
     msgRx_ReguPacket[i] = SPI0_Receive_byte();
  }
  EnableInterrupts;
  if(msgRx_ReguPacket[7] != MLX90363_ComputeCrc(msgRx_ReguPacket)) {
    return ERR_INCORRECT_CRC;
  }

  if(Marker0 == (msgRx_ReguPacket[6] & Marker3)){   
    alpha_zero = ((msgRx_ReguPacket[1] & 0x3F) << 8) + (msgRx_ReguPacket[0]);
  }
  
  return ERR_OK;
}


//#pragma INLINE
static 
void MLX90363_SendGET1(void){
  unsigned char i;
  
  DisableInterrupts;
  for(i=0; i<8; i++){
    SPI0_Send_byte(g_msgTx_GET1[i]);
  }
  EnableInterrupts;  
}




//#pragma INLINE
static 
unsigned char MLX90363_RecvGET1(void){
  unsigned char i;
  unsigned char msgRx_ReguPacket[8];
  
  DisableInterrupts;
  (void)SPI0_Receive_byte();
  for(i=0; i<8; i++){
    msgRx_ReguPacket[i] = SPI0_Receive_byte();
  }
  EnableInterrupts;

  if(msgRx_ReguPacket[7] != MLX90363_ComputeCrc(msgRx_ReguPacket)) {
    return ERR_INCORRECT_CRC;
  }
    
  rxMarker = msgRx_ReguPacket[6] & Marker3;//The bits byte6[7] and byte6[6] are markers.
  if(Marker0 == rxMarker){   
    alpha = ((msgRx_ReguPacket[1] & 0x3F) << 8) + (msgRx_ReguPacket[0]);
  }else if(Marker1 == rxMarker){
    alpha = ((msgRx_ReguPacket[1] & 0x3F) << 8) + (msgRx_ReguPacket[0]);
    beta = ((msgRx_ReguPacket[3] & 0x3F) << 8) + (msgRx_ReguPacket[2]);
  } else if(Marker2 == rxMarker){
    x = ((msgRx_ReguPacket[1] & 0x3F) << 8) + (msgRx_ReguPacket[0]);
    y = ((msgRx_ReguPacket[3] & 0x3F) << 8) + (msgRx_ReguPacket[2]);    
    z = ((msgRx_ReguPacket[5] & 0x3F) << 8) + (msgRx_ReguPacket[4]);    
  }else{
    return ERR_NOT_REGULAR_MSG;//not a regular message
  }
  
  return ERR_OK;
}


//#pragma INLINE
static
void MLX90363_SendDiag(void){  
  unsigned char i;
    
  DisableInterrupts;
  for(i=0; i<8; i++){
     SPI0_Send_byte(msgTx_DiagDetail[i]);
  }    
  EnableInterrupts;
}



//#pragma INLINE
static
unsigned char MLX90363_RecvDiag(void){
  unsigned char i;
  unsigned char msgRx_DiagDetail[8];
  unsigned char FSMERC;
  
  DisableInterrupts;
  (void)SPI0_Receive_byte();
  for(i=0; i<8; i++){
     msgRx_DiagDetail[i] = SPI0_Receive_byte();
  }
  EnableInterrupts;
  if(msgRx_DiagDetail[7] != MLX90363_ComputeCrc(msgRx_DiagDetail)) {
    return ERR_INCORRECT_CRC;
  }

  FSMERC = msgRx_DiagDetail[3] & 0xC0;
  /*if(FSMERC0 == FSMERC){
    ETC_m_st_PosSensorPlau = 0;
  }else if(FSMERC1 == FSMERC){
    ETC_m_st_PosSensorPlau = 1;
  }else if(FSMERC2 == FSMERC){
    ETC_m_st_PosSensorPlau = 1;
  }else if(FSMERC3 == FSMERC){
    ETC_m_st_PosSensorPlau = 1;
  }*/
  if(FSMERC0 != FSMERC)
    return ERR_POSSIG_UNPLAU;
  
  return ERR_OK;
}


//after first time GET1 issued, delay at least tREFE_mod1(920 1050 920) us
/*unsigned char MLX90363_TrigMode1_Loop(void){
  unsigned char i;
  unsigned char msgRx_ResultPre[8];
   
  MLX90363_PackGet1Cmd();

  DisableInterrupts;
  for(i=0; i<8; i++){
     SPI0_Send_byte(g_msgTx_GET1[i]);
  }
  EnableInterrupts;
  
  delay_us(920);
  
  DisableInterrupts;
  (void)SPI0_Receive_byte();
  for(i=0; i<8; i++){
     msgRx_ResultPre[i] = SPI0_Receive_byte();
  }
  EnableInterrupts;
  if(msgRx_ResultPre[7] != MLX90363_ComputeCrc(msgRx_ResultPre)) {
    return ERR_INCORRECT_CRC;
  }
    
  rxMarker = msgRx_ResultPre[6] & Marker3;//The bits byte6[7] and byte6[6] are markers.
  if(Marker0 == rxMarker){   
    alpha = (msgRx_ResultPre[0] << 8) + (msgRx_ResultPre[1] & 0x3F);
  }else if(Marker1 == rxMarker){
    alpha = (msgRx_ResultPre[0] << 8) + (msgRx_ResultPre[1] & 0x3F);
    beta = (msgRx_ResultPre[2] << 8) + (msgRx_ResultPre[3] & 0x3F);    
  } else if(Marker2 == rxMarker){
    x = (msgRx_ResultPre[0] << 8) + (msgRx_ResultPre[1] & 0x3F);
    y = (msgRx_ResultPre[2] << 8) + (msgRx_ResultPre[3] & 0x3F);    
    z = (msgRx_ResultPre[4] << 8) + (msgRx_ResultPre[5] & 0x3F);    
  }else{
    return ERR_NOT_REGULAR_MSG;//not a regular message
  }


  return ERR_OK;
}


unsigned char MLX90363_TrigMode1_BreakLoop(void){
  unsigned char msgTx_Nop[8];
  unsigned char key_byte2, key_byte3;
  
  MLX90363_PackNopCmd(msgTx_Nop, key_byte2, key_byte3);
  
  if(0 != MLX90363_Send1Msg(msgTx_Nop)){
      return 1;    
  }

  return MLX90363_ChallengeEcho(msgTx_Nop, key_byte2, key_byte3);
}*/



#pragma INLINE
unsigned char Task_PosSelflearn(void)
{ 
  static unsigned long int cntSelflearn = 0;
  unsigned char onceCompleted;
  
  if(0 == (cntSelflearn % 10)) {
    MLX90363_SendGET1();
  } else if(8 == (cntSelflearn % 10)){
    if(ERR_OK == MLX90363_RecvGET1()) {
      alpha_zero = alpha;
    }
  } 
  cntSelflearn ++;      
  if(cntSelflearn >= 10) {
    cntSelflearn = 0;
    return 1;
  }
  
  return 0;
}


#pragma INLINE
void Task_PosSample_Loop(void)
{ 
  static unsigned long int cntTrigMode1 = 0;

  if(FIRST == (cntTrigMode1 % TOTAL)) {
    MLX90363_SendGET1();
  } else if(SECOND == (cntTrigMode1 % TOTAL)){
    if(ERR_OK == MLX90363_RecvGET1())
      if(alpha >= alpha_zero) {
        ETC_m_pc_ActualPos = (t_Percent2)(5*(alpha-alpha_zero) + 5*(alpha-alpha_zero)/8 - 5*256);//[-5,85)
      }else{
        
      }
  } else if(THIRD == (cntTrigMode1 % TOTAL)){
    MLX90363_SendDiag();    
  }else if(FOURTH == (cntTrigMode1 % TOTAL)){
    if(ERR_OK == MLX90363_RecvDiag())
      ETC_m_st_PosSensorPlau = 1;
    else
      ETC_m_st_PosSensorPlau = 0;
  }
  
  cntTrigMode1 ++;      
  if(cntTrigMode1 >= 40000) {
    cntTrigMode1 = 0; 
  }
}

