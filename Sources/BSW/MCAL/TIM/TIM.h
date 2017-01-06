/*
 * TIM.h
 *
 *  Created on: Oct 19, 2016
 *      Author: Administrator
 */

#ifndef TIM_H_
#define TIM_H_

#include "derivative.h" /* include peripheral declarations */
#include "clock.h"


//#define ECT_PERIOD 391  //500us (prescaler=8, bus clock 6.25MHz)
#define ECT_US 500
#define ECT_PERIOD ((BUS_FRQ_MHZ/8)*ECT_US)  //500us (prescaler=8, bus clock 32MHz)

//24M busclk
#define TIM_PRESACLER 3//0~7

extern volatile unsigned int overflow;

#define read_tbl() (uint32_t)(overflow*0x10000 + TCNT)

extern void TIM_Init(void);

extern unsigned char TIM_Cycle125us(void);



#endif /* TIM_H_ */
