#ifndef __CFG_H_
#define __CFG_H_



//#define  MSG_DIAG_RX_INDEX		0

/************************ CAN_A/PCAN TX DIAG MSG INDEX ************************
	DIAG_TX		0x18DAFA01		8	TT_ASYNC	0
*************************************************************************/
//#define  MSG_DIAG_TX_INDEX		0


/************************************************************************
ticks calculation:

read_tbl() return TBL, uint is 12.5ns with 80MHz bus clock;

1ms = 1000000ns / 12.5ns = 80000 TBL count

*************************************************************************/
#define TBL_COUNT_1MS_DIAG	(24000/(1<<TIM_PRESACLER))
//#define TBL_COUNT_1MS	80000UL

#define TT_ASYNC 	  0     
#define TT_CYCLIC 	1    
#define TT_BAM 		  2       
#define TT_CMDT 	  3      
#define TT_FORWARD 	4      


#define PG_TX_FREE     0      
#define PG_TX_REQ      1      
#define PG_TX_XMT      2      
#define PG_TX_TX       3     

#define PG_RX_FREE     0      
#define PG_RX_FULL     1      

















#endif /* _J1939CFG_H_ */





