#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "clock.h"
#include "adc.h"
#include "can.h"
#include "SPI.h"
#include "MLX90363.h"
#include "TLE6209R.h"
#include "ETC_PID.h"
#include "TIM.h"
#include "J1939_Cfg.h"
#include "J1939_Ext.h"
#include "diag.h"
#include "EEIf.h"
#include "xcp_data.h"
#include "SchM.h"

static unsigned char positionSelfLearningFlag = 0;


void SchM_BSW_Init(void){
  PLL_PEE_Init();

	TLE6209_Init();
  PWM0_SetDuty(50);
  MOTOR_COUNTER_CLOCKWISE();

  //ADC_Init();  
  //(void)FlashInit(PSSDConfig);
  CAN_InitMsg();//removed in lib, add here
  J1939init();
  //Diag_InitTP();
  Xcp_Init();
  
  SPI0_Init();
  MLX90363_InitMisc();
  (void)MLX90363_Reboot();	
	
	TIM_Init();
}


void SchM_ASW_Init(void){
  	ETC_PID_initialize();  
}


#pragma INLINE
void SchM_MainFunction(void){
    J1939();//CAN_Rx();//CAN_PeriodHandle(CAN_HANDLE_RX); //Task background--real time        
    //Diag_State();
    Xcp_Background();
}



    

 
#pragma INLINE
void SchM_Period125us(void){
  if(positionSelfLearningFlag){
    Task_PosSample_Loop();  
  }else{    
    static uint16_t drive2zeroDelayCnt = 0;
    drive2zeroDelayCnt ++;
    if(drive2zeroDelayCnt > (8*300))//300ms
    {
      if(Task_PosSelflearn()) {
        PWM0_SetDuty(0);
        positionSelfLearningFlag = 1;
      }
    }
  }
}


#pragma INLINE
void SchM_Period1ms(void){
}

int16_T X,Y,Z;

#pragma INLINE
void SchM_Period5ms(void){
  if(positionSelfLearningFlag){    
    ETC_PID_step();
    X = ETC_MAPY_trq_ETCVlvPcSet[0];
    Y = ETC_MAPX_n_ETCVlvPcSetEngSpd[1]; 
    Z = ETC_MAP_pc_ETCVlvPcSet[2];
    if(Z >(X+Y))
      Z = 0;
    
    TLE6209_CtrlDiag();
  } else{
    
  }
}


static uint16_t batVoltValue;

#pragma INLINE
void SchM_Period10ms(void){
  Xcp_Service(0);
  //(void)ADC_ReadChannel(AN_CHAN_0, &batVoltValue);//CAN_PeriodHandle(CAN_HANDLE_TX);
}


#pragma INLINE
void SchM_Period100ms(void){
   Xcp_Service(1);
}


#pragma INLINE
void SchM_Period200ms(void){
   Xcp_Service(2);
}


unsigned char read_en;
unsigned char write_en;
unsigned int test_;
unsigned long int xcp_chksum_save;
unsigned long int xcp_chksum_read;
unsigned long int xcp_chksum_cal;
unsigned char xcp_save_rc;

#define AUTOTEST_POS_MAX (85*256)
#define AUTOTEST_POS_MIN (-5*256)
#define AUTOTEST_POS_STEP (10*256)

unsigned char autotest_en;
#pragma INLINE
void SchM_Period2500ms(void){
   /*static unsigned char b_clockwise = 0;
	 
   if(positionSelfLearningFlag){  
     if(!autotest_en) {
      TLE6209_DisOutput();
      return;
     } else{
      TLE6209_EnaOutput();
     }
        
     if(ETC_m_pc_ETCVlvSetPc >= AUTOTEST_POS_MAX){
       b_clockwise = 1;
     }else if(ETC_m_pc_ETCVlvSetPc <= AUTOTEST_POS_MIN){
       b_clockwise = 0;
     }   
     
     if(1 == b_clockwise)
      ETC_m_pc_ETCVlvSetPc -= AUTOTEST_POS_STEP;
     else
      ETC_m_pc_ETCVlvSetPc += AUTOTEST_POS_STEP;
     
     if(ETC_m_pc_ETCVlvSetPc >= AUTOTEST_POS_MAX)
      ETC_m_pc_ETCVlvSetPc = AUTOTEST_POS_MAX;
     else if(ETC_m_pc_ETCVlvSetPc <= AUTOTEST_POS_MIN)
      ETC_m_pc_ETCVlvSetPc = AUTOTEST_POS_MIN;
   }*/
}