#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "clock.h"
#include "TIM.h"
#include "adc.h"
#include "can.h"
#include "SPI.h"
#include "MLX90363.h"
#include "TLE6209R.h"
#include "ETC_PID.h"
#include "J1939_Ext.h"
#include "SchM.h"



void main(void) {
  
  DisableInterrupts;
  SchM_ASW_Init();
  SchM_BSW_Init();
	EnableInterrupts;


  for(;;) {
    static unsigned int cnt = 0;    
    
    SchM_MainFunction();

    if(TIM_Cycle125us()) {
      SchM_Period125us();
      cnt ++;
      if(cnt >= 40000)
        cnt = 0;      
    }
    
    if((cnt % 8) == 0){
       SchM_Period1ms();
    }
    
    if((cnt % 40) == 0){
       SchM_Period5ms();
    }

    if((cnt % 80) == 0){
       SchM_Period10ms();
    }

    if((cnt % 800) == 0){
       SchM_Period100ms();
    }

    if((cnt % 1600) == 0){
       SchM_Period200ms();
    }
    
    if((cnt % 20000) == 0){
       SchM_Period2500ms();
    }
    
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}






























/*void Task_CanTx(void)
{ 
  static unsigned long int cntCanTxRun;
  
  if((RTI_Cnt-cntCanTxRun) >= 80) 
  {
    cntCanTxRun = RTI_Cnt;
    CAN_PeriodHandle(CAN_HANDLE_TX);
  }
}




void Task_Tle6209(void)
{ 
  static unsigned long int cntTle6209;
  
  if((RTI_Cnt-cntTle6209) >= 50)//5ms 
  {
    cntTle6209 = RTI_Cnt;
    TLE6209_CtrlDiag();
  }
}*/
