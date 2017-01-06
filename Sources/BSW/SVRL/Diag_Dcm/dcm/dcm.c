#include "dcm.h"

uint8_t diagSessionMode;

uint8_t ucSeed[4];
uint8_t DCM_m_st_SecurityPassed;
uint8_t DCM_m_st_SeedRequest;
uint8_t DCM_m_ct_SecurityFailCnt;
uint32_t SecurityFailTimeStamp;
uint8_t DCM_m_st_StartRoutine;
uint8_t DCM_m_st_RequestDownload;


uint8 Dem_DTCSetValid;



//void Dcm_SetNegResponse(Dcm_NRCType errorCode)
void Dcm_SetNegResponse(uint8_t errorCode)
{            
  diag_tx_buff[0] = 0x7f;
  diag_tx_buff[1] = diag_rx_buff[0];
  diag_tx_buff[2] = errorCode;
  tx_state.data_length = 0x03;
}

/*如果服务器当前条件可以执行诊断模式切换，只有成功发送肯定响应报文之后(网络层使用 
N_USData.con 向应用层确认 N_Result = N_Success)，服务器才激活所请求的诊断模式，否则诊断 
模式维持不变；如果当前不能执行模式切换，服务器需要发送否定响应报文，并维持原诊断模式不 
变*/
void Dcm_ApplStartDefaultSession(void)
{
	if(DEFAULT_SESSION_MODE==diagSessionMode){
	  //执行完全的初始化，复位所有在defaultSession模式下激活的事件、设置和控制等操作，
	  //但不包括已经固化的操作。
	}
	if(PROGRAMMING_SESSION_MODE==diagSessionMode || EXTENDED_SESSION_MODE==diagSessionMode){
	  //服务器安全状态切换到锁定状态 
	  DCM_m_st_SecurityPassed = 0;//added by xyl 2014.12.8
	  //CommunicationControl (0x28)和ControlDTCSetting(0x85)进行的设置需要被复位
	  //执行完全的初始化，复位所有在非默认模式下激活的事件、设置和控制等操作，
	  //但不包括已经固化的操作
	}
	diag_tx_buff[0] = 0x40+SID_StartDiagnosticSession;
  diag_tx_buff[1] = LID_SDS_DefaultDiagMode;
  diag_tx_buff[2] = 0;
  diag_tx_buff[3] = 20;
  diag_tx_buff[4] = 0;
  diag_tx_buff[5] = 100;
	tx_state.data_length = 0x06;
	diagSessionMode = DEFAULT_SESSION_MODE;
}
void Dcm_ApplStartProgSession(void)
{
	if(DEFAULT_SESSION_MODE==diagSessionMode){
	  //基本不需复位
	}
	if(EXTENDED_SESSION_MODE==diagSessionMode){
	  //服务器安全状态切换到锁定状态 DCM_m_st_SecurityPassed = 0;
	  //CommunicationControl (0x28)和ControlDTCSetting(0x85)进行的设置保持不变
	}
	diag_tx_buff[0] = 0x40+SID_StartDiagnosticSession;
  diag_tx_buff[1] = LID_SDS_ProgrammingMode;
  diag_tx_buff[2] = 0;
  diag_tx_buff[3] = 20;
  diag_tx_buff[4] = 0;
  diag_tx_buff[5] = 100;
	tx_state.data_length = 0x06;
	diagSessionMode = PROGRAMMING_SESSION_MODE;
}
void Dcm_ApplStartExtSession(void)
{
	if(DEFAULT_SESSION_MODE==diagSessionMode){
	  //基本不需复位
	}
	if(PROGRAMMING_SESSION_MODE==diagSessionMode){
	  //服务器安全状态切换到锁定状态 DCM_m_st_SecurityPassed = 0;
	  //CommunicationControl (0x28)和ControlDTCSetting(0x85)进行的设置保持不变
	}
	diag_tx_buff[0] = 0x40+SID_StartDiagnosticSession;
  diag_tx_buff[1] = LID_SDS_ExtendedDiagnosticMode;
  diag_tx_buff[2] = 0;
  diag_tx_buff[3] = 20;
  diag_tx_buff[4] = 0;
  diag_tx_buff[5] = 100;
	tx_state.data_length = 0x06;
	diagSessionMode = EXTENDED_SESSION_MODE;
}
        
        
/*void Dcm_DiagSessionControl(uint8 diagSessionType){  
  switch(diagSessionType)
  {
  	case LID_SDS_DefaultDiagMode:         //0X01
  	{
  		Dcm_ApplStartDefaultSession();
  		break;
  	}
  	case LID_SDS_ProgrammingMode:      	  //0x02
  	{
  		Dcm_ApplStartProgSession();
  		break;
  	}      		
  	case LID_SDS_ExtendedDiagnosticMode:   //0x03
  	{
  		Dcm_ApplStartExtSession();
  		break;
  	}        		
  	default:
  	{
      Dcm_SetNegResponse(NEG_ACK_SUBFUNCTIONNOTSUPPORTED);              
      break;
  	}
  }      	         
} */



