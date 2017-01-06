#ifndef __CAN_H_APP__
#define __CAN_H_APP__

#include "typedefs.h"


extern uint8_t SysStatus_Send_buf[8];
extern uint8_t EtcInfo_Send_buf[8];


extern unsigned int alpha,beta;
extern unsigned char rxMarker; 
extern uint8_t  rc_MLX90363_TrigMode1;
extern unsigned long int MLX90363_DiagnosticDetails;

extern void CAN_Send_SysStatus(void);
extern void CAN_Send_EtcInfo(void);


#endif /*End of __HACG_CAN_H__ */