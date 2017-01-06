/*******************************************************************************
 *
 *  FILE
 *     Dem.c
 *
 *  DESCRIPTION
 *     Source Code File for Dem Module  
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
//#include "Det.h"
//#include "Com.h"
//#include "Eep.h"

/*******************************************************************************
 * Defines                                                                
 *******************************************************************************/

/*******************************************************************************
 * Macros                                                                
 *******************************************************************************/

/*******************************************************************************
 * Global Constant definition                         
 *******************************************************************************/
VAR(boolean,DEM_VAR) Dem_stClear;

/*******************************************************************************
 * Local Constant definition                         
 *******************************************************************************/

/*******************************************************************************
 * Global Variables definition                         
 *******************************************************************************/

boolean dem_status;

/*******************************************************************************
 * Local Variables definition                         
 *******************************************************************************/
_STATIC_ VAR(uint8,DEM_VAR) dem_ClrCnt;
_STATIC_ VAR(uint16,DEM_VAR) dem_ErrCol_mp[DEM_ERROR_COL_NUM];
_STATIC_ VAR(Dem_FaultStatusType,DEM_VAR) dem_errStatus[DEM_EVENT_NUM];
_STATIC_ VAR(uint8,DEM_VAR) dem_MemEntryNextEmpty;
_STATIC_ VAR(boolean,DEM_VAR) Dem_stClearOld;


_STATIC_ volatile VAR(Dem_FaultMemType,DEM_VAR) dem_ErrMemoEntry[DEM_MEM_ENTRY_NUM];
volatile VAR(Dem_FreezeFrameType,DEM_VAR) dem_FreezeFrame[DEM_MEM_ENTRY_NUM];




//这里原来就是为了指定下面几个变量保存在程序内存还是Flash，还是EEPROM内
#define DEM_START_SEC_FAULT_ROM
//#include "MemMap.h" //

_STATIC_ volatile VAR(Dem_FaultMemType,DEM_VAR_FAR) dem_ErrMemoEntry_MEM[DEM_MEM_ENTRY_NUM];
_STATIC_ volatile VAR(Dem_ChksumType,DEM_VAR_FAR) dem_ErrMemoCheckSum;
_STATIC_ volatile VAR(Dem_FreezeFrameType,DEM_VAR_FAR) dem_FreezeFrame_MEM[DEM_MEM_ENTRY_NUM];


#define DEM_STOP_SEC_FAULT_ROM
//#include "MemMap.h"


/*******************************************************************************
 * Local Functions prototypes                         
 *******************************************************************************/
_STATIC_ FUNC(Dem_ChksumType,DEM_CODE) Dem_FaultMemChksumCal(void);
_STATIC_ FUNC(void,DEM_CODE) Dem_ErrMemUpdate(Dem_EventIdType EventId, uint8 EventStatus);
_STATIC_ FUNC(void,DEM_CODE) Dem_ErrColUpdate(Dem_EventIdType errorIndex, uint8 errorStatus);
_STATIC_ FUNC(void,DEM_CODE) Dem_FaultMemClearOneEntry(uint8 entryNum);
_STATIC_ FUNC(void,DEM_CODE) Dem_ErrStoreToMem(Dem_EventIdType EventId,uint8 entryNum);
_STATIC_ FUNC(void,DEM_CODE) Dem_UpdateHealingState(void);
_STATIC_ FUNC(void,DEM_CODE) Dem_ErrStatusInit(void);

/*******************************************************************************
 *  Global Functions Body                                   
 *******************************************************************************/

/*******************************************************************************
 * NAME:             Dem_Init
 * CALLED BY:        Application
 * PRECONDITIONS:
 * INPUT PARAMETERS: Void
 * RETURN VALUES:    Void
 * DESCRIPTION:      Initialize Fault Memory   
 *******************************************************************************/
