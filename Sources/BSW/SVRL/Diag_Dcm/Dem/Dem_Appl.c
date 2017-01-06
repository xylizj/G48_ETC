/*******************************************************************************
*
*  FILE
*     Dem_Appl.c
*
*  DESCRIPTION
*       
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

/*******************************************************************************
* include files                                                 
*******************************************************************************/
#include "Dem.h"
/*******************************************************************************
* Defines                                                                
*******************************************************************************/




/*******************************************************************************
* Macros                                                                
*******************************************************************************/
#define DEM_FREEFRAME_SIZE    (sizeof(Dem_FreezeFrameType))

/*******************************************************************************
* Global Constant definition                         
*******************************************************************************/

/*******************************************************************************
* Local Constant definition                         
*******************************************************************************/
/*******************************************************************************
* Global Variables definition                         
*******************************************************************************/
uint16 dem_runTime;
/*******************************************************************************
* Local Variables definition                         
*******************************************************************************/
_STATIC_ VAR(boolean,DEM_VAR) dem_DrvCycleReachFlag; 







/*******************************************************************************
* Local Functions prototypes                         
*******************************************************************************/


/*******************************************************************************
*  Global Functions Body                                   
*******************************************************************************/

/*******************************************************************************
* NAME:             Dem_ApplGetErrorStatus
* CALLED BY:        Dem_GetErrorStatus
* PRECONDITIONS:
* INPUT PARAMETERS: uint8 EntryNum
*                   uint8* buffer: Data Buffer Pointer
* RETURN VALUES:    Dem_ErrStatusLengthType: the Error Status Length
* DESCRIPTION:      Get the Error Status (Freeze Frame)  
*******************************************************************************/

FUNC(Dem_ErrStatusLengthType,DEM_CODE) Dem_ApplGetErrorStatus(uint8 EntryNum, uint8* buffer)
{
  uint8 index;
  uint8* __far srcPtr;
  
  srcPtr = (uint8 *)(&dem_FreezeFrame[EntryNum]);
  /* Implemented by the customer*/
  for (index = 0; index<DEM_FREEFRAME_SIZE; index++)
  {
    *buffer = *srcPtr;
    buffer++;
    srcPtr++;
  }

  return(DEM_FREEFRAME_SIZE);
}
/*******************************************************************************
* NAME:             Dem_ApplDrvCycleRisingEdge
* CALLED BY:        Dem_MainFunction
* PRECONDITIONS:
* INPUT PARAMETERS: Void
* RETURN VALUES:    E_OK: Driving Cycle is reached
*                   E_NOT_OK: Driving Cycle is not reached.
* DESCRIPTION:      Get the Error Status (Freeze Frame)  
*******************************************************************************/
FUNC(Std_ReturnType,DEM_CODE) Dem_ApplDrvCycleRisingEdge(void)
{
  /* Implemented by the customer*/

  if (dem_DrvCycleReachFlag == FALSE)
  {
    if (dem_runTime++ > 1) // euqal to 30 seconds -> 600
    {	
      dem_DrvCycleReachFlag = TRUE;	
      return E_OK;
    }     
  }
  
  return E_NOT_OK;
}


/*******************************************************************************
* NAME:             Dem_ApplFreezeFrameStore
* CALLED BY:        Dem_ErrStoreToMem
* PRECONDITIONS:
* INPUT PARAMETERS: uint8 entryNum
* RETURN VALUES:    Dem_ErrStatusLengthType: the Error Status Length
* DESCRIPTION:      Get the Error Status (Freeze Frame)  
*******************************************************************************/
FUNC(void,DEM_CODE)Dem_ApplFreezeFrameStore(uint8 entryNum)
{/*
  dem_FreezeFrame[entryNum].second = sd2405_CurTime.second;   
  dem_FreezeFrame[entryNum].min = sd2405_CurTime.minute; 
  dem_FreezeFrame[entryNum].hour = sd2405_CurTime.hour; 
  dem_FreezeFrame[entryNum].month = sd2405_CurTime.month; 
  dem_FreezeFrame[entryNum].day = sd2405_CurTime.day; 
  dem_FreezeFrame[entryNum].year = sd2405_CurTime.year;       
*/}

/*******************************************************************************
*  Local Functions Body                                   
*******************************************************************************/


