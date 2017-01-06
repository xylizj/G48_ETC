#include "J1939_PGN.h"
#include "J1939_Cfg.h"
#include "J1939_Ext.h"
#include "diag.h"



volatile struct PGN_RX_tag RxMsgBuffA[CANA_RX_MSG_NUM];
volatile struct PGN_TX_tag TxMsgBuffA[CANA_TX_MSG_NUM];


volatile Boolean CAN_m_st_RxMsgEnableA[CANA_RX_MSG_NUM];
volatile Boolean CAN_m_st_TxMsgEnableA[CANA_TX_MSG_NUM];


//======================Rx enable config=============================
const Boolean CAN_C_st_MsgEnableXCP_RX		= TRUE;
const Boolean CAN_C_st_MsgEnableDIAG_RX		= FALSE;
//======================Rx enable config=============================



//======================Tx enable config=============================
const Boolean CAN_C_st_MsgEnableETC	 	= FALSE;    
const Boolean CAN_C_st_MsgEnableXCP_TX	= TRUE;
const Boolean CAN_C_st_MsgEnableDIAG_TX	= FALSE;
//======================Tx enable config=============================



void ConfigTxMsgA(	uint8_t 	index, 
					uint32_t 	id, 
					uint8_t 	dl, 
					uint32_t 	rate, 
					uint32_t* 	data, 
					uint8_t 	tt,  
					FuncType    encode_func,
					FuncType    tx_err_func
					)
{
	if( index >= CANA_TX_MSG_NUM )
	{
		return;
	}
	
	TxMsgBuffA[index].id 		= id;	
	TxMsgBuffA[index].dl 		= dl;	
	TxMsgBuffA[index].rate 		= rate*TBL_COUNT_1MS;	
	TxMsgBuffA[index].data 		= data;	
	TxMsgBuffA[index].enable 	= CAN_m_st_TxMsgEnableA[index];	
	TxMsgBuffA[index].state 	= PG_RX_FREE;	
	TxMsgBuffA[index].tt 		= tt;	
	TxMsgBuffA[index].timer 	= read_tbl();	
	TxMsgBuffA[index].encode_func 	= encode_func;
	TxMsgBuffA[index].tx_err_func 	= tx_err_func;
}

void ConfigRxMsgA(	uint8_t 		index, 
					uint32_t 	id, 
					uint8_t 	dl, 
					uint32_t 	rate, 
					uint32_t* 	data, 
					uint8_t 	tt,  
					FuncType    decode_func,
					FuncType    rx_timeout_func
					)
{
	if( index >= CANA_RX_MSG_NUM )
	{
		return;
	}
	
	RxMsgBuffA[index].id 		= id;	
	RxMsgBuffA[index].dl 		= dl;	
	RxMsgBuffA[index].rate 		= rate*TBL_COUNT_1MS;	
	RxMsgBuffA[index].data 		= data;	
	RxMsgBuffA[index].enable 	= CAN_m_st_RxMsgEnableA[index];	
	RxMsgBuffA[index].state 	= PG_RX_FREE;	
	RxMsgBuffA[index].tt 		= tt;	
	RxMsgBuffA[index].timer 	= read_tbl();	
	RxMsgBuffA[index].decode_func 	= decode_func;
	RxMsgBuffA[index].rx_timeout_func 	= rx_timeout_func;
}


/*********************************************************************
  function name:  CAN_InitMsg() 
  description:    initialize message
  parameters:     none
  return values:  none
  remark:         should be recalled before CAN_A_Init()
*********************************************************************/
void CAN_InitMsg(void)
{
	CAN_m_st_RxMsgEnableA[MSG_XCP_RX_INDEX] 	= CAN_C_st_MsgEnableXCP_RX;
	CAN_m_st_RxMsgEnableA[MSG_DIAG_RX_INDEX] 	= CAN_C_st_MsgEnableDIAG_RX;

  //1	XCP_RX		0x18FFFFFE	8	TT_ASYNC	0
  ConfigRxMsgA(MSG_XCP_RX_INDEX, 0x18FFFFFE, 8, 0, (uint32_t*)&gMSG_XCP_RX,\
				 TT_ASYNC, J1939MsgXCP_RX_Decode, NULL);
				 
//2	DIAG_RX		0x767	8	TT_ASYNC	0
  /*ConfigRxMsgA(MSG_DIAG_RX_INDEX, MSG_DIAG_RX_PHY_ID, 8, 0, (uint32_t*)&gMSG_DIAG_RX,\
				 TT_ASYNC, MsgDIAG_RX_Decode, NULL);*/ 
  ConfigRxMsgA(MSG_DIAG_RX_INDEX, MSG_DIAG_RX_PHY_ID, 8, 0, (uint32_t*)&gMSG_DIAG_RX,\
				 TT_ASYNC, NULL, NULL); 				 	

	CAN_m_st_TxMsgEnableA[ MSG_ETC_TX_INDEX] 	= CAN_C_st_MsgEnableETC;
	CAN_m_st_TxMsgEnableA[ MSG_XCP_TX_INDEX] 	= CAN_C_st_MsgEnableXCP_TX;
	CAN_m_st_TxMsgEnableA[ MSG_DIAG_TX_INDEX] = CAN_C_st_MsgEnableDIAG_TX;

//1 ETC Info	        0x0CFFFA46	8	TT_CYCLIC   10
	ConfigTxMsgA(MSG_ETC_TX_INDEX, 0x0CFFFA46, 8, 10, (uint32_t*)&gMSG_EtcInfo,\
				 TT_CYCLIC, J1939MsgEtcInfo_Encode, NULL);                     				 

//2	XCP_TX	0x18FFFFFF	8	TT_ASYNC	0    //only take a place
  ConfigTxMsgA(MSG_XCP_TX_INDEX, 0x18FFFFFF, 8, 0, (uint32_t*)&gMSG_XCP_TX,\
				 TT_ASYNC, NULL, NULL);	

//3	DIAG_TX			8	TT_ASYNC	0            //only take a place
  ConfigTxMsgA(MSG_DIAG_TX_INDEX, MSG_DIAG_TX_ID, 8, 0, (uint32_t*)&gMSG_DIAG_TX,\
				 TT_ASYNC, NULL, NULL); 
}