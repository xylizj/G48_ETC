/*******************************************************************************
*
*  FILE
*     Dem_Cfg.h
*
*  DESCRIPTION
*     The Configuration Header file for Dem Module
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
*    1.1.0
*
*******************************************************************************/

#ifndef _DEM_CFG_H_
#define _DEM_CFG_H_

/*******************************************************************************
* include files                                                 
*******************************************************************************/
#include "Std_Types.h"
#include "Dem_Types.h"

/*******************************************************************************
* Defines                                                                
*******************************************************************************/
#define DEM_DEV_ERROR_DETECT        STD_ON

#define DEM_EVENT_NUM               50U

#define DEM_ERROR_COL_NUM           (DEM_EVENT_NUM/16+1)

#define DEM_MEM_ENTRY_NUM           10U

#define DEM_CLR_DELAY               10U

/* When set on, the fault memory placed in Ram and cannot be stored 
 simply used for debugger */
//#define DEM_FAULT_MEM_DEBUG         STD_OFF

/* Event Id */
#define HVA1_TIME_OUT                  0U
#define HVM1_TIME_OUT                  (HVA1_TIME_OUT+1)
#define M1CMT_TIME_OUT                 (HVM1_TIME_OUT+1)
#define M1STS1_TIME_OUT                (M1CMT_TIME_OUT+1)
#define M1STS2_TIME_OUT                (M1STS1_TIME_OUT+1) //
#define M2CMT_TIME_OUT                 (M1STS2_TIME_OUT+1)
#define M2STS1_TIME_OUT                (M2CMT_TIME_OUT+1)
#define M2STS2_TIME_OUT                (M2STS1_TIME_OUT+1) 
#define M3CMT_TIME_OUT                 (M2STS2_TIME_OUT+1)
#define M3STS1_TIME_OUT                (M3CMT_TIME_OUT+1)
#define M3STS2_TIME_OUT                (M3STS1_TIME_OUT+1) 
#define M4CMT_TIME_OUT                 (M3STS2_TIME_OUT+1)
#define M4STS1_TIME_OUT                (M4CMT_TIME_OUT+1)
#define M4STS2_TIME_OUT                (M4STS1_TIME_OUT+1) //30
#define CCP1_TIME_OUT                  (M4STS2_TIME_OUT+1) //31
#define MSP1_TIME_OUT                  (CCP1_TIME_OUT+1) //32
#define MSP3_TIME_OUT                  (MSP1_TIME_OUT+1) //33
#define VD_TIME_OUT                    (MSP3_TIME_OUT+1) //34
#define CS_TIME_OUT                    (VD_TIME_OUT+1) //35
#define CC_TIME_OUT                    (CS_TIME_OUT+1) //35
#define INN_CAN_BUS_OFF                (CC_TIME_OUT+1)
#define VEH_CAN_BUS_OFF                (INN_CAN_BUS_OFF+1)
#define CHRG_CAN_BUS_OFF               (VEH_CAN_BUS_OFF+1)
#define EEPROM_ERR                     (CHRG_CAN_BUS_OFF+1)
#define TLE8104_INIT_ERR1              (EEPROM_ERR+1)
#define TLE8104_INIT_ERR2              (TLE8104_INIT_ERR1+1)
#define TLE8104_INIT_ERR3              (TLE8104_INIT_ERR2+1)
#define TLE8104_SPI_ERR1               (TLE8104_INIT_ERR3+1)
#define TLE8104_SPI_ERR2               (TLE8104_SPI_ERR1+1)
#define TLE8104_SPI_ERR3               (TLE8104_SPI_ERR2+1)
#define CLK_ERR                        (TLE8104_SPI_ERR3+1)


#define BATT_PACK_TEMP_HIGH        (CLK_ERR+1)
#define FAST_CHRG_TEMP_HIGH        (BATT_PACK_TEMP_HIGH+1)
#define SLOW_CHRG_TEMP_HIGH        (FAST_CHRG_TEMP_HIGH+1)
#define VOLTAGE_LOW                (SLOW_CHRG_TEMP_HIGH+1)
#define DISCHRG_CURR_HIGH          (VOLTAGE_LOW+1)
#define FAST_CHRG_CURR_HIGH        (DISCHRG_CURR_HIGH+1)
#define SLOW_CHRG_CURR_HIGH        (FAST_CHRG_CURR_HIGH+1)
#define ISULATION_ERROR            (SLOW_CHRG_CURR_HIGH+1)
#define CELL_VOLT_HIGH             (ISULATION_ERROR+1)
#define CELL_VOLT_LOW              (CELL_VOLT_HIGH+1)
#define CELL_VOLT_DIFF_HIGH        (CELL_VOLT_LOW+1)
#define NEG_RELAY_CONTACT_ERROR    (CELL_VOLT_DIFF_HIGH+1)
#define SOC_LOW                    (NEG_RELAY_CONTACT_ERROR+1)
#define BATT_PACK_TEMP_LOW         (SOC_LOW+1)
#define T30_VOLT_LOW               (BATT_PACK_TEMP_LOW+1)
#define T30_VOLT_HIGH              (T30_VOLT_LOW+1)
#define CHRG_12V_LOW               (T30_VOLT_HIGH+1)
#define CHRG_12V_HIGH              (CHRG_12V_LOW+1)
#define CTL_12V_OUT_ERROR              (CHRG_12V_HIGH+1)
/*******************************************************************************
* Macros                                                                
*******************************************************************************/

/*******************************************************************************
* Global Variables declaration                         
*******************************************************************************/

/*******************************************************************************
* Global functions declaration                         
*******************************************************************************/



#endif /* #ifndef _DEM_CFG_H_ */