FUNC(void,DEM_CODE) Dem_Init(void)
{
	uint16 index;
	uint8* srcPtr;
	uint8* desPtr;

	/*if (eep_Status == E_NOT_OK)//这里应该是使用EEPROM的情况，再这个函数被调用之前肯定调用EEPROM初始化
	{
		Dem_FaultMemClearAll();
		dem_MemEntryNextEmpty = 0;
		dem_status = E_NOT_OK; 
		return;   
	}*/

	srcPtr = (uint8*)(&dem_ErrMemoEntry_MEM);
	desPtr = (uint8*)(&dem_ErrMemoEntry);
	for(index = 0; index<sizeof(dem_ErrMemoEntry); index++)
	{
		*desPtr = *srcPtr;
		desPtr++;
		srcPtr++; 
	}

	srcPtr = (uint8*)(&dem_FreezeFrame_MEM);
	desPtr = (uint8*)(&dem_FreezeFrame);
	for(index = 0; index<sizeof(dem_FreezeFrame); index++)
	{
		*desPtr = *srcPtr;
		desPtr++;
		srcPtr++; 
	}  

	/* Check if the checksum is okay, if not, clear the fault memory */
	//这里初始化的时候计算故障内存校验和与上一次下电保存计算并保存下来的比较
	//如果不一致说明上次保存的故障内存出现了变化，Dem初始化失败，调用此函数的上一层就知道Dem初始化失败
	if (Dem_FaultMemChksumCal() != dem_ErrMemoCheckSum)
	{    
		Dem_FaultMemClearAll();
		dem_MemEntryNextEmpty = 0;
		dem_status = E_NOT_OK;
	}
	else
	{
		Dem_ErrStatusInit(); 
		dem_status = E_OK;    
	}
}

/*******************************************************************************
 * NAME:             Dem_GetErrorStatus
 * CALLED BY:        Application
 * PRECONDITIONS:
 * INPUT PARAMETERS: Dem_EventIdType EventId:
 *                   uint8* buffer: Data Buffer Pointer
 * RETURN VALUES:    Dem_ErrStatusLengthType: the Error Status Length
 *                   0 represent error not found
 * DESCRIPTION:      Get the Error Status (Freeze Frame)  
 *******************************************************************************/
FUNC(Dem_ErrStatusLengthType,DEM_CODE) Dem_GetErrorStatus(Dem_EventIdType EventId, uint8* buffer)
{
	Dem_EventIdType idx;
	Dem_ErrStatusLengthType length;

	length = 0;

	for (idx = 0;idx < dem_MemEntryNextEmpty;/*10*/ idx++)
	{
		if (EventId == dem_ErrMemoEntry[idx].EventId)
		{
			break;   
		}
	}

	/* check if DTC found*/
	if (idx < dem_MemEntryNextEmpty)
	{
		*buffer = dem_ErrMemoEntry[idx].ErrOccurCounter;
		buffer++;
		length = 1;
		/* Add more user defined freeze frame data into buffer */
		length += Dem_ApplGetErrorStatus(idx,buffer);

	}
	return (length);
}





/*******************************************************************************
 * NAME:             Dem_GetNumberOfDTCByStatusMask
 * CALLED BY:        Dcm_ApplReportNumberOfDTCByStatusMask
 * PRECONDITIONS:
 * INPUT PARAMETERS: Dem_DTCStatusMaskType DTCStsMask
 * 						uint8* buffer: Data Buffer Pointer
 * RETURN VALUES:    Dem_ErrStatusLengthType: the Error Status Length
 *                   0 represent error not found
 * DESCRIPTION:      Get the Get Number Of DTC By Status Mask  
 *******************************************************************************/
