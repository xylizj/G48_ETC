#ifndef _J1939_CFG_H_
#define _J1939_CFG_H_


#include "typedefs.h"
#include "J1939_PGN.h"



/************************ CAN_A J1939 RX MSG INDEX ***********************/
#define CANA_J1939_RX_MSG_NUM	20

/************************ CAN_A RX other(DIAG XCP etc..) MSG INDEX ************************
DIAG_RX		0x18DA00FA	8	TT_ASYNC	0
*******************************************************************************/
#define MSG_XCP_RX_INDEX		  (CANA_J1939_RX_MSG_NUM+0)
#define MSG_DIAG_RX_INDEX		  (CANA_J1939_RX_MSG_NUM+1)

#define CANA_RX_MSG_NUM			  (CANA_J1939_RX_MSG_NUM+2)



/************************ CAN_A TX MSG INDEX ***********************/
#define  MSG_ETC_TX_INDEX		  0

#define CANA_J1939_TX_MSG_NUM	1

/************************ CAN_A TX other(DIAG XCP etc..) MSG INDEX ************************
	DIAG_TX		0x18DAFA31		8	TT_ASYNC	0
*************************************************************************/
#define  MSG_XCP_TX_INDEX		  (CANA_J1939_TX_MSG_NUM+0)
#define  MSG_DIAG_TX_INDEX		(CANA_J1939_TX_MSG_NUM+1)

#define CANA_TX_MSG_NUM			  (CANA_J1939_TX_MSG_NUM+2)



/************************************************************************
ticks calculation:

read_tbl() return TBL, uint is 12.5ns with 80MHz bus clock;

1ms = 1000000ns / 12.5ns = 80000 TBL count

*************************************************************************/
//#define TBL_COUNT_1MS	80000UL

/************************************************************************
ticks calculation:

read_tbl(), 24MHz timer clock;

1ms

*************************************************************************/
#define TBL_COUNT_1MS	(24000/(1<<TIM_PRESACLER))

#define TT_ASYNC 	0     
#define TT_CYCLIC 	1    
#define TT_BAM 		2       
#define TT_CMDT 	3      
#define TT_FORWARD 	4      


#define PG_TX_FREE     0      
#define PG_TX_REQ      1      
#define PG_TX_XMT      2      
#define PG_TX_TX       3     

#define PG_RX_FREE     0      
#define PG_RX_FULL     1      



#endif /* _J1939CFG_H_ */  