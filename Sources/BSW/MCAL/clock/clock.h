#ifndef __CLOCK_H_
#define __CLOCK_H_

#define OSC_FRQ_MHZ 8 /*[MHz]*/
#define BUS_FRQ_MHZ 24 /*[MHz]*/
#define BUS_CLOCK (BUS_FRQ_MHZ*1000000)//Hz
#define NOP_DELAY_1US() {\
                          asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;\
                          asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;\
                          asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;\
                          asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;\
                          asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;\
                          asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;asm nop;\
}

//#define _NO_PLL_FROM_OSCCLK_
#define _PLL_FROM_OSCCLK_
//#define _PLL_FROM_IRCCLK_

#define _PLL_USE_INTERRUPT_


extern volatile unsigned long int RTI_Cnt;


void PLL_PEE_Init(void);
void RTI_Init(void);
void delay_us(unsigned int n);

#endif/*__CLOCK_H_*/ 