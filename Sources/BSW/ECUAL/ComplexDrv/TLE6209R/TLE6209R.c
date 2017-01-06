#include <hidef.h>      /* common defines and macros */
#include "derivative.h"
#include "PWM.h"
#include "SPI.h"
#include "TLE6209R.h"
#include "clock.h"
#include "ETC_PID.h"

control_word_t  tle6209_control_word;
status_word_t   tle6209_status_word;
extern unsigned int alpha_zero;


void TLE6209_Init(void){
  tle6209_control_word.Bits.reset = 0;
  tle6209_control_word.Bits.OVLO = 0;//Over Voltage Lockout
  tle6209_control_word.Bits.offtime = OFF_time_24us;
  tle6209_control_word.Bits.decaymode = 0;//Slow decay
  tle6209_control_word.Bits.currlimit = Currentlimit_4A;
  
  SPI1_Init();

  PWM0_Init();
  PWM0_SetPeriod(255);
  MOTOR_CLOCKWISE();
  ETC_m_pc_ETCVlvSetPc = 90*256;
  //ETC_m_pc_ETCVlvSetPc = (t_Percent2)(100*1024 - (256*10*5/9) - (25*((alpha_zero)/4)));
  
  TLE6209_Inhibit_ExitSleepMode();
  TLE6209_EnaOutput();
}


unsigned char ETC_m_st_HBridge;

void TLE6209_CtrlDiag(void){
  unsigned char dutySet;      
  
  if(ETC_m_pc_DutyOut>=0){
    dutySet = (unsigned char)(ETC_m_pc_DutyOut/100);
    MOTOR_CLOCKWISE();
  }else{
    dutySet = (unsigned char)((-ETC_m_pc_DutyOut)/100);
    MOTOR_COUNTER_CLOCKWISE();
  }
  
  PWM0_SetDuty(dutySet);
  DisableInterrupts;
  ETC_m_st_HBridge = tle6209_status_word.uByte = SPI1_SendRecv_byte(tle6209_control_word.uByte);//
  EnableInterrupts;  
  /*
  if(0 != tle6209_status_word.Bits.errflag) {
     TLE6209_DisOutput();
  }
  */
}