/*
FUNC(Dem_ErrStatusLengthType,DEM_CODE) 
Dem_GetNumberOfDTCByStatusMask(uint8* dataPtr, 
      Dem_DTCStatusMaskType DTCStsMask)
{
	uint8 errNum;//uint8 errNum;
	uint8 length;
	uint8 index;
	uint8* tmpDataPtr;

	errNum = 0;
	tmpDataPtr = dataPtr;

	for (index=0; index < dem_MemEntryNextEmpty; index++)
	{
		if (DTCStsMask == ALL_DTC)
		{
			errNum++;
		}
		else if ((DTCStsMask == ACTIVE_DTC)&&(dem_ErrMemoEntry[index].ErrState == DEM_ERRMEM_CONFIRMED))
		{
			errNum++;			  
		}
		else if ((DTCStsMask == INACTIVE_DTC)&&(dem_ErrMemoEntry[index].ErrState == DEM_ERRMEM_HEALING))
		{	    
			errNum++;			
		}	  
	}  	
	dataPtr = errNum;
	length =  1;
	return (length); 		
}
*/
FUNC(Dem_ErrStatusLengthType,DEM_CODE) 
Dem_GetNumberOfDTCByStatusMask(uint8* dataPtr, Dem_DTCStatusMaskType DTCStsMask)
{
	uint8 errNum;
	uint8 length;
	uint8 index;

	errNum = 0;

	for (index=0; index < dem_MemEntryNextEmpty; index++)
	{
		/*if (DTCStsMask == ALL_DTC)
		{
			errNum++;
		}
		else */if ((DTCStsMask == MASK_CONFIRMED_DTC)&&(dem_ErrMemoEntry[index].ErrState == DEM_ERRMEM_CONFIRMED))
		{
			errNum++;			  
		}
		else if ((DTCStsMask == MASK_TEST_FAILED)&&(dem_ErrMemoEntry[index].ErrState == DEM_ERRMEM_HEALING))
		{	    
			errNum++;			
		}	  
	}  	
	*(uint16*)dataPtr = errNum;
	length =  2;
	return (length); 
}





/*******************************************************************************
 * NAME:             Dem_GetDTCByStatusMask
 * CALLED BY:        Dcm_ApplReportDTCByStatusMask
 * PRECONDITIONS:
 * INPUT PARAMETERS: Dem_DTCStatusMaskType DTCStsMask
 * 						uint8* buffer: Data Buffer Pointer
 * RETURN VALUES:    Dem_ErrStatusLengthType: the Error Status Length
 *                   0 represent error not found
 * DESCRIPTION:      Get the Get Number Of DTC By Status Mask  
 *******************************************************************************/
FUNC(Dem_ErrStatusLengthType,DEM_CODE) Dem_GetDTCByStatusMask(uint8* dataPtr, Dem_DTCStatusMaskType DTCStsMask)
		//Dem_DTCStatusMaskType DTCStsMask = &pMsgContext->reqData[0]
{
	uint8 errNum;
	uint8 length;
	uint8 index;
	uint8* tmpDataPtr;

	errNum = 0;
	tmpDataPtr = dataPtr;

	for (index=0; index < dem_MemEntryNextEmpty; index++)
	{
		/*if (DTCStsMask == ALL_DTC)
		{
			errNum++;
			*(uint16 *)tmpDataPtr = (uint16)(dem_ErrMemoEntry[index].EventId);        
			tmpDataPtr +=2;
			*(uint8 *)tmpDataPtr =  (uint8)(dem_ErrMemoEntry[index].ErrState);
			tmpDataPtr +=1;	    
		}
		else */if ((DTCStsMask == MASK_CONFIRMED_DTC)&&(dem_ErrMemoEntry[index].ErrState == DEM_ERRMEM_CONFIRMED))
		{
			errNum++;
			*(uint16 *)tmpDataPtr = (uint16)(dem_ErrMemoEntry[index].EventId);        
			tmpDataPtr +=2;
			*(uint8 *)tmpDataPtr =  (uint8)(dem_ErrMemoEntry[index].ErrState); 
			tmpDataPtr +=1;	  
		}
		else if ((DTCStsMask == MASK_TEST_FAILED)&&(dem_ErrMemoEntry[index].ErrState == DEM_ERRMEM_HEALING))
		{	    
			errNum++;
			*(uint16 *)tmpDataPtr = (uint16)(dem_ErrMemoEntry[index].EventId);
			tmpDataPtr +=2;
			*(uint8 *)tmpDataPtr =  (uint8)(dem_ErrMemoEntry[index].ErrState);
			tmpDataPtr +=1;
		}	  
	}  

	length = errNum * 3;
	return (length);
}


