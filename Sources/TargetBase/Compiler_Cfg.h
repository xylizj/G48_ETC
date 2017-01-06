/*******************************************************************************
*
*  FILE
*     Compiler_Cfg.h
*
*  DESCRIPTION
*     Module specific configuration 
*      
*       
*  COPYRIGHT
*     
*     All rights reserved.
*
*  AUTHOR
*    
*
*  VERSION
*    1.00
*
*******************************************************************************/

#ifndef _COMPILER_CFG_H_
#define _COMPILER_CFG_H_

/*******************************************************************************
* include files                                                 
*******************************************************************************/

#include <string.h>

/*******************************************************************************
*                      Configuration data                                  
*******************************************************************************/

/*****************************************************************
*           MCU (MCU Driver)                                  
******************************************************************/

#define MCU_CODE
#define MCU_CODE_NEAR __near
#define MCU_CONST     
#define MCU_CONST_NEAR __near
#define MCU_VAR 
#define MCU_VAR_FAR  __far 

#define MCU_APPL_DATA

/*****************************************************************
*           OS (Operating System)                                 
******************************************************************/
#define OS_CONST      MCU_CONST
#define OS_CODE_ISR   MCU_CODE_NEAR
#define OS_CODE_NEAR  MCU_CODE_NEAR
#define OS_CODE       MCU_CODE


#define OS_VAR        MCU_VAR
#define OS_VAR_FAR    MCU_VAR_FAR
/*****************************************************************
*           DET                                  
******************************************************************/

#define DET_CODE      MCU_CODE
#define DET_CODE_NEAR MCU_CODE_NEAR

/*****************************************************************
*           CAN (CAN Driver)                                  
******************************************************************/

#define CAN_CODE
#define CAN_ISR_CODE	MCU_CODE_NEAR
#define CAN_CODE_NEAR MCU_CODE_NEAR

#define CAN_CONST  MCU_CONST

#define CAN_CONST_NEAR MCU_CONST_NEAR

#define CAN_VAR

#define CAN_VAR_FAR  MCU_VAR_FAR

/*****************************************************************
*           CANIF                                  
******************************************************************/
#define CANIF_CODE MCU_CODE
#define CANIF_VAR  MCU_VAR
#define CANIF_VAR_FAR  MCU_VAR_FAR
#define CANIF_CONST MCU_CONST

/*****************************************************************
*           J1939TP                                  
******************************************************************/
#define J1939TP_CODE MCU_CODE
#define J1939TP_VAR  MCU_VAR

/*****************************************************************
*           J1939                                 
******************************************************************/
#define J1939_CODE MCU_CODE
#define J1939_VAR  MCU_VAR

/*****************************************************************
*           ADC                                 
******************************************************************/

#define ADC_CODE
#define ADC_CODE_NEAR MCU_CODE_NEAR

/*****************************************************************
*           DIO                                   
******************************************************************/

#define DIO_CODE
#define DIO_CODE_NEAR MCU_CODE_NEAR

/*****************************************************************
*           GPT                                 
******************************************************************/

#define GPT_CODE
#define GPT_CODE_NEAR MCU_CODE_NEAR

#define GPT_ISR_CODE  MCU_CODE_NEAR

/*****************************************************************
*           EEP                                 
******************************************************************/

#define EEP_CODE
#define EEP_CODE_NEAR MCU_CODE_NEAR

#define EEP_VAR       MCU_VAR

/*****************************************************************
*           PWM                                 
******************************************************************/

#define PWM_CODE
#define PWM_CODE_NEAR MCU_CODE_NEAR

/*****************************************************************
*           PWM                                 
******************************************************************/

#define WDG_CODE
#define WDG_CODE_NEAR MCU_CODE_NEAR

/*****************************************************************
*           SCHM                                
******************************************************************/

#define SCHM_CODE
#define SCHM_CODE_NEAR MCU_CODE_NEAR

/*****************************************************************
*           ICU                                
******************************************************************/

#define ICU_CODE
#define ICU_CODE_NEAR MCU_CODE_NEAR

#define ICU_VAR     MCU_VAR
/*****************************************************************
*           XGATE                                
******************************************************************/

#define XGATES_CODE
#define XGATES_CODE_NEAR MCU_CODE_NEAR

#define XGATES_CONST     MCU_CONST

#define XGATES_VAR       MCU_VAR

/*****************************************************************
*           IOA                                
******************************************************************/
#define IOA_CODE
#define IOA_CODE_NEAR MCU_CODE_NEAR

#define IOA_CONST     MCU_CONST

#define IOA_VAR       MCU_VAR

/*****************************************************************
*           CANTP                               
******************************************************************/
#define CANTP_CODE
#define CANTP_CODE_NEAR MCU_CODE_NEAR

#define CANTP_CONST     MCU_CONST

#define CANTP_VAR       MCU_VAR

/*****************************************************************
*           DCM                               
******************************************************************/
#define DCM_CODE
#define DCM_CODE_NEAR MCU_CODE_NEAR

#define DCM_CONST     MCU_CONST

#define DCM_VAR       MCU_VAR

#define DCM_APPL_DATA MCU_APPL_DATA

/*****************************************************************
*           ECUM                               
******************************************************************/
#define ECUM_CODE
#define ECUM_CODE_NEAR MCU_CODE_NEAR

#define ECUM_CONST     MCU_CONST

#define ECUM_VAR       MCU_VAR

#define ECUM_APPL_DATA MCU_APPL_DATA

/*****************************************************************
*           DEM                               
******************************************************************/
#define DEM_CODE
#define DEM_CODE_NEAR MCU_CODE_NEAR

#define DEM_CONST     MCU_CONST

#define DEM_VAR       MCU_VAR

#define DEM_APPL_DATA MCU_APPL_DATA

#define DEM_VAR_FAR   MCU_VAR_FAR  

/*****************************************************************
*           FIM                               
******************************************************************/
#define FIM_CODE
#define FIM_CODE_NEAR MCU_CODE_NEAR

#define FIM_CONST     MCU_CONST

#define FIM_VAR       MCU_VAR

#define FIM_APPL_DATA MCU_APPL_DATA

#define FIM_VAR_FAR   MCU_VAR_FAR 

/*****************************************************************
*           XCP                               
******************************************************************/
#define XCP_CODE
#define XCP_CODE_NEAR MCU_CODE_NEAR

#define XCP_CONST     MCU_CONST

#define XCP_VAR       MCU_VAR

#define XCP_APPL_DATA MCU_APPL_DATA

#define XCP_VAR_FAR   MCU_VAR_FAR 

/*****************************************************************
*           COM                               
******************************************************************/
#define COM_CODE
#define COM_CODE_NEAR MCU_CODE_NEAR

#define COM_CONST     MCU_CONST

#define COM_VAR       MCU_VAR

#define COM_APPL_DATA MCU_APPL_DATA

#define COM_VAR_FAR   MCU_VAR_FAR 

/*****************************************************************
*           TLE8104                                 
******************************************************************/

#define TLE8104_CODE
#define TLE8104_CODE_NEAR MCU_CODE_NEAR

#define TLE8104_ISR_CODE  MCU_CODE_NEAR

#define TLE8104_VAR       MCU_VAR


#endif /* _COMPILER_CFG_H_ */
