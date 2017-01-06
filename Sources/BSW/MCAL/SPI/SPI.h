#ifndef __SPI__
#define __SPI__

#include "derivative.h"



/* definitions */
#define ENABLE    0
#define DISABLE   1

/* Global Variables */

/* Prototypes */
extern void SPI0_Init(void);
extern unsigned char SPI0_Send_byte(unsigned char u8Data);
extern unsigned char SPI0_Receive_byte(void);
extern void SPI0_High_rate(void);


extern void SPI1_Init(void);
extern unsigned char SPI1_Send_byte(unsigned char u8Data);
extern unsigned char SPI1_Receive_byte(void);
extern unsigned char SPI1_SendRecv_byte(unsigned char u8Data);


#endif /* __SPI__ */