FUNC(Dem_ErrStatusLengthType,DEM_CODE) Dem_GetSupportedDTC(uint8* dataPtr)
{
	uint8 errNum;
	uint8 length;
	uint8 index;
	uint8* tmpDataPtr;

	errNum = 0;
	tmpDataPtr = dataPtr;

  //modified by xyl 2015-04-24
	for (index=0; index<DEM_EVENT_NUM; index++)//发送所有支持的故障的状态，Dem_FaultStatusType active mem
	{
		errNum++;
		*(uint16 *)tmpDataPtr = (uint16)(index);        
		tmpDataPtr +=2;
		*(uint8 *)tmpDataPtr =  *(uint8*)(&dem_errStatus[index]);
		tmpDataPtr +=1;	      
	}

	length = errNum * 3;
	return (length);
}

/*******************************************************************************
 * NAME:             Dem_SetEventStatus
 * CALLED BY:        Application Dcm_ControlDTCSetting()
 * PRECONDITIONS:
 * INPUT PARAMETERS: Dem_EventIdType EventId:
 *                   uint8* buffer: Data Buffer Pointer
 * RETURN VALUES:    Void
 * DESCRIPTION:      Set the Error Status 
 *******************************************************************************/
FUNC(Std_ReturnType,DEM_CODE) Dem_SetEventStatus(Dem_EventIdType EventId, uint8 EventStatus)
{  
#if (DEM_DEV_ERROR_DETECT == STD_ON)
	if (EventId >= DEM_EVENT_NUM)//50
	{
		Det_ReportError(DEM_MODULE_ID,DEM_INSTANCE_ID,DEM_API_SETERROR_ID,DEM_E_INVALID_INDEX);
		return E_NOT_OK;
	}
	if (EventStatus > DEM_ERR_STATUS_MAX)//DEM_ERR_STATUS_MAX = 1
	{
		Det_ReportError(DEM_MODULE_ID,DEM_INSTANCE_ID,DEM_API_SETERROR_ID,DEM_E_INVALID_ERRSTATUS);
		return E_NOT_OK;
	}
#endif   
	if (dem_errStatus[EventId].active!= EventStatus)
	{ 
		dem_errStatus[EventId].active = EventStatus;
		Dem_ErrColUpdate(EventId,EventStatus);	
		//FiM_DemTriggerOnEventStatus(EventId,EventStatus);
		Dem_ErrMemUpdate(EventId,EventStatus); 
	}
	/* UPDATE THE FAULT MEM WHENEVER THE FAULT IS ACTIVE AND NOT IN MEM */
	else if ((dem_errStatus[EventId].active == TRUE) &&\
			(dem_errStatus[EventId].mem == FALSE))
	{
		Dem_ErrMemUpdate(EventId,EventStatus);  
	}
	return E_OK;
}

/*******************************************************************************
 * NAME:             Dem_GetError
 * CALLED BY:        Application
 * PRECONDITIONS:
 * INPUT PARAMETERS: Dem_EventIdType EventId:
 * RETURN VALUES:    uint8: Error Status
 * DESCRIPTION:      Get the Error Status 
 *******************************************************************************/
FUNC(uint8,DEM_CODE) Dem_GetError(Dem_EventIdType EventId)
{
	return (dem_errStatus[EventId].active);
}

/*******************************************************************************
 * NAME:             Dem_MainFunction
 * CALLED BY:        Application
 * PRECONDITIONS:
 * INPUT PARAMETERS: Void
 * RETURN VALUES:    Void
 * DESCRIPTION:      Must be called cyclely,
 *******************************************************************************/
