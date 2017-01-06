#ifndef __MLX90363_H_
#define __MLX90363_H_

#define OPCODE_GET1   0x13
#define OPCODE_NOP    0x10
#define OPCODE_ECHO    0x10
#define OPCODE_READY  44
#define OPCODE_OSCCNTSTRT  24
#define OPCODE_OSCCNTSTRTACK  25
#define OPCODE_OSCCNTSTOP  26
#define OPCODE_OSCCOUNTER  27
#define OPCODE_DIAGDETAIL  22
#define OPCODE_DIAGANS  23
#define OPCODE_NTT  62
#define OPCODE_MEMREAD  1
#define OPCODE_MEMREADANS  2
#define OPCODE_EEPWRITE  3
#define OPCODE_EEReadChallenge  15
#define OPCODE_EEChallenge  4
#define OPCODE_EEChallengeAns  5
#define OPCODE_EEReadAnswer 40
#define OPCODE_EEPROMWriteStatus 14
#define OPCODE_REBOOT 47

#define ERR_OK                      0
#define ERR_INCORRECT_CRC           1
#define ERR_OPC_INVALID             2
#define ERR_NOT_REGULAR_MSG         3
#define ERR_POSSIG_UNPLAU           4


enum __Marker_{
  Marker0 = 0,// refers to frame type ¡°ALPHA + Diagnostic¡±. 
  Marker1= (1<<6),// refers to frame type ¡°ALPHA + BETA + Diagnostic¡±. 
  Marker2 = (2<<6),// refers to frame type ¡°Components X + Y +Z +Diagnostic¡±.
  Marker3 = (3<<6),//not a regular message
};


/*
If FSMERC= 3, ANADIAGCNTtakes another meaning: 
193 protection error interruption happened 
194 invalid address error interruption happened 
195 program error interruption happened 
196 exchange error interruption happened 
197 not connected error interruption happened 
198 Stack Interrupt 
199 Flow Control Error

Parameter FSMERC reports the root-cause of entry in fail-safe mode 
o  FSMERC= 0 : the chip is not in fail safe mode 
o  FSMERC= 1 : BIST error happened and the chip is in fail safe mode 
o  FSMERC= 2 : digital diagnostic error happened and the chip is in fail safe mode 
o  FSMERC= 3 : one of the 5 error interruptions listed above happened and the chip is in fail safe mode 
*/
enum __FSMERC_{
  FSMERC0 = 0,  
  FSMERC1 = (1<<6),
  FSMERC2 = (2<<6),
  FSMERC3 = (3<<6),  
};


#define ALPHA_OFFSET (5*4096*4/360)

#define FIRST 0
#define SECOND 10 
#define THIRD 20
#define FOURTH 22
#define TOTAL 40




extern unsigned int alpha_zero;
extern unsigned int alpha;

extern unsigned char MLX90363_Reboot(void);//followed by start-up
extern unsigned char MLX90363_StartupSafety(void);
extern unsigned char MLX90363_TrigMode1_FirstTime(void);
extern unsigned char MLX90363_TrigMode1_Loop(void);

extern void MLX90363_SendGET1(void);
extern unsigned char MLX90363_RecvGET1(void);
extern void MLX90363_SendDiag(void);
extern unsigned char MLX90363_RecvDiag(void);

extern void MLX90363_InitMisc(void);
extern unsigned char Task_PosSelflearn(void);
extern void Task_PosSample_Loop(void);

#endif/*__MLX90363_H_*/