void Dcm_ApplEcuHardReset(void)
{
#if SESSION_CHECK_ENABLE > 0
  if(PROGRAMMING_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
  
	if( Appl_EcuHardReset_ConditionCheck() )
	{
		Dcm_SetNegResponse(NEG_ACK_CONDITIONSNOTCORRECT); 
	}
	else
	{
		diag_tx_buff[0] = 0x40+SID_EcuReset;
		diag_tx_buff[1] = LID_HardReset;
		tx_state.data_length = 0x02;
	}
}

void Dcm_ApplEcuSoftReset(void)
{
#if SESSION_CHECK_ENABLE > 0
	if(DEFAULT_SESSION_MODE != diagSessionMode && EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
  
  if( Appl_EcuSoftResetCheck() )
	{
		Dcm_SetNegResponse(NEG_ACK_CONDITIONSNOTCORRECT);
	}
	else
	{
		diag_tx_buff[0] = 0x40+SID_EcuReset;
		diag_tx_buff[1] = LID_SoftReset;
		tx_state.data_length = 0x02;
	}
}


void Dcm_ApplClearFaultMemory(void)
{
  uint16 Id ;

#if SESSION_CHECK_ENABLE > 0
	if(DEFAULT_SESSION_MODE != diagSessionMode && EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
	Id = (*(uint16*)(&diag_rx_buff[1]));
  if(Id == 0xFFFF)
  {
  	if(Appl_ClearFaultMemory_ConditionCheck()){//条件不满足
  	  Dcm_SetNegResponse(NEG_ACK_CONDITIONSNOTCORRECT);
  	}
  	else{
    	tx_state.data_length = 0x01;
    	diag_tx_buff[0] = 0x40+SID_ClearFaultMemory;
      Dem_FaultMemClearAll();  
    }
  }
  else
  {
    Dcm_SetNegResponse(NEG_ACK_REQUESTOUTOFRANGE);
  }
}


void Dcm_ApplRequestSeed(void)
{
  uint32_t tmp;
  uint32_t TimePassed;

#if SESSION_CHECK_ENABLE > 0
  if(EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(0x7F); 
     return;
  }
#endif
  if(DCM_m_ct_SecurityFailCnt>0)
  {
    TimePassed = read_tbl() - SecurityFailTimeStamp;
    if(TimePassed<10000)
    {
      Dcm_SetNegResponse(NEG_ACK_REQUIREDTIMEDELAYNOTEXPIRED);
      return;
    }
    DCM_m_ct_SecurityFailCnt = 0;
    SecurityFailTimeStamp = read_tbl();
  }
  
  tmp = read_tbl();
  ucSeed[0] = (uint8_t)tmp;
  if( ucSeed[0] == 0x00 )
      ucSeed[0] = 0xea;
  ucSeed[1] = (uint8_t) (tmp>>8);
  
	diag_tx_buff[0] = 0x40+SID_SecurityAccess;
	diag_tx_buff[1] = 0x01;
	diag_tx_buff[2] = ucSeed[0];
	diag_tx_buff[3] = ucSeed[1];
	tx_state.data_length = 4;
  DCM_m_st_SeedRequest = 1;    
}

//extern uint16_t generated_key;
void Dcm_ApplSendKey(uint16_t key)
{
	uint16_t seed;

#if SESSION_CHECK_ENABLE > 0
  if(EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(0x7F); 
     return;
  }
#endif
  seed = (uint16_t)ucSeed[0];
	seed <<= 8;
	seed += ucSeed[1];

  if(!DCM_m_st_SeedRequest)
  {
    Dcm_SetNegResponse(NEG_ACK_REQUESTSEQUENCEERROR);
    return;  
  }
	
	if( (DCM_m_ct_SecurityFailCnt<3) && (Appl_SeedKeyMatched(seed, key)) )
	{
		DCM_m_ct_SecurityFailCnt = 0;
		DCM_m_st_SecurityPassed = 1;
  	diag_tx_buff[0] = 0x40+SID_SecurityAccess;
  	diag_tx_buff[1] = 0x02;
  	//*(uint16*)(diag_tx_buff[2]) = generated_key;
  	tx_state.data_length = 2; 
	}
	else 
	{
		DCM_m_ct_SecurityFailCnt += 1;
		DCM_m_st_SecurityPassed = 0;
		SecurityFailTimeStamp = read_tbl();	  
	  if(DCM_m_ct_SecurityFailCnt>=3)	{
	    Dcm_SetNegResponse(NEG_ACK_EXCEEDNUMOFATTEMPTS);
	  }
	  else{
	    Dcm_SetNegResponse(NEG_ACK_INVALIDKEY);
	  }
	}
	
	DCM_m_st_SeedRequest = 0;	
}


/*void Dcm_ApplReadActiveDTC(void)
{
  diag_tx_buff[0] = 0x40+SID_ReadDTC;
	tx_state.data_length = 0x01;
}
void Dcm_ApplReadInactiveDTC(void)
{
  diag_tx_buff[0] = 0x40+SID_ReadDTC;
	tx_state.data_length = 0x01;
}
void Dcm_ApplReadAllDTC(void)
{
  diag_tx_buff[0] = 0x40+SID_ReadDTC;
	tx_state.data_length = 0x01;
}*/



void Dcm_ApplReadECUId(uint16_t id_option)
{
	//uint16_t index;
	
	switch(id_option)
	{/*
		case DCM_READ_ECU_ID_SYS_HARDWARE:
		{
		  	diag_tx_buff[0] = 0x40+SID_ReadECUId;
		  	diag_tx_buff[1] = id_option;
		  	for (index =0;index<sizeof(Dcm_SysHardwareID);index++)
		  	{
		    	diag_tx_buff[index+2] = Dcm_SysHardwareID[index];   
		  	}
		  	tx_state.data_length = sizeof(Dcm_SysHardwareID) + 2; 
		}
		break;

		case DCM_READ_ECU_ID_APPSW_VERSION:
		{
		  	diag_tx_buff[0] = 0x40+SID_ReadECUId;
		  	diag_tx_buff[1] = id_option;
		  	for (index =0;index<sizeof(Dcm_AppSwVersion);index++)
		  	{
		    	diag_tx_buff[index+2] = Dcm_AppSwVersion[index];   
		  	}
		  	tx_state.data_length = sizeof(Dcm_AppSwVersion) + 2; 
		}
		break;

		case DCM_READ_ECU_ID_BOOTSW_VERSION:
		{
		  	diag_tx_buff[0] = 0x40+SID_ReadECUId;
		  	diag_tx_buff[1] = id_option;
		  	for (index =0;index<sizeof(Dcm_BootSwVersion);index++)
		  	{
		    	diag_tx_buff[index+2] = Dcm_BootSwVersion[index];   
		  	}
		  	tx_state.data_length = sizeof(Dcm_BootSwVersion) + 2; 
		}
		break;     

		case DCM_READ_ECU_ID_BSW_VERSION:
		{
		  	diag_tx_buff[0] = 0x40+SID_ReadECUId;
		  	diag_tx_buff[1] = id_option;
		  	for (index =0;index<sizeof(Dcm_BSWVersion);index++)
		  	{
		    	diag_tx_buff[index+2] = Dcm_BSWVersion[index];   
		  	}
		  	tx_state.data_length = sizeof(Dcm_BSWVersion) + 2; 
		}
		break; 

		case DCM_READ_ECU_ID_SW_RELEASE_DATE:
		{
		  	diag_tx_buff[0] = 0x40+SID_ReadECUId;
		  	diag_tx_buff[1] = id_option;
		  	for (index =0;index<sizeof(Dcm_SwReleaseDate);index++)
		  	{
		    	diag_tx_buff[index+2] = Dcm_SwReleaseDate[index];   
		  	}
		  	tx_state.data_length = sizeof(Dcm_SwReleaseDate) + 2; 
		}
		break;*/     
		    
		default:
		{
      Dcm_SetNegResponse(NEG_ACK_REQUESTOUTOFRANGE); 
  	}
		break;     
	}
}


void Dcm_ApplReportNumberOfDTCByStatusMask(Dem_DTCStatusMaskType DTCStsMask)//19 01 00(01/02) request length = 3
{  
#if SESSION_CHECK_ENABLE > 0
	if(DEFAULT_SESSION_MODE != diagSessionMode && EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
	diag_tx_buff[0] = 0x40+SID_ReadDTCInfo;
  diag_tx_buff[1] = LID_ReportNumOfDTC;
  diag_tx_buff[2] = diag_rx_buff[2];
  diag_tx_buff[3] = DTCFormat_ISO14229;
  tx_state.data_length = 0x04+Dem_GetNumberOfDTCByStatusMask(diag_tx_buff+4,DTCStsMask);//Dem_GetNumberOfDTCByStatusMask()
}
void Dcm_ApplReportDTCByStatusMask(Dem_DTCStatusMaskType DTCStsMask)//19 01 00(01/02)  request length = 3
{
#if SESSION_CHECK_ENABLE > 0
	if(DEFAULT_SESSION_MODE != diagSessionMode && EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
	diag_tx_buff[0] = 0x40+SID_ReadDTCInfo;
  diag_tx_buff[1] = LID_ReportDTCByStatusMask;
  diag_tx_buff[2] = diag_rx_buff[2];
  tx_state.data_length = 0x03 + Dem_GetDTCByStatusMask(diag_tx_buff+3,DTCStsMask);//Dem_GetDTCByStatusMask()
}
void Dcm_ApplReportSupportedDTC(void)//19 0A   request length = 2
{
#if SESSION_CHECK_ENABLE > 0
	if(    DEFAULT_SESSION_MODE != diagSessionMode 
	    && PROGRAMMING_SESSION_MODE != diagSessionMode
	    && EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
	diag_tx_buff[0] = 0x40+SID_ReadDTCInfo;
  diag_tx_buff[1] = LID_ReadSupportedDTC;
  tx_state.data_length = 0x02 + Dem_GetSupportedDTC(diag_tx_buff+2);//DTC_STATUS_TESTFAILED | DTC_STATUS_CONFIRMED;
}



void 
Dcm_ApplReadDataByLocalId(uint16_t lid)
{
	uint8_t index;

#if SESSION_CHECK_ENABLE > 0
	if(    DEFAULT_SESSION_MODE != diagSessionMode 
	    && PROGRAMMING_SESSION_MODE != diagSessionMode
	    && EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
  if(Appl_ReadDataConditionCheck())
  {
  	Dcm_SetNegResponse(NEG_ACK_CONDITIONSNOTCORRECT);
		return;
	}
	
	switch(lid)
	{
		case DCM_PRODUCT_PART_NUM:
		{
	  	diag_tx_buff[0] = 0x40+SID_ReadDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid;         
	  	for (index =0;index<LEN_PRODUCT_PART_NUM;index++)
	  	{
	    	diag_tx_buff[index+3] = Dcm_ProductPartNumber[index];   
	  	}
	  	tx_state.data_length = LEN_PRODUCT_PART_NUM + 3; 
		}
		break;

		case DCM_SYS_NAME:
		{
	  	diag_tx_buff[0] = 0x40+SID_ReadDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid;         
	  	for (index =0;index<LEN_SYS_NAME;index++)
	  	{
	    	diag_tx_buff[index+3] = Dcm_SystemName[index];   
	  	}
	  	tx_state.data_length = LEN_SYS_NAME + 3; 
		}
		break;
		
		case DCM_SUPPLIER_ID:
		{
	  	diag_tx_buff[0] = 0x40+SID_ReadDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid;         
	  	for (index =0;index<LEN_SUPPLYER_ID;index++)
	  	{
	    	diag_tx_buff[index+3] = SupplierIdentifier[index];   
	  	}
	  	tx_state.data_length = LEN_SUPPLYER_ID + 3; 
		}
		break;
		
		case DCM_ECUSW_VERSION:
		{
	  	diag_tx_buff[0] = 0x40+SID_ReadDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid;         
	  	for (index =0;index<LEN_ECU_SW_VER;index++)
	  	{
	    	diag_tx_buff[index+3] = Dcm_ECUSoftwareVersionNumber[index];   
	  	}
	  	tx_state.data_length = LEN_ECU_SW_VER + 3; 
		}
		break;
		
		case DCM_ECUHW_VERSION:
		{
	  	diag_tx_buff[0] = 0x40+SID_ReadDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid;         
	  	for (index =0;index<LEN_ECU_HW_VER;index++)
	  	{
	    	diag_tx_buff[index+3] = Dcm_ECUHardtwareVersionNumber[index];   
	  	}
	  	tx_state.data_length = LEN_ECU_HW_VER + 3; 
		}
		break;
		
		case DCM_SW_CHECK:
		{
	  	diag_tx_buff[0] = 0x40+SID_ReadDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid;         
	  	for (index =0;index<LEN_SW_CHECK;index++)
	  	{
	    	diag_tx_buff[index+3] = Dcm_SoftwareCheck[index];   
	  	}
	  	tx_state.data_length = LEN_SW_CHECK + 3; 
		}
		break;
		
		case DCM_SW_PROGRAMMING_DATE:
		{
	  	diag_tx_buff[0] = 0x40+SID_ReadDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid;         
	  	for (index =0;index<LEN_PROG_DATE;index++)
	  	{
	    	diag_tx_buff[index+3] = Dcm_ProgrammingDate[index];   
	  	}
	  	tx_state.data_length = LEN_PROG_DATE + 3; 
		}
		break;
		
		case DCM_REPAIR_INFO:
		{
		  uint8 result;
	  	diag_tx_buff[0] = 0x40+SID_ReadDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid;         
	  	/*for (index =0;index<sizeof(Dcm_RepairInformation);index++)
	  	{
	    	diag_tx_buff[index+3] = Dcm_RepairInformation[index];   
	  	}*/
	  	result = Appl_ReadRepairInfo((uint8*)(diag_tx_buff+3));//use callback function to instead 
	  	if(result != 0){
	  	  Dcm_SetNegResponse(NEG_ACK_GENERALPROGRAMMINGFAILURE);
		    return;
	  	}
	  	tx_state.data_length = LEN_REPAIR_INFO + 3; 
		}
		break;
		
		case DCM_ECU_SN:
		{
	  	uint8 result;
	  	diag_tx_buff[0] = 0x40+SID_ReadDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid;         
	  	/*for (index =0;index<LEN_ECU_SN;index++)
	  	{
	    	diag_tx_buff[index+3] = Dcm_ECUSerialNumber[index];   
	  	}*/
	  	result = Appl_ReadECUSeriNum((uint8*)(diag_tx_buff+3));//use callback function to instead 
	  	if(result != 0){
	  	  Dcm_SetNegResponse(NEG_ACK_GENERALPROGRAMMINGFAILURE);
		    return;
	  	}
	  	tx_state.data_length = LEN_ECU_SN + 3; 
		}
		break;
		
		case DCM_CALIBRA_SW_VERSION:
		{
	  	diag_tx_buff[0] = 0x40+SID_ReadDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid;         
	  	for (index =0;index<LEN_CALIBRA_SW_VER;index++)
	  	{
	    	diag_tx_buff[index+3] = Dcm_CalibrationSoftwareVersion[index];   
	  	}
	  	tx_state.data_length = LEN_CALIBRA_SW_VER + 3; 
		}
		break;
		
		case DCM_HW_VERSION:
		{
	  	diag_tx_buff[0] = 0x40+SID_ReadDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid;         
	  	for (index =0;index<LEN_HW_VER;index++)
	  	{
	    	diag_tx_buff[index+3] = Dcm_HardwareVersion[index];   
	  	}
	  	tx_state.data_length = LEN_HW_VER + 3; 
		}
		break;
		
		case DCM_SW_VERSION:
		{
	  	diag_tx_buff[0] = 0x40+SID_ReadDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid;         
	  	for (index =0;index<LEN_SW_VER;index++)
	  	{
	    	diag_tx_buff[index+3] = Dcm_SoftwareVersion[index];   
	  	}
	  	tx_state.data_length = LEN_SW_VER + 3; 
		}
		break;
		

		case DCM_LEFT_DRL_INFO:		
			diag_tx_buff[0] = 0x40+SID_ReadDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid;         
			diag_tx_buff[3] = (uint8_t)(leftDRLInfo); //variable interface 
			tx_state.data_length = 4; 
			break;
		case DCM_RIGHT_DRL_INFO:		
			diag_tx_buff[0] = 0x40+SID_ReadDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid;         
			diag_tx_buff[3] = (uint8_t)(rightDRLInfo);//variable interface 
			tx_state.data_length = 4; 
			break;
		case DCM_LEFT_POSITION_LAMP_INFO:		
			diag_tx_buff[0] = 0x40+SID_ReadDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid;         
			diag_tx_buff[3] = (uint8_t)(leftPosLampInfo);//variable interface 
			tx_state.data_length = 4; 
			break;
		case DCM_RIGHT_POSITION_LAMP_INFO:		
			diag_tx_buff[0] = 0x40+SID_ReadDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid;         
			diag_tx_buff[3] = (uint8_t)(rightPosLampInfo);//variable interface 
			tx_state.data_length = 4; 
			break;			
		
		default:
      Dcm_SetNegResponse(NEG_ACK_REQUESTOUTOFRANGE);
			break;
	}
}



void 
Dcm_ApplWriteDataByLocalId(uint16_t lid)
{
	uint8 index;
	uint8 result;

#if SESSION_CHECK_ENABLE > 0
	if(PROGRAMMING_SESSION_MODE != diagSessionMode
	    //&& EXTENDED_SESSION_MODE != diagSessionMode
	    ){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
  if(Appl_WriteDataConditionCheck())
  {
  	Dcm_SetNegResponse(NEG_ACK_CONDITIONSNOTCORRECT);
		return;
	}
	
	switch(lid)
	{
		case DCM_REPAIR_INFO:
		{        
	  	if( rx_state.data_length != (LEN_REPAIR_INFO+3))
  		{
        Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
    		return;
  		}
	  	/*for (index=0; index<sizeof(Dcm_RepairInformation); index++)
	  	{
	    	Dcm_RepairInformation[index] = diag_rx_buff[index+3];   
	  	}*/
	  	result = Appl_WriteRepairInfo((uint8*)(diag_rx_buff+3));//use callback function to instead 
	  	if(result != 0){
	  	  Dcm_SetNegResponse(NEG_ACK_GENERALPROGRAMMINGFAILURE);
		    return;
	  	}
	    diag_tx_buff[0] = 0x40+SID_WriteDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid; 
			tx_state.data_length = 3; 
		}
		break;
		
		case DCM_ECU_SN:
		{        
	  	if( rx_state.data_length != (LEN_ECU_SN+3))
  		{
        Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
    		return;
  		}
	  	result = Appl_WriteECUSeriNum((uint8*)(diag_rx_buff+3));//use callback function to instead 
	  	if(result != 0){
	  	  Dcm_SetNegResponse(NEG_ACK_GENERALPROGRAMMINGFAILURE);
		    return;
	  	}
	    diag_tx_buff[0] = 0x40+SID_WriteDataByLocalId;
			diag_tx_buff[1] = (uint8)(lid>>8);
			diag_tx_buff[2] = (uint8)lid; 
			tx_state.data_length = 3; 
		}
		break;

		default:
		{
      Dcm_SetNegResponse(NEG_ACK_REQUESTOUTOFRANGE);
			break;  
	  }
	}
}



void Dcm_ApplIOCById( vuint16_t dataId, vuint8_t par_size, vuint8_t *par )
{
	uint8_t exitStatus;

	switch( diag_rx_buff[3] )
	{
		case PAR_IOC_ReturnControlToECU:         //0X00
		{
#if SESSION_CHECK_ENABLE > 0
	if(EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
			if(0xFF == Dcm_ApplIOCReturnControlToECU( dataId, par_size, par, &exitStatus ) )
    	{
    	  Dcm_SetNegResponse(NEG_ACK_REQUESTOUTOFRANGE);    	
    	}
    	else if(1 == Dcm_ApplIOCReturnControlToECU( dataId, par_size, par, &exitStatus ) )
    	{
    	  Dcm_SetNegResponse(NEG_ACK_CONDITIONSNOTCORRECT);    	
    	}
	    else
    	{
    	  	diag_tx_buff[0] = 0x40 + SID_InputOutputControlByIdentifier; 
    	  	diag_tx_buff[1] = (uint8_t)(dataId>>8);
    	  	diag_tx_buff[2] = (uint8_t)(dataId);
    	  	diag_tx_buff[3] = exitStatus;
    	  	tx_state.data_length = 4;
    	}	
			break;
		}
		case PAR_IOC_ResetToDefault:      	  //0x01
		{
			if(0xFF == Dcm_ApplIOCResetToDefault( dataId, par_size, par, &exitStatus ) )
    	{
    	  Dcm_SetNegResponse(NEG_ACK_REQUESTOUTOFRANGE);    	
    	}
    	else if(1 == Dcm_ApplIOCResetToDefault( dataId, par_size, par, &exitStatus ) )
    	{
    	  Dcm_SetNegResponse(NEG_ACK_CONDITIONSNOTCORRECT);    	
    	}
	    else
    	{
  	  	diag_tx_buff[0] = 0x40 + SID_InputOutputControlByIdentifier; 
  	  	diag_tx_buff[1] = (uint8_t)(dataId>>8);
  	  	diag_tx_buff[2] = (uint8_t)(dataId);
  	  	diag_tx_buff[3] = exitStatus;
  	  	tx_state.data_length = 4;
    	}	
			break;
		}       		
		case PAR_IOC_ShortTermAdustment:   //0x03
		{
#if SESSION_CHECK_ENABLE > 0
	if(EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
			if(0xFF == Dcm_ApplIOCShortTermAdustment( dataId, par_size, par, &exitStatus ) )
    	{
    	  Dcm_SetNegResponse(NEG_ACK_REQUESTOUTOFRANGE);    	
    	}
    	else if(1 == Dcm_ApplIOCShortTermAdustment( dataId, par_size, par, &exitStatus ) )
    	{
    	  Dcm_SetNegResponse(NEG_ACK_CONDITIONSNOTCORRECT);    	
    	}
	    else
    	{
  	  	diag_tx_buff[0] = 0x40 + SID_InputOutputControlByIdentifier; 
  	  	diag_tx_buff[1] = (uint8_t)(dataId>>8);
  	  	diag_tx_buff[2] = (uint8_t)(dataId);
  	  	diag_tx_buff[3] = exitStatus;
  	  	tx_state.data_length = 4;
    	}	
			break;
		}        		
		default:
		{
      Dcm_SetNegResponse(NEG_ACK_SUBFUNCTIONNOTSUPPORTED);
      break;
		}
	}
}

void Dcm_DcmTesterPresent(void)
{
#if SESSION_CHECK_ENABLE > 0
	if(DEFAULT_SESSION_MODE != diagSessionMode
	    && PROGRAMMING_SESSION_MODE != diagSessionMode
	    && EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
 	diag_tx_buff[0] = 0x40 + SID_TesterPresent; 
	diag_tx_buff[1] = LID_TP_ZEROSUBFUNCTION;
  tx_state.data_length = 2;
}

//停止或恢复诊断故障码的设置
//故障码被停止的情况下，当发生以下事件时，ECU恢复故障码的设置
//1.ECU收到ControlDTCSetting,其DTCSettingType为switch on
//2.ECU复位或断电
//3.诊断会话模式切换到默认会话模式
//另：该服务不影响0x19和0x14复位的执行
void 
Dcm_ApplControlDTCSettingOn(void)   //0x85   01
{
#if SESSION_CHECK_ENABLE > 0
	if(EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
  if(Appl_ControlDTCSetting_ConditionCheck()){
  	Dcm_SetNegResponse(NEG_ACK_CONDITIONSNOTCORRECT);  
  	return;
	}
	Dem_DTCSetValid = TRUE ;    // Turn on DTC setting	
  diag_tx_buff[0] = 0x40+SID_ControlDTCSetting;
  diag_tx_buff[1] = LID_CDTCS_ON;
	tx_state.data_length = 0x02;
}


  
void 
Dcm_ApplControlDTCSettingOff(void)   //0x85   02
{
#if SESSION_CHECK_ENABLE > 0
	if(EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
	if(Appl_ControlDTCSetting_ConditionCheck()){
  	Dcm_SetNegResponse(NEG_ACK_CONDITIONSNOTCORRECT);  
  	return;
	}	
	Dem_DTCSetValid = FALSE ;   // Turn off DTC setting
  diag_tx_buff[0] = 0x40+SID_ControlDTCSetting;
  diag_tx_buff[1] = LID_CDTCS_OFF;
  tx_state.data_length = 0x02;
}


void Dcm_ApplEnableRxAndTx(uint8 communicationType)//0x28   00
{
#if SESSION_CHECK_ENABLE > 0
	if(EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
  switch(communicationType){
    case NCM:{//常规应用报文
        diag_tx_buff[0] = 0x40+SID_CommunicationControlService;
        diag_tx_buff[1] = LID_CCS_EnableRxAndTx;
        tx_state.data_length = 0x02;
        Appl_EnableRxAndTx_NCM();
        break;
    }
    case NWMCM:{//网络管理报文
        diag_tx_buff[0] = 0x40+SID_CommunicationControlService;
        diag_tx_buff[1] = LID_CCS_EnableRxAndTx;
        tx_state.data_length = 0x02;
        Appl_EnableRxAndTx_NWMCM();
        break;
    }
    case NCM_NWMCM:{//both
        diag_tx_buff[0] = 0x40+SID_CommunicationControlService;
        diag_tx_buff[1] = LID_CCS_EnableRxAndTx;
        tx_state.data_length = 0x02;
        Appl_EnableRxAndTx_NCM_NWMCM();
        break;
    }
    default:{
      Dcm_SetNegResponse(NEG_ACK_REQUESTOUTOFRANGE);
      break;
    }
  }
}

void Dcm_ApplDisableRxAndTx(uint8 communicationType)//0x28   03
{
#if SESSION_CHECK_ENABLE > 0
	if(EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
  switch(communicationType){
    case NCM:{//常规应用报文
        diag_tx_buff[0] = 0x40+SID_CommunicationControlService;
        diag_tx_buff[1] = LID_CCS_DisableRxAndTx;
        tx_state.data_length = 0x02;
        Appl_DisableRxAndTx_NCM();
        break;
    }
    case NWMCM:{//网络管理报文
        diag_tx_buff[0] = 0x40+SID_CommunicationControlService;
        diag_tx_buff[1] = LID_CCS_DisableRxAndTx;
        tx_state.data_length = 0x02;
        Appl_DisableRxAndTx_NWMCM();
        break;
    }
    case NCM_NWMCM:{//both
        diag_tx_buff[0] = 0x40+SID_CommunicationControlService;
        diag_tx_buff[1] = LID_CCS_DisableRxAndTx;
        tx_state.data_length = 0x02;
        Appl_DisableRxAndTx_NCM_NWMCM();
        break;
    }
    default:{
      Dcm_SetNegResponse(NEG_ACK_REQUESTOUTOFRANGE);
      break;
    }
  }
}

/*void Dcm_ApplEnableRxAndTx(void)//0x28   00
{
  
  diag_tx_buff[0] = 0x40+SID_CommunicationControlService;
  diag_tx_buff[1] = LID_CCS_EnableRxAndTx;
  tx_state.data_length = 0x02;
  Appl_EnableRxAndTx();
}
void Dcm_ApplDisableRxAndTx(void)//0x28   03
{
  diag_tx_buff[0] = 0x40+SID_CommunicationControlService;
  diag_tx_buff[1] = LID_CCS_DisableRxAndTx;
  tx_state.data_length = 0x02;
  Appl_DisableRxAndTx();
}*/



void 
Dcm_ApplStartRoutineByLocalId( uint16_t routineId, uint8_t par_size, uint8_t *par )
{
	uint8_t exitStatus;

#if SESSION_CHECK_ENABLE > 0
	if(PROGRAMMING_SESSION_MODE != diagSessionMode
	    &&EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
 	if(!DCM_m_st_SecurityPassed)
	{
  	Dcm_SetNegResponse(NEG_ACK_ACCESSDENIED);  
  	return;
	}

	if( Appl_StartRoutineByLocalId(routineId, par_size, par, &exitStatus) )
	{
	  Dcm_SetNegResponse(NEG_ACK_CONDITIONSNOTCORRECT);
	}
	else
	{
  	diag_tx_buff[0] = 0x40+SID_RoutineControl;
  	diag_tx_buff[1] = LID_RC_StartRoutine; 
  	*(uint16*)(&diag_tx_buff[2]) = routineId;
  	diag_tx_buff[4] = exitStatus;
  	tx_state.data_length = 5;
  	DCM_m_st_StartRoutine = 1;
	}	
}


void 
Dcm_ApplStopRoutineByLocalId( uint16_t routineId, uint8_t par_size, uint8_t *par )
{
	uint8_t exitStatus;
	
#if SESSION_CHECK_ENABLE > 0
	if(PROGRAMMING_SESSION_MODE != diagSessionMode
	    &&EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
 	if(!DCM_m_st_SecurityPassed)
	{
  	Dcm_SetNegResponse(NEG_ACK_ACCESSDENIED);  
  	return;
	}                         
	if(!DCM_m_st_StartRoutine){
  	Dcm_SetNegResponse(NEG_ACK_REQUESTSEQUENCEERROR);
		return;	
	}
	else{
	  //Service StartRoutine is active
	}
	if( Appl_StopRoutineByLocalId(routineId, par_size, par, &exitStatus) )
	{
	  Dcm_SetNegResponse(NEG_ACK_CONDITIONSNOTCORRECT);
	}
	else
	{
  	diag_tx_buff[0] = 0x40+SID_RoutineControl;
  	diag_tx_buff[1] = LID_RC_StopRoutine; 
  	*(uint16*)(&diag_tx_buff[2]) = routineId;
  	diag_tx_buff[4] = exitStatus;
  	tx_state.data_length = 5;
	}
}



void Dcm_ApplReqRoutineResultByLocalId(uint16_t routineId)
{
	uint8_t exitStatus;
	
#if SESSION_CHECK_ENABLE > 0
	if(PROGRAMMING_SESSION_MODE != diagSessionMode
	    &&EXTENDED_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
 	if(!DCM_m_st_SecurityPassed)
	{
  	Dcm_SetNegResponse(NEG_ACK_ACCESSDENIED);  
  	return;
	}
	if(!DCM_m_st_StartRoutine){
  	Dcm_SetNegResponse(NEG_ACK_REQUESTSEQUENCEERROR);
		return;	
	}
	else{
	  //Service StartRoutine is active
	}
	if( Appl_ReqRoutineResultByLocalId(routineId, &exitStatus) )
	{
	  Dcm_SetNegResponse(NEG_ACK_CONDITIONSNOTCORRECT);
	}
	else
	{
  	diag_tx_buff[0] = 0x40+SID_RoutineControl;
  	diag_tx_buff[1] = LID_RC_ReqRoutineResult; 
  	*(uint16*)(&diag_tx_buff[2]) = routineId;
  	diag_tx_buff[4] = exitStatus;
  	tx_state.data_length = 5;
	}
}



        
void Dcm_ReadMemoryByAddress(uint32_t address, uint8_t size)
{
	
}

#define MAX_NUM_OF_BLOCKLENGTH_PAR_LENGTH 1

uint8 maxNumberOfBlockLength = 0xF0;//sample

void Dcm_ApplRequestDownload(void)//0x34 EQC-773 P64
{
#if SESSION_CHECK_ENABLE > 0
	if(PROGRAMMING_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
  if(!DCM_m_st_SecurityPassed)
	{
  	Dcm_SetNegResponse(NEG_ACK_ACCESSDENIED);
		return;
	}
	if(Appl_RequestDownload_ConditionCheck())
	{
	  Dcm_SetNegResponse(NEG_ACK_CONDITIONSNOTCORRECT);
	  return;
	}
	

	memAddress = (uint32)(((uint16)(diag_rx_buff[3]<<8) + diag_rx_buff[4])<<16)
              +(uint16)(diag_rx_buff[5]<<8) + diag_rx_buff[6];
  memSize = (uint32)(((uint16)(diag_rx_buff[7]<<8) + diag_rx_buff[8])<<16)
              +(uint16)(diag_rx_buff[9]<<8) + diag_rx_buff[10];
	//requestOutOfRange
	/*if((0!=diag_rx_buff[1])//dataFormatIdentifier 
	    || (0x44 != diag_rx_buff[2])//addressAndLengthFormatIdentifier
	    || (memAddress memSize RangeCheck)){
	    Dcm_SetNegResponse(NEG_ACK_REQUESTOUTOFRANGE);
	    return;
	 }*/
	              
  diag_tx_buff[0] = 0x40+SID_RequestDownload;
  diag_tx_buff[1] = MAX_NUM_OF_BLOCKLENGTH_PAR_LENGTH;//EQC-773 P64
  diag_tx_buff[1] = (uint8)(diag_tx_buff[1]<<4);
  diag_tx_buff[2] = maxNumberOfBlockLength;
  tx_state.data_length = 0x02+MAX_NUM_OF_BLOCKLENGTH_PAR_LENGTH;
  
  DCM_m_st_RequestDownload = 1;
}



void Dcm_ApplTransferData(uint8 blcokSeqCnt, uint8 reqParRecordLen, uint8 *reqParRecord)//0x36
{
#if SESSION_CHECK_ENABLE > 0
	if(PROGRAMMING_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
   if(!DCM_m_st_SecurityPassed)
	{
  	Dcm_SetNegResponse(NEG_ACK_ACCESSDENIED);
		return;
	}
	if(!DCM_m_st_RequestDownload){
  	Dcm_SetNegResponse(NEG_ACK_REQUESTSEQUENCEERROR);
		return;	
	}
	else{
	  //Service RequestDownload is active
	  //WriteMemoryData(reqParRecordLen,reqParRecord);//用到memAddress
	  //memAddress += reqParRecordLen;
	  //memSize -= reqParRecordLen;
	}
	diag_tx_buff[0] = 0x40+SID_TransferData;
  diag_tx_buff[1] = blcokSeqCnt;
  tx_state.data_length = 2+Appl_FillTransRespParRecord(reqParRecordLen, reqParRecord);
}



void Dcm_ApplRequestTransferExit(uint8 reqParRecordLen, uint8 *reqParRecord)//0x37
{  
#if SESSION_CHECK_ENABLE > 0
	if(PROGRAMMING_SESSION_MODE != diagSessionMode){
     Dcm_SetNegResponse(NEG_ACK_GENERALREJECT); 
     return;
  }
#endif
  if(!DCM_m_st_SecurityPassed)
	{
  	Dcm_SetNegResponse(NEG_ACK_ACCESSDENIED);
		return;
	}
	if(Appl_RequestTransferExit_ConditionCheck())
	{
	  Dcm_SetNegResponse(NEG_ACK_CONDITIONSNOTCORRECT);
	  return;
	}
	if(!DCM_m_st_RequestDownload){
  	Dcm_SetNegResponse(NEG_ACK_REQUESTSEQUENCEERROR);
		return;	
	}
	else{
	  DCM_m_st_RequestDownload = 0;//
	}
	diag_tx_buff[0] = 0x40+SID_RequestTransferExit;
  tx_state.data_length = 1+Appl_FillTransExitRespParRecord(reqParRecordLen, reqParRecord);
}