FUNC(void,DEM_CODE) Dem_MainFunction(void)
{ 

	if(Dem_ApplDrvCycleRisingEdge() == E_OK)
	{	
		Dem_UpdateHealingState();		
	}     

	/* Clear the Fault Memory using Calibration Tool */
	if ((Dem_stClear_C == TRUE)&&(Dem_stClearOld != Dem_stClear_C))
	{
		Dem_FaultMemClearAll();    
	}
	Dem_stClearOld = Dem_stClear_C;

	/* Delay the Clear to reset the Application Fault Monitoring */
	if (Dem_stClear == TRUE)
	{
		dem_ClrCnt++;
		if (dem_ClrCnt > DEM_CLR_DELAY)
		{
			Dem_stClear = FALSE;
			dem_ClrCnt = 0;
		}

	} 
}

/*******************************************************************************
 * NAME:             Dem_AfterRunChkSum
 * CALLED BY:        Application
 * PRECONDITIONS:    Called before store to EEE
 * INPUT PARAMETERS: Void
 * RETURN VALUES:    Void
 * DESCRIPTION:      Must be called at ECU afterRun state to calculate the 
 *                   checksum of fault memory
 *******************************************************************************/
FUNC(void,DEM_CODE) Dem_AfterRunChkSum(void)
{
	uint16 index;
	uint8* srcPtr;
	uint8* desPtr;

	srcPtr = (uint8*)(&dem_ErrMemoEntry_MEM);
	desPtr = (uint8*)(&dem_ErrMemoEntry);
	for(index = 0; index<sizeof(dem_ErrMemoEntry); index++)
	{
		*srcPtr = *desPtr;
		desPtr++;
		srcPtr++; 
	}

	srcPtr = (uint8*)(&dem_FreezeFrame_MEM);
	desPtr = (uint8*)(&dem_FreezeFrame);
	for(index = 0; index<sizeof(dem_FreezeFrame); index++)
	{
		*srcPtr = *desPtr;
		desPtr++;
		srcPtr++; 
	} 

	dem_ErrMemoCheckSum = Dem_FaultMemChksumCal();
}


/*******************************************************************************
 * NAME:             Dem_DtcAssembly
 * CALLED BY:        Dcm
 * PRECONDITIONS:    
 * INPUT PARAMETERS: uint8* dataPtr : pointer to data buffer
 *                   Dem_DtcType type:  type of Dtc Requested
 * RETURN VALUES:    uint8:  data length of the error 
 * DESCRIPTION:      Assembly the Error Information that Dcm Required
 *******************************************************************************/
/*FUNC(uint8,DEM_CODE) Dem_DtcAssembly(uint8* dataPtr,Dem_DtcType type)
{
	uint8 errNum;
	uint8 length;
	uint8 index;
	uint8* tmpDataPtr;

	errNum = 0;
	tmpDataPtr = dataPtr + 1;

	for (index=0; index < dem_MemEntryNextEmpty; index++)
	{
		if (type == ALL_DTC)
		{
			errNum++;
			*(uint16 *)tmpDataPtr = (uint16)(dem_ErrMemoEntry[index].EventId);        
			tmpDataPtr +=2;
			*(uint8 *)tmpDataPtr =  (uint8)(dem_ErrMemoEntry[index].ErrState);
			tmpDataPtr +=1;	    
		}
		else if ((type == MASK_CONFIRMED_DTC)&&(dem_ErrMemoEntry[index].ErrState == DEM_ERRMEM_CONFIRMED))
		{
			errNum++;
			*(uint16 *)tmpDataPtr = (uint16)(dem_ErrMemoEntry[index].EventId);        
			tmpDataPtr +=2;
			*(uint8 *)tmpDataPtr =  (uint8)(dem_ErrMemoEntry[index].ErrState); 
			tmpDataPtr +=1;	  
		}
		else if ((type == MASK_TEST_FAILED)&&(dem_ErrMemoEntry[index].ErrState == DEM_ERRMEM_HEALING))
		{	    
			errNum++;
			*(uint16 *)tmpDataPtr = (uint16)(dem_ErrMemoEntry[index].EventId);
			tmpDataPtr +=2;
			*(uint8 *)tmpDataPtr =  (uint8)(dem_ErrMemoEntry[index].ErrState);
			tmpDataPtr +=1;
		}	  
	}  
	*dataPtr = errNum;
	length = errNum * 3 + 1;
	return (length);  
}*/

