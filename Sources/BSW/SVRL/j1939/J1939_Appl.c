#include "J1939_PGN.h"
#include "J1939_Ext.h"
#include "J1939_Cfg.h"
//#include "CanXcp_CanIf.h"
//#include "InterfaceVar.h"
#include "typedefs.h"
#include <string.h>



volatile struct XCP_RX_tag 		gMSG_XCP_RX;


volatile struct EtcInfo_tag gMSG_EtcInfo;
volatile struct XCP_TX_tag 		gMSG_XCP_TX;








void J1939MsgXCP_RX_Decode(void)
{
  Xcp_CanIfRxIndication((uint8_t *)(&(RxMsgBuffA[MSG_XCP_RX_INDEX].data[0])), RxMsgBuffA[MSG_XCP_RX_INDEX].dl);
}


uint8_t Xcp_CanIfTransmit(uint8_t* buff, uint8_t length)
{
  return CAN_A_Transmit(0x18FFFFFF, buff, length);
}






/*
  A =(256-((ETC_m_pc_ActualPos+5*256)/90))*100
    =25600- (ETC_m_pc_ActualPos/9*10) - (256*50/9)
  open ratio:[100,0],scaling 2^-8
  
  B =A/256*400, scaling 0.0025
  B =((256-((ETC_m_pc_ActualPos+5*256)/90))*100) * (400/256)
    =100*400 - ((ETC_m_pc_ActualPos+5*256)/90)*100*400/256
    =100*400 - (ETC_m_pc_ActualPos+5*256)/9*25/8*5

*/
void J1939MsgEtcInfo_Encode(void) 
{ 
	uint16_t temp = 100*400 - (ETC_m_pc_ActualPos+5*256)/9*25/8*5;
	
	gMSG_EtcInfo.Byte1.ActualPos_lo = (temp & 0xFF);
	gMSG_EtcInfo.Byte2.ActualPos_hi = ((temp >> 8) & 0xFF);
	//gMSG_EtcInfo.Byte3.HBridge = ETC_m_st_HBridge;
	//gMSG_EtcInfo.Byte4.B.PosSensorPlau = ETC_m_st_PosSensorPlau;
	
	//gMSG_EtcInfo.Byte5.SetPos_lo = (ETC_m_pc_ETCVlvSetPc & 0xFF);
	//gMSG_EtcInfo.Byte6.SetPos_hi = ((ETC_m_pc_ETCVlvSetPc >> 8) & 0xFF);
	//gMSG_EtcInfo.Byte7.GovDvt_lo = (ETC_m_pc_GovDvt & 0xFF);
	//gMSG_EtcInfo.Byte8.GovDvt_hi = ((ETC_m_pc_GovDvt >> 8) & 0xFF);	
}  