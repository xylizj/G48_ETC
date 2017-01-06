/*******************************************************************************
*
*  FILE
*     Dem.h
*
*  DESCRIPTION
*     The Header file for Dem Module
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

#ifndef _DEM_H_
#define _DEM_H_

/*******************************************************************************
* include files                                                 
*******************************************************************************/
#include "Std_Types.h"
#include "Dem_Types.h"

/*******************************************************************************
* Defines                                                                
*******************************************************************************/

/* Vendor ID. */
#define DEM_VENDOR_ID           6666

/* Module ID  */
#define DEM_MODULE_ID           54

/* Version number of the module */
#define DEM_SW_MAJOR_VERSION    1
#define DEM_SW_MINOR_VERSION    0
#define DEM_SW_PATCH_VERSION    0

#define DEM_INSTANCE_ID                       1

#define DEM_API_SETERROR_ID                   1
#define DEM_API_ERRMEMUPDATE_ID               2


#define DEM_E_INVALID_INDEX                   1
#define DEM_E_INVALID_ERRSTATUS               2


#define DEM_ERR_MEM_UPDATE_OK         ((uint8)0)
#define DEM_ERR_MEM_FULL              ((uint8)1)
#define DEM_ERR_MEM_INDEX_NA          ((uint8)2)

#define DEM_ERR_PASSIVE               ((uint8)0)
#define DEM_ERR_ACTIVE                ((uint8)1)

#define DEM_ERR_STATUS_MAX            (DEM_ERR_ACTIVE)

#define DEM_NOT_FOUND                 ((uint8)0xFF)

#define DEM_NO_ERR                    ((uint8)0xFF)

#define DEM_NO_PARAM                  ((uint8)0xFF)

#define DEM_MAX_ERR_CLASS             ((uint8)0xFF)



#define DEM_EVENT_ID_NA               ((Dem_EventIdType)0xFFFF)
/*******************************************************************************
* Macros                                                                
*******************************************************************************/

/*******************************************************************************
* Global Variables declaration                         
*******************************************************************************/
extern VAR(boolean,DEM_VAR) Dem_stClear;
extern boolean dem_status;
extern volatile VAR(Dem_FreezeFrameType,DEM_VAR) dem_FreezeFrame[DEM_MEM_ENTRY_NUM];
/*******************************************************************************
* Global Constant declaration                         
*******************************************************************************/
extern CONST(boolean,DEM_CONST) Dem_stClear_C;
extern CONST(Dem_FaultCodeType,DEM_CONST) Dem_ErrJ1939FaultCode[DEM_EVENT_NUM];

/*******************************************************************************
* Global functions declaration                         
*******************************************************************************/
extern FUNC(void,DEM_CODE)  Dem_Init(void);
extern FUNC(void,DEM_CODE)  Dem_MainFunction(void);
extern FUNC(void,DEM_CODE)  Dem_AfterRunChkSum(void);
extern FUNC(void,DEM_CODE)  Dem_FaultMemClearAll(void);
extern FUNC(Std_ReturnType,DEM_CODE)  Dem_SetEventStatus(Dem_EventIdType EventId, uint8 EventStatus);
extern FUNC(uint8,DEM_CODE) Dem_DtcAssembly(uint8* dataPtr,Dem_DtcType type);
extern FUNC(uint8,DEM_CODE) Dem_GetError(Dem_EventIdType EventId);
extern FUNC(Dem_ErrStatusLengthType,DEM_CODE) Dem_GetErrorStatus(Dem_EventIdType EventId, uint8* buffer);

extern FUNC(Dem_ErrStatusLengthType,DEM_CODE) Dem_ApplGetErrorStatus(uint8 EntryNum, uint8* buffer);

extern FUNC(Std_ReturnType,DEM_CODE) Dem_ApplDrvCycleRisingEdge(void);


extern FUNC(Dem_ErrStatusLengthType,DEM_CODE) Dem_GetNumberOfDTCByStatusMask(uint8* dataPtr, Dem_DTCStatusMaskType DTCStsMask);
extern FUNC(Dem_ErrStatusLengthType,DEM_CODE) Dem_GetDTCByStatusMask(uint8* dataPtr, Dem_DTCStatusMaskType DTCStsMask);
extern FUNC(Dem_ErrStatusLengthType,DEM_CODE) Dem_GetSupportedDTC(uint8* dataPtr);


















#endif /* #ifndef _DEM_H_ */
