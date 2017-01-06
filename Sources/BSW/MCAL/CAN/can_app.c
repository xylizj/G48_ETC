#include "can_app.h"
#include "ETC_PID.h"



uint8_t SysStatus_Send_buf[8];
uint8_t EtcInfo_Send_buf[8];

void CAN_Send_SysStatus(void){
   unsigned int phy_angle;
   
   phy_angle = (unsigned int)(((alpha /45)));
   //phy_angle = (unsigned int)(((alpha * 360) / 4096) /4);
   
   SysStatus_Send_buf[0] = (uint8_t)(phy_angle & 0xFF);
   SysStatus_Send_buf[1] = (uint8_t)((phy_angle>>8) & 0xFF);
   SysStatus_Send_buf[2] = rxMarker;
   SysStatus_Send_buf[3] = rc_MLX90363_TrigMode1;
   
   *(unsigned long int *)(&SysStatus_Send_buf[4]) = MLX90363_DiagnosticDetails;
}


void CAN_Send_EtcInfo(void){
   EtcInfo_Send_buf[0] = (uint8_t)(ETC_m_pc_ActualPos & 0xFF);
   EtcInfo_Send_buf[1] = (uint8_t)((ETC_m_pc_ActualPos>>8) & 0xFF);
}