/*******************************************************************************
 * NAME:             Dem_FaultMemClearAll
 * CALLED BY:        Dcm
 * PRECONDITIONS:    
 * INPUT PARAMETERS: Void
 * RETURN VALUES:    Void
 * DESCRIPTION:      Clear whole fault memory
 *******************************************************************************/
FUNC(void,DEM_CODE) Dem_FaultMemClearAll(void)
{
	uint8 index;

	for(index = 0; index < DEM_MEM_ENTRY_NUM; index++)
	{
		Dem_FaultMemClearOneEntry(index);  
	}

	for(index = 0; index < DEM_EVENT_NUM; index++ )
	{
		dem_errStatus[index].active = FALSE;
		dem_errStatus[index].mem = FALSE;
	}
	Dem_stClear = TRUE;
	dem_MemEntryNextEmpty = 0;

	//Com_RxTOReset();  
}


/*******************************************************************************
 *  Local Functions Body                                   
 *******************************************************************************/


/*******************************************************************************
 * NAME:             Dem_ErrStatusInit
 * CALLED BY:        Dem
 * PRECONDITIONS:
 * INPUT PARAMETERS: Void
 * RETURN VALUES:    Void
 * DESCRIPTION:      Initialize the Error Status
 *******************************************************************************/
_STATIC_ FUNC(void,DEM_CODE) Dem_ErrStatusInit(void)
{
	uint8 index;
	Dem_EventIdType err_Index;  

	dem_MemEntryNextEmpty = DEM_MEM_ENTRY_NUM;

	for (index = 0;index < DEM_MEM_ENTRY_NUM;index++)
	{
		if (dem_ErrMemoEntry[index].ErrState == DEM_ERRMEM_CONFIRMED)
		{      
			err_Index = dem_ErrMemoEntry[index].EventId;

			dem_errStatus[err_Index].active = DEM_ERR_ACTIVE;
			dem_errStatus[err_Index].mem = TRUE;

			Dem_ErrColUpdate(err_Index,DEM_ERR_ACTIVE);	
			//FiM_DemTriggerOnEventStatus (err_Index,DEM_ERR_ACTIVE);       
		}
		else if ((dem_ErrMemoEntry[index].ErrState) == DEM_ERRMEM_HEALING)
		{      
			err_Index = dem_ErrMemoEntry[index].EventId;

			dem_errStatus[err_Index].active = DEM_ERR_PASSIVE;
			dem_errStatus[err_Index].mem = TRUE;

			Dem_ErrColUpdate(err_Index,DEM_ERR_PASSIVE);	
			//FiM_DemTriggerOnEventStatus (err_Index,DEM_ERR_ACTIVE);       
		}
		else
		{
			dem_MemEntryNextEmpty = index;
			break;
		}
	}    
}

/*******************************************************************************
 * NAME:             Dem_UpdateHealingState
 * CALLED BY:        Dem
 * PRECONDITIONS:
 * INPUT PARAMETERS: Void
 * RETURN VALUES:    Void
 * DESCRIPTION:      Update the Healing Counter and remove the defect if
 *                   Counter become zero.
 *******************************************************************************/
