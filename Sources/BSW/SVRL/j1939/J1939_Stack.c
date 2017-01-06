#include "typedefs.h"
#include "J1939_Cfg.h"
#include "J1939_PGN.h"
#include "J1939_Trans.h"
#include "J1939_Ext.h"
//#include "CAN_cfg.h"

uint32_t CAN_m_st_RxTimeOutA;
uint32_t CAN_m_st_RxTimeOutC;
uint32_t CAN_m_st_TxTimeOutA;
uint32_t CAN_m_st_TxTimeOutC;

extern void CAN_InitMsg(void);
void J1939init(void);
void J1939_State(void);
void J1939_Timer(void);
void J1939(void);


const uint8_t CAN_C_fac_RxTimeout = 4;

/*********************************************************************/
/*  function name:  J1939init() 
/*  description:    
/*  parameters:     none
/*  return values:  none
/*  remark:         
/*********************************************************************/
void J1939init(void)
{
	//CAN_InitMsg();
	
	//J1939TP_Init();

 	CAN_m_st_RxTimeOutA = 0;
 	CAN_m_st_RxTimeOutC = 0;
 	CAN_m_st_TxTimeOutA = 0;
 	CAN_m_st_TxTimeOutC = 0;
	
	CAN_Init(); //xyl comment for compiler 20160718
}


/*********************************************************************/
/*  function name:  J1939() 
/*  description:    start protocol stack
/*  parameters:     none
/*  return values:  none
/*  remark:         
/*********************************************************************/
void J1939(void) 
{
	CAN_A_Receive(); //xyl comment for compiler 20160718
	J1939_State();
	J1939_Timer();	
}

void J1939_State(void)
{
	uint8_t i;

	//J1939TP_State();
	
	for( i=0; i<CANA_J1939_TX_MSG_NUM; i++ )
	{
		if( (TxMsgBuffA[i].state == PG_TX_REQ) && (TxMsgBuffA[i].dl <= 8) )
		{
			if( !CAN_A_Transmit(TxMsgBuffA[i].id, (uint8_t*)TxMsgBuffA[i].data, TxMsgBuffA[i].dl) )
			{
				TxMsgBuffA[i].state = PG_TX_FREE;
				CAN_m_st_TxTimeOutA &= ~((uint32_t)0x01<<i);
				break;
			}
		}
	}			
}

void J1939_Timer(void)
{
	uint32_t ct;
	uint32_t tmp;
	uint8_t i;
	
	ct = read_tbl();
	for( i=0; i<CANA_J1939_TX_MSG_NUM; i++ )
	{
		if(TxMsgBuffA[i].enable == 1)
		{
			if( (TxMsgBuffA[i].tt == TT_CYCLIC)&&(TxMsgBuffA[i].state == PG_TX_FREE) )
			{
				if(ct >= TxMsgBuffA[i].timer)
				  tmp = ct - TxMsgBuffA[i].timer;
				else
				  tmp = 0xFFFFFFFF - TxMsgBuffA[i].timer + ct;
				//tmp = ct - TxMsgBuffA[i].timer;//20161114 xyl modify
				if( tmp >= TxMsgBuffA[i].rate )
				{
					TxMsgBuffA[i].state = PG_TX_REQ;
					TxMsgBuffA[i].timer = ct;
					if(TxMsgBuffA[i].encode_func != NULL)
					{
						TxMsgBuffA[i].encode_func();
					}
					break;
				}
			}
			else if( (TxMsgBuffA[i].tt == TT_CMDT)&&(TP.State == BAM_TX_STATE_WAIT) )
			{
				tmp = ct - TxMsgBuffA[i].timer;
				if( tmp > TxMsgBuffA[i].rate ) 
				{
					if( TxMsgBuffA[i].state == PG_TX_FREE) 
					{
						TP.State = BAM_TX_STATE_DATA;
						TxMsgBuffA[i].timer = ct;
					}
				}
			}
		}
	}
	
	/* check Rx time out */
	for( i=0; i<CANA_J1939_RX_MSG_NUM; i++ )
	{
		if(RxMsgBuffA[i].enable == 1)
		{
			if(RxMsgBuffA[i].tt == TT_CYCLIC)
			{
				if(ct >= TxMsgBuffA[i].timer)
				  tmp = ct - TxMsgBuffA[i].timer;
				else
				  tmp = 0xFFFFFFFF - TxMsgBuffA[i].timer + ct;
				//tmp = ct - TxMsgBuffA[i].timer;//20161114 xyl modify
				if( tmp > (CAN_C_fac_RxTimeout*RxMsgBuffA[i].rate) )
				{
					CAN_m_st_RxTimeOutA |= (uint32_t)((uint32_t)0x00000001<<i);
					if(RxMsgBuffA[i].rx_timeout_func != NULL)
					{
						RxMsgBuffA[i].rx_timeout_func();
					}
				}
			}
		}
	}
	
	/* check Tx time out, which means Tx error or bus busy */
	for( i=0; i<CANA_J1939_TX_MSG_NUM; i++ )
	{
		if(TxMsgBuffA[i].state == PG_TX_REQ)
		{
			if(ct >= TxMsgBuffA[i].timer)
			  tmp = ct - TxMsgBuffA[i].timer;
			else
			  tmp = 0xFFFFFFFF - TxMsgBuffA[i].timer + ct;
			//tmp = ct - TxMsgBuffA[i].timer;//20161114 xyl modify
			if( tmp > TxMsgBuffA[i].rate )
			{
				CAN_m_st_TxTimeOutA |= ((uint32_t)0x00000001<<i);

				if(TxMsgBuffA[i].tx_err_func != NULL)
				{
					TxMsgBuffA[i].tx_err_func();
				}
			}
		}
	}
}

