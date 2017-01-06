#ifndef _J1939_EXT_H_
#define _J1939_EXT_H_

//#include "CAN_cfg.h"
#include "Platform_Types.h"
#include "AUTOSAR_Types.h"
#include "J1939_Cfg.h"
#include "TIM.h"
#include "can.h"
#include "ETC_PID.h"

extern uint32_t CAN_m_st_RxTimeOutA;
extern uint32_t CAN_m_st_RxTimeOutC;
extern uint32_t CAN_m_st_TxTimeOutA;
extern uint32_t CAN_m_st_TxTimeOutC;

extern volatile struct PGN_RX_tag RxMsgBuffA[CANA_RX_MSG_NUM];
extern volatile struct PGN_TX_tag TxMsgBuffA[CANA_TX_MSG_NUM];
extern volatile Boolean CAN_m_st_RxMsgEnableA[CANA_RX_MSG_NUM];
extern volatile Boolean CAN_m_st_TxMsgEnableA[CANA_TX_MSG_NUM];

#ifdef USE_CAN_C
extern volatile struct PGN_RX_tag RxMsgBuffC[CANC_RX_MSG_NUM];
extern volatile struct PGN_TX_tag TxMsgBuffC[CANC_TX_MSG_NUM];
#endif

extern volatile  struct XCP_RX_tag 		gMSG_XCP_RX;


extern volatile struct EtcInfo_tag gMSG_EtcInfo;
extern volatile  struct TPCM_tag 		gMSG_TPCM;
extern volatile  struct TPDT_tag 		gMSG_TPDT;
extern volatile  struct XCP_TX_tag 		gMSG_XCP_TX;



 
extern unsigned char ETC_m_st_HBridge;
extern unsigned char ETC_m_st_PosSensorPlau;


extern void J1939MsgXCP_RX_Decode(void);



extern void J1939MsgEtcInfo_Encode(void);


extern void Xcp_CanIfRxIndication(uint8_t* buff, uint8_t length);
extern uint8_t Xcp_CanIfTransmit(uint8_t* buff, uint8_t length);
/* this function should be called after one XCP message sent out successfully */
extern void __near Xcp_CanIfTxConfirmation( void );


extern void J1939init(void);
extern void J1939_State(void);
extern void J1939_Timer(void);
extern void J1939(void);



#define Dem_SetError(err_index, set_clear)

#endif