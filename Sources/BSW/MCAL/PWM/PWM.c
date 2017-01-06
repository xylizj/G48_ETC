/*
 * PWM.c
 *
 *  Created on: Oct 18, 2016
 *      Author: Administrator
 */
#include "derivative.h" /* include peripheral declarations */
#include "clock.h"

void PWM01_Init(void){
	//1. Select 8 bit (CONxx=0) or 16 bit (CONxx=1) operation.
	PWMCTL_CON01 = 1;	  
	//2. Select the clock source (PWMCLK register).
	PWMCLK = 0;

	//3. Configure prescaler (PWMPRCLK register).
	PWMPRCLK = 0;//divide by 1 = 24M

	//4. Configure duty (PWMDTYx) and period (PWMPERx).
	PWMPER01 = 24000;//24M/24000=1K
	PWMDTY01 = 0;

	//5. Select the PWM polarity (PWMPOL).
	PWMPOL_PPOL1 = 1;

	//6. Select left aligned (CAEx=0) or center aligned (CAEx=1) output.
	PWMCAE_CAE1 = 1;

	//7. Enable the needed channels (PWMEx=1).
	PWME_PWME1 = 1;
}


void Pwm01DrvPeriod(unsigned int PerValue){//hz
     PWMPER01 = (unsigned int)(PerValue);         
}

void Pwm01DrvDuty(unsigned int DutyValue){
     PWMDTY01 = DutyValue;         
}

void PWM0_Init(void){
	//1. Select 8 bit (CONxx=0) or 16 bit (CONxx=1) operation.
	PWMCTL_CON01 = 0;	  
	//2. Select the clock source (PWMCLK register).
	PWMCLK = 0;
	//3. Configure prescaler (PWMPRCLK register).
	PWMPRCLK_PCKA = 0b110;//divide by 128 = 1.5/8M

	//4. Configure duty (PWMDTYx) and period (PWMPERx).
	PWMPER0 = 250;
	PWMDTY0 = 0;

	//5. Select the PWM polarity (PWMPOL).
	PWMPOL_PPOL0 = 1;

	//6. Select left aligned (CAEx=0) or center aligned (CAEx=1) output.
	PWMCAE_CAE0 = 0;

	//7. Enable the needed channels (PWMEx=1).
	PWME_PWME0 = 1;
}

void PWM0_SetPeriod(unsigned char PerValue){
     PWMPER0 = PerValue;         
}



#pragma CONST_SEG CAL_RAM
extern const volatile unsigned char pwm_duty_threshold;
#pragma CONST_SEG DEFAULT

void PWM0_SetDuty(unsigned char DutyValue){
     unsigned char PerValue;

     PerValue = PWMPER0;
     
     if(((DutyValue*100)/PerValue) >= pwm_duty_threshold) {
       DutyValue = (PerValue * pwm_duty_threshold) / 100;
     }
     
     PWMDTY0 = DutyValue;         
}