_STATIC_ FUNC(void,DEM_CODE) Dem_UpdateHealingState(void)
{
	uint8 index;

	for (index=0;index<DEM_MEM_ENTRY_NUM;index++)
	{
		if (dem_ErrMemoEntry[index].ErrState==DEM_ERRMEM_HEALING)
		{
			dem_ErrMemoEntry[index].HealingCounter--;
			if(dem_ErrMemoEntry[index].HealingCounter==0)
			{
				if (index < dem_MemEntryNextEmpty-1)
				{
					memcpy(&(dem_ErrMemoEntry[index]),&(dem_ErrMemoEntry[dem_MemEntryNextEmpty-1]) \
							,sizeof(dem_ErrMemoEntry[0]));
				}
				Dem_FaultMemClearOneEntry(dem_MemEntryNextEmpty-1);
				dem_MemEntryNextEmpty--;
			}
		}
	}
}

/*******************************************************************************
 * NAME:             Dem_ErrColUpdate
 * CALLED BY:        Dem
 * PRECONDITIONS:
 * INPUT PARAMETERS: Dem_EventIdType errorIndex
 *                   uint8 errorStatus
 * RETURN VALUES:    Void
 * DESCRIPTION:      Update Error Column
 *******************************************************************************/
_STATIC_ FUNC(void,DEM_CODE) Dem_ErrColUpdate(Dem_EventIdType errorIndex, uint8 errorStatus)
{
	uint8 colIndex;
	uint8 bitPosition;

	colIndex = errorIndex / 16;
	bitPosition = errorIndex % 16;
	if (errorStatus == DEM_ERR_ACTIVE)
	{
		dem_ErrCol_mp[colIndex] |= (uint16)(1<<bitPosition);
	}
	else
	{
		dem_ErrCol_mp[colIndex] &= (~(uint16)(1<<bitPosition));
	}
}

/*******************************************************************************
 * NAME:             Dem_FaultMemChksumCal
 * CALLED BY:        Dem
 * PRECONDITIONS:
 * INPUT PARAMETERS: Void
 * RETURN VALUES:    Dem_ChksumType: Chksum value
 * DESCRIPTION:      Calculate Chksum
 *******************************************************************************/
_STATIC_ FUNC(Dem_ChksumType,DEM_CODE) Dem_FaultMemChksumCal(void)
{
	Dem_ChksumType chksum;
	uint16 index;
	uint16 errMemSize;

	uint8 * dataPtr;


	chksum = 0;
	errMemSize =  sizeof(dem_ErrMemoEntry);
	dataPtr = (uint8 *)&dem_ErrMemoEntry[0]; 

	for (index = 0; index <errMemSize ; index++)
	{
		chksum += *dataPtr;
		dataPtr++;  
	}
	return (chksum);
}

/*******************************************************************************
 * NAME:             Dem_FaultMemClearOneEntry
 * CALLED BY:        Dem
 * PRECONDITIONS:
 * INPUT PARAMETERS: uint8 entryNum
 * RETURN VALUES:    Void
 * DESCRIPTION:      Delete One Error Memory Entry
 *******************************************************************************/   		 	
_STATIC_ FUNC(void,DEM_CODE) Dem_FaultMemClearOneEntry(uint8 entryNum)
{

	dem_ErrMemoEntry[entryNum].EventId = DEM_EVENT_ID_NA;

	dem_ErrMemoEntry[entryNum].ErrClass=0;
	dem_ErrMemoEntry[entryNum].ErrState=0;                          													
	dem_ErrMemoEntry[entryNum].HealingCounter=0;
	dem_ErrMemoEntry[entryNum].ErrOccurCounter=0;                                                            	
} 

/*******************************************************************************
 * NAME:             Dem_ErrMemUpdate
 * CALLED BY:        Dem
 * PRECONDITIONS:
 * INPUT PARAMETERS: Dem_EventIdType EventId
 *                   uint8 EventStatus
 * RETURN VALUES:    Void
 * DESCRIPTION:      Update Error Memory
 *******************************************************************************/ 
