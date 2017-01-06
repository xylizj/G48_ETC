/*******************************************************************************
*
*  FILE
*     Dem_Types.h
*
*  DESCRIPTION
*     The Data Type Header file for Dem Module  
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

#ifndef _DEM_TYPES_H_
#define _DEM_TYPES_H_

/*******************************************************************************
* include files                                                 
*******************************************************************************/
#include "Std_Types.h"
#include "Dem_Cfg.h"

/*******************************************************************************
* Defines                                                                
*******************************************************************************/
typedef uint16 Dem_ChksumType;
typedef uint8  Dem_ErrStatusLengthType;
typedef uint8  Dem_EventIdType;//if need to modify??
typedef uint8  Dem_DTCStatusMaskType;

/*typedef enum
{
  DEM_ERRMEM_OK = 0,
  DEM_ERRMEM_CONFIRMED = 1,
  DEM_ERRMEM_HEALING = 2,  
}Dem_ErrMemStatusType;*/
//!< =======================modified by xyl=======================
typedef enum
{
  DEM_ERRMEM_CONFIRMED = 0x08,
  DEM_ERRMEM_HEALING = 0x01,  
}Dem_ErrMemStatusType;


typedef struct {   	    
  uint16 SPN;
  uint8 FMI;
  uint8 ErrorClass;
  uint8 HealingCounter;
}Dem_FaultCodeType;

typedef struct { 
  Dem_EventIdType EventId;
  uint8 ErrClass;
  Dem_ErrMemStatusType ErrState;                           													
  uint8 HealingCounter;
  uint8 ErrOccurCounter;   	                                                           
}Dem_FaultMemType;

typedef enum
{
  DTCFormat_ISO15031   = 0,
  DTCFormat_ISO14229   = 1,
  DTCFormat_J1939      = 2    
}Dem_DtcFormat;

/*typedef enum
{
  ALL_DTC      = 0,
  ACTIVE_DTC   = 1,
  INACTIVE_DTC = 2    
}Dem_DtcType;*/
//!< =======================modified by xyl=======================
//Ö§³ÖµÄDTCstatus  DRL
/*
#define MASK_TEST_FAILED                            0x01//Y
#define MASK_TEST_FAILED_THIS_MONITOR_CYCLE         0x02//N
#define MASK_PENDING_DTC                            0x04//N
#define MASK_CONFIRMED_DTC                          0x08//Y
#define MASK_TEST_NOT_COMPLETED_SINCE_LAST_CLEAR    0x10//N
#define MASK_TEST_FAILED_SINCE_LAST_CLEAR           0x20//N
#define MASK_TEST_NOT_COMPLETED_THIS_MONITOR_CYCLE  0x40//N
#define MASK_WARING_INDICATOR_REQUESTED             0x80//N
*/
typedef enum
{
  MASK_TEST_FAILED                                = 0x01,//Y
  MASK_TEST_FAILED_THIS_MONITOR_CYCLE             = 0x02,//N
  MASK_PENDING_DTC                                = 0x04,//N
  MASK_CONFIRMED_DTC                              = 0x08,//Y
  MASK_TEST_NOT_COMPLETED_SINCE_LAST_CLEAR        = 0x10,//N
  MASK_TEST_FAILED_SINCE_LAST_CLEAR               = 0x20,//N 
  MASK_TEST_NOT_COMPLETED_THIS_MONITOR_CYCLE      = 0x40,//N
  MASK_WARING_INDICATOR_REQUESTED                 = 0x80,//N  
}Dem_DtcType;

typedef struct
{
  bittype active:1;
  bittype mem:1;
  bittype resv:6;
}Dem_FaultStatusType;

typedef struct
{	
	uint8	year;	
	uint8	month;	
	uint8	day;	
	uint8	hour;	
	uint8	min;	
	uint8	second;	
}Dem_FreezeFrameType;


/*******************************************************************************
* Macros                                                                
*******************************************************************************/

/*******************************************************************************
* Global Variables declaration                         
*******************************************************************************/

/*******************************************************************************
* Global functions declaration                         
*******************************************************************************/

#endif /* #ifndef _DEM_TYPES_H_ */
