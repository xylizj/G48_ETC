/*
 * TIM.c
 *
 *  Created on: Oct 19, 2016
 *      Author: Administrator
 */

#include "derivative.h" /* include peripheral declarations */
#include "TIM.h"
#include "clock.h"


volatile unsigned int overflow = 0;

volatile unsigned char capture_edge_idx = 0;
volatile unsigned int new_capture = 0;
volatile unsigned int old_capture = 0;
volatile unsigned int old_old_capture = 0;

volatile unsigned long int cycles_1;
volatile unsigned long int cycles_2;
volatile unsigned long int cycles_period;


volatile unsigned long int frequency;
volatile unsigned long int duty;//sacling 2^-16

void TIM_Init(void){  
  //1. Configure the prescaler (TSCR2[PR]).
  TSCR2_PR = TIM_PRESACLER;//0b011;//divide by 8
  
  //2. Configure needed channels as Input Capture (TIOS[IOSx]=0) or Output Compare (TIOS[IOSx]=1).
  TIOS_IOS2 = 0;      //channel 2 as an input capture
  TCTL4_EDG2x = 0b11; //capture on any edge
  //3. Enable interrupts if needed in the timer interrupt enable register (TIE).
  TIE_C2I = 1;//channel 0
  
  //4. Set the timer enable bit (TSCR1[TEN]).
  TSCR1 = 0xE0;       //enable timer, stop timer during wait and freeze, disable fast flag clear
  TSCR2_TOI = 1;       //timer overflow interrupt enable, timer prescaler is 0
}



#pragma CODE_SEG NON_BANKED
void interrupt __near
//VectorNumber_Vtimovf  
TOI_ISR(void)
{
    overflow++;
    
    TFLG2_TOF = 1;       //clear interrupt flag
}

//capture period
void interrupt __near
//VectorNumber_Vtimch2  
IC2_ISR(void)
{    
	new_capture = TC0;   //save the value of input capture register
    
	if(capture_edge_idx != 0)
	{
		if(capture_edge_idx == 1)  //capture_edge_idx just take care about the first calculating when we don't know the value of previous input capture
		{
			capture_edge_idx = 2;
			old_old_capture = old_capture;
			cycles_1 = (0xFFFF * (unsigned long int)overflow) + new_capture - old_capture;
		}
		else if(capture_edge_idx == 2)
		{
			capture_edge_idx = 1;
			cycles_2 = (0xFFFF * (unsigned long int)overflow) + new_capture - old_capture;
			cycles_period = (0xFFFF * (unsigned long int)overflow) + new_capture - old_old_capture;
			if((cycles_1+cycles_2) <= (cycles_period+4) && cycles_period <= (cycles_1+cycles_2+4))//Redundant check
			{
				frequency = BUS_CLOCK/cycles_period;
				if(cycles_1 >= cycles_2)
				{
					duty = cycles_1*65536/cycles_period;
				}
				else
				{
					duty = cycles_2*65536/cycles_period;
				}
			}
		}
	}
	else
	{
		capture_edge_idx = 1;
	}
    
  overflow = 0;
  old_capture = new_capture;   //save the IC for next using
  
  TFLG1_C0F = 1;       //clear interrupt flag   
}

//==============================================================================
// Input Capture Interrupt
//==============================================================================
void interrupt __near
//VectorNumber_Vtimch3  
IC3_ISR(void)
{    
	TC3 = TC3+ECT_PERIOD;     //the next event update
	TFLG1_C3F = 1;   //clear the flag
}
#pragma CODE_SEG DEFAULT


//==============================================================================
// InitOutputCompare
//==============================================================================
void InitOutputCompare(void)
{
	TIOS_IOS3 = 1;      //channel 0 as an output compare  
	TIE_C3I = 1;        //interrupt enable
	TSCR2_PR = 0b011;//divide by 8
  TC3 = ECT_PERIOD;
  //TIM0TCTL2 = 0x01;		//toggle with timer output pin
  TCTL2 = 0x00;		//no action
  OCPD = 0xFF;//1 Disables the timer channel port. 
    				//Output Compare action will not occur on the channel pin, but the output compare flag still become set. 
  TSCR1 = 0xE0;       //enable timer, stop timer during wait and freeze, disable fast flag clear
}



#pragma INLINE
unsigned char TIM_Cycle125us(void){//(125/(1/(24/(1<<TIM_PRESACLER))))
  unsigned int delta_cnt;
  static unsigned int old_cnt = 0;  
  static unsigned int new_cnt = 0;  
  
  new_cnt = TCNT;
  
  delta_cnt = 0;
  
  if(new_cnt >= old_cnt) {
    delta_cnt = new_cnt - old_cnt;
  }else{
    delta_cnt = 0xFFFF - old_cnt + new_cnt;
  }
                  
  if(delta_cnt >= (125*(24/(1<<TIM_PRESACLER))) ) {
    old_cnt = new_cnt;
    return 1; 
  }
  
  return 0;
}