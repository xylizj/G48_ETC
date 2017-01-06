#ifndef __TLE6209R_H_
#define __TLE6209R_H_

#include "derivative.h"

#define TLE6209_Inhibit_EnterSleepMode()  {PTP_PTP3 = 0; DDRP_DDRP3 = 1;}
#define TLE6209_Inhibit_ExitSleepMode()   {PTP_PTP3 = 1; DDRP_DDRP3 = 1;}
#define TLE6209_DisOutput()               {PTP_PTP1 = 1; DDRP_DDRP1 = 1;}
#define TLE6209_EnaOutput()               {PTP_PTP1 = 0; DDRP_DDRP1 = 1;}
#define TLE6209_Out1()                    {PTP_PTP2 = 1; DDRP_DDRP2 = 1;}//Motor turns counterclockwise
#define TLE6209_Out2()                    {PTP_PTP2 = 0; DDRP_DDRP2 = 1;}//Motor turns clockwise
#define MOTOR_CLOCKWISE()                 TLE6209_Out1()
#define MOTOR_COUNTER_CLOCKWISE()         TLE6209_Out2()

typedef union {
  byte uByte;
  struct {
    byte currlimit   :2;  //2 bit chopper current limit
    byte decaymode   :1;  //PWM Operation mode: H = Fast decay, L = Slow decay
    byte offtime     :2;  //2bit chopper-OFF-time
    byte notused     :1;                                    
    byte OVLO        :1;  //OVLO: H = on, L = off
    byte reset       :1;  //Status Register Reset: H = reset                                    
  } Bits;
} control_word_t;


typedef union {
  byte uByte;
  struct {
    byte errflag     :1;//bit0
    byte tempmon     :2;  //Temperature Monitoring
    byte ol          :1;  //Open load
    byte stg         :1;  //Short to GND
    byte stp_stl     :1;  //Short to VS or across the load                                  
    byte notused     :1;  //not used, always H
    byte pwrsupply   :1;//bit7  //Power supply fail                                   
  } Bits;
} status_word_t;//H = Error/L = no error


//Switch-OFF Time during Current Limitation
enum _ChopperOFFTime{
   OFF_time_24us = 0b00,
   OFF_time_43us = 0b01,
   OFF_time_62us = 0b10,
   OFF_time_80us = 0b11,
};

enum CurrentLimits{
   Currentlimit_4A = 0b00,
   Currentlimit_5A = 0b01,
   Currentlimit_6A = 0b10,
   Currentlimit_7A = 0b11,
};

enum TemperatureMonitoring{
   TempBelowPrewarning = 0b00,
   TempPrewarning = 0b01,
   TempWarning = 0b10,
   TempShutdown = 0b11,
};


//extern unsigned char ETC_m_st_HBridge;

extern void TLE6209_Init(void);
extern void TLE6209_CtrlDiag(void);




#endif/*__TLE6209R_H_*/