_STATIC_ FUNC(void,DEM_CODE) Dem_ErrMemUpdate(Dem_EventIdType EventId,uint8 EventStatus)
{ 
	uint8 index; 

#if (DEM_DEV_ERROR_DETECT == STD_ON)	
	if (EventId >= DEM_EVENT_NUM)
	{
		Det_ReportError(DEM_MODULE_ID,DEM_INSTANCE_ID,DEM_API_ERRMEMUPDATE_ID,DEM_E_INVALID_INDEX);
	}
#endif	

	/* Search for the Error which is already in the Error memory */  
	for (index=0; index < dem_MemEntryNextEmpty; index++)
	{		
		if (dem_ErrMemoEntry[index].EventId == EventId)
		{
			if ((dem_ErrMemoEntry[index].ErrState == DEM_ERRMEM_CONFIRMED) \
					&& (EventStatus == DEM_ERR_PASSIVE))
			{      
				dem_ErrMemoEntry[index].ErrState=DEM_ERRMEM_HEALING;
				if (dem_ErrMemoEntry[index].ErrOccurCounter < 255)//max 255, if 255 ?
				{	
					dem_ErrMemoEntry[index].ErrOccurCounter++;
				}
				dem_ErrMemoEntry[index].HealingCounter=Dem_ErrJ1939FaultCode[EventId].HealingCounter;

			}
			else if ((dem_ErrMemoEntry[index].ErrState==DEM_ERRMEM_HEALING) \
					&& (EventStatus == DEM_ERR_ACTIVE))
			{
				dem_ErrMemoEntry[index].ErrState=DEM_ERRMEM_CONFIRMED;
			}
			/* Found it, jump out of the cycle */
			return;        
		}
	}/* for */

	/* Check if EventStatus is Active */
	if (EventStatus == DEM_ERR_ACTIVE)
	{
		/* Add err in empty Entry */ 
		if (dem_MemEntryNextEmpty < DEM_MEM_ENTRY_NUM)
		{
			Dem_ErrStoreToMem(EventId,dem_MemEntryNextEmpty);      	
			dem_MemEntryNextEmpty++;
			return;
		}

		{          
			uint8 minClass = DEM_MAX_ERR_CLASS;
			uint8 minClass_EntryNum;
			uint8 minClass_ErrIndex;
			/* Look for the minium class entry in the error memory */
			for (index=0;index<DEM_MEM_ENTRY_NUM;index++)
			{
				if (minClass > dem_ErrMemoEntry[index].ErrClass)
				{
					minClass = dem_ErrMemoEntry[index].ErrClass;
					minClass_EntryNum = index;
					minClass_ErrIndex = dem_ErrMemoEntry[index].EventId;
				}        
			} /* for */

			if (Dem_ErrJ1939FaultCode[EventId].ErrorClass > minClass)
			{                  
				Dem_ErrStoreToMem(EventId,minClass_EntryNum);
				dem_errStatus[minClass_ErrIndex].mem = FALSE;
				return;	
			}
		}
	}   	

}

/*******************************************************************************	
 * NAME:             Dem_ErrStoreToMem
 * CALLED BY:        Dem
 * PRECONDITIONS:
 * INPUT PARAMETERS: Dem_EventIdType EventId
 *                   uint8 entryNum
 * RETURN VALUES:    Void
 * DESCRIPTION:      Store one Error To Memory
 *******************************************************************************/ 
_STATIC_ FUNC(void,DEM_CODE) Dem_ErrStoreToMem(Dem_EventIdType EventId,uint8 entryNum)
{
	uint8  index;
	uint8* dataPtr;
	uint8  dataLength;

	dem_errStatus[EventId].mem = TRUE;

	dem_ErrMemoEntry[entryNum].EventId = EventId;
	dem_ErrMemoEntry[entryNum].ErrClass = Dem_ErrJ1939FaultCode[EventId].ErrorClass;
	dem_ErrMemoEntry[entryNum].ErrState = DEM_ERRMEM_CONFIRMED;                          													
	dem_ErrMemoEntry[entryNum].HealingCounter = 0;
	dem_ErrMemoEntry[entryNum].ErrOccurCounter = 0;   

	Dem_ApplFreezeFrameStore(entryNum);                                                         
}




