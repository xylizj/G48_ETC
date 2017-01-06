#include "dcm_cbk.h"
#include "dcm.h"


#define CONDITION_NOT_CORRECT 0



const uint8_t Dcm_ProductPartNumber[LEN_PRODUCT_PART_NUM];                          
const uint8_t Dcm_SystemName[LEN_SYS_NAME];                          
const uint8_t SupplierIdentifier[LEN_SUPPLYER_ID];                          
const uint8_t Dcm_ECUSoftwareVersionNumber[LEN_ECU_SW_VER];                          
const uint8_t Dcm_ECUHardtwareVersionNumber[LEN_ECU_HW_VER];                          
const uint8_t Dcm_SoftwareCheck[LEN_SW_CHECK];  
const uint8_t Dcm_ProgrammingDate[LEN_PROG_DATE];  
uint8_t Dcm_RepairInformation[LEN_REPAIR_INFO];//May be modified by Service WriteDataByLocalId 
uint8_t Dcm_ECUSerialNumber[LEN_ECU_SN];
const uint8_t Dcm_CalibrationSoftwareVersion[LEN_CALIBRA_SW_VER];  
const uint8_t Dcm_HardwareVersion[LEN_HW_VER];
const uint8_t Dcm_SoftwareVersion[LEN_SW_VER]; 




uint8_t leftDRLInfo;
uint8_t rightDRLInfo;
uint8_t leftPosLampInfo;
uint8_t rightPosLampInfo;
uint32_t memSize;
uint32_t memAddress;

uint8_t leftDRLControl;
uint8_t rightDRLControl;
uint8_t leftPosLampControl;
uint8_t rightPosLampControl;



uint8_t TEST_m_st_CurrentMode = 0;
uint8_t TEST_m_st_Request = 0;
uint8_t TEST_m_st_ShutoffDieselInj = 0;
uint8_t TEST_m_st_OpenLngValve = 0;
uint8_t TEST_m_st_ShutoffRequest = 0;
uint32_t TEST_m_ti_LngInjTime = 0;
uint32_t TEST_m_ti_NGInjVlvTest = 0; 

uint8_t EcuHardResetConditionOk;
uint8_t EcuSoftResetConditionOk;

/*
*********************************************************************************************************
*                                          
*********************************************************************************************************
*/
void Appl_EcuHardReset(void)
{
	//Do ECU Hard Reset Action
}

/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
void Appl_EcuSotfReset(void)
{
	//Do ECU Soft Reset Action
}

/*
*********************************************************************************************************
*                                     
*********************************************************************************************************
*/
uint8_t Appl_EcuHardReset_ConditionCheck(void)
{
	#if CONDITION_NOT_CORRECT > 0	
	return 1;
	#else
	EcuHardResetConditionOk = 1;
	return 0;
	#endif
}

/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
uint8_t Appl_EcuSoftResetCheck(void)
{
	#if CONDITION_NOT_CORRECT > 0	
	return 1;
	#else
	EcuSoftResetConditionOk = 1;
	return 0;
	#endif
}

/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/ 
uint8_t Appl_ClearFaultMemory_ConditionCheck(void)
{
	#if CONDITION_NOT_CORRECT > 0	
	return 1;
	#else
	return 0;
	#endif
}

/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
uint8_t Appl_ReadDataConditionCheck(void)
{
	#if CONDITION_NOT_CORRECT > 0	
	return 1;
	#else
	return 0;
	#endif
}

/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
// data_ptr = diag_tx_buff+3
uint8_t Appl_ReadRepairInfo(uint8_t *data_ptr)
{	  	
      /*for (index =0;index<sizeof(Dcm_RepairInformation);index++)
	  	{
	    	diag_tx_buff[index+3] = Dcm_RepairInformation[index];   
	  	}*/
}

/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
// data_ptr = diag_tx_buff+3
uint8_t Appl_ReadECUSeriNum(uint8_t *data_ptr)
{	  	
      /*for (index =0;index<sizeof(Dcm_RepairInformation);index++)
	  	{
	    	diag_tx_buff[index+3] = Dcm_RepairInformation[index];   
	  	}*/
}



/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
uint8_t Appl_WriteDataConditionCheck(void)
{
	#if CONDITION_NOT_CORRECT > 0	
	return 1;
	#else
	return 0;
	#endif
}


/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
// data_ptr = diag_rx_buff+3
uint8_t Appl_WriteECUSeriNum(uint8_t *data_ptr)
{
	  	/*for (index=0; index<sizeof(Dcm_ECUSerialNumber); index++)
	  	{
	    	Dcm_ECUSerialNumber[index] = diag_rx_buff[index+3];   
	  	}*/
	  	return 0;
}

/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
// data_ptr = diag_rx_buff+3
uint8_t Appl_WriteRepairInfo(uint8_t *data_ptr)
{
	  	/*for (index=0; index<sizeof(Dcm_RepairInformation); index++)
	  	{
	    	Dcm_RepairInformation[index] = diag_rx_buff[index+3];   
	  	}*/
	  	return 0;
}





/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
uint8_t Appl_ControlDTCSetting_ConditionCheck(void)
{
	#if CONDITION_NOT_CORRECT > 0	
	return 1;
	#else
	return 0;
	#endif
} 

void Appl_EnableRxAndTx_NCM(void)
{
}
void Appl_EnableRxAndTx_NWMCM(void)
{
}
void Appl_EnableRxAndTx_NCM_NWMCM(void)
{
}

void Appl_DisableRxAndTx_NCM(void)
{
}
void Appl_DisableRxAndTx_NWMCM(void)
{
}
void Appl_DisableRxAndTx_NCM_NWMCM(void)
{
}

/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
uint8_t Appl_RequestDownload_ConditionCheck(void)
{
	#if CONDITION_NOT_CORRECT > 0	
	return 1;
	#else
	return 0;
	#endif
}

/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
uint8_t Appl_RequestTransferExit_ConditionCheck(void)
{
	#if CONDITION_NOT_CORRECT > 0	
	return 1;
	#else
	return 0;
	#endif
}

/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
uint8_t Appl_SeedKeyMatched(uint16_t seed, uint16_t key)
{
   uint8_t seed_h,seed_l;
   uint8_t tmp1_h,tmp1_l;
   uint8_t tmp2_h,tmp2_l;
   uint8_t tmp3_h,tmp3_l;
   uint8_t tmp4_h,tmp4_l;
   uint8_t tmp5_h,tmp5_l;
   uint16_t generated_key;
  
   seed_h = (uint8_t)seed;
   seed_l = (uint8_t)(seed>>8);                                  //PROGRAM_CONST
   
   tmp1_h = ((uint8_t)(seed_h>>3)) & 0x1F + ((uint8_t)(seed_h<<5)) & 0xE0;
   tmp1_l = ((uint8_t)(seed_l>>3)) & 0x1F + ((uint8_t)(seed_l<<5)) & 0xE0;
   
   tmp2_h = ((uint8_t)(tmp1_h>>1)) & 0x55 + ((uint8_t)(tmp1_h<<1)) & 0xAA;
   tmp2_l = ((uint8_t)(tmp1_l>>1)) & 0x55 + ((uint8_t)(tmp1_l<<1)) & 0xAA;
  
   tmp3_h = ((uint8_t)(tmp2_l>>5)) & 0x07 + ((uint8_t)(tmp2_h<<3)) & 0xF8;
   tmp3_l = ((uint8_t)(tmp2_h>>5)) & 0x07 + ((uint8_t)(tmp2_l<<3)) & 0xF8;

   tmp4_h = (uint8_t)(tmp3_h ^ COEF1_CONST);
   tmp4_l = (uint8_t)(tmp3_l ^ COEF2_CONST);

   tmp5_h = (uint8_t)(tmp4_h ^ 0xFF);//Reverse every bit
   tmp5_l = (uint8_t)(tmp4_l ^ 0xFF);

   generated_key = (uint16_t)(tmp5_h<<8) + tmp5_l;
   
   if( generated_key == key ) 
    return 1;
   else 
    return 0;
}









/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
#if 1
uint8_t Dcm_ApplIOCReturnControlToECU( uint16_t dataId, uint8_t par_size, uint8_t *par, uint8_t *exitStatus)
{   
  switch(dataId)
	{
		case DCM_LEFT_DRL_CONTROL:		
		{
    	#if CONDITION_NOT_CORRECT > 0	
    	return 1;
    	#else
    	return 0;
    	#endif
		  break;
    }
		case DCM_RIGHT_DRL_CONTROL:		
		{
    	#if CONDITION_NOT_CORRECT > 0	
    	return 1;
    	#else
    	return 0;
    	#endif
		  break;
    }
		case DCM_LEFT_POSITION_LAMP_CONTROL:		
		{
    	#if CONDITION_NOT_CORRECT > 0	
    	return 1;
    	#else
    	return 0;
    	#endif
		  break;
    }
		case DCM_RIGHT_POSITION_LAMP_CONTROL:		
		{
    	#if CONDITION_NOT_CORRECT > 0	
    	return 1;
    	#else
    	return 0;
    	#endif
		  break;
    }
		default:
		{
		  return 0xFF;//out of range
		  break;
		}
	}
  return 0;  
}
#endif

/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
uint8_t Dcm_ApplIOCResetToDefault( uint16_t dataId, uint8_t par_size, uint8_t *par, uint8_t *exitStatus)
{   
  switch(dataId)
	{
		case DCM_LEFT_DRL_CONTROL://例子		
		{
    	#if CONDITION_NOT_CORRECT > 0	
    	return 1;
    	#else
    	return 0;
    	#endif
		  break;
    }
		case DCM_RIGHT_DRL_CONTROL:		
		{
    	#if CONDITION_NOT_CORRECT > 0	
    	return 1;
    	#else
    	return 0;
    	#endif
		  break;
    }
		case DCM_LEFT_POSITION_LAMP_CONTROL:		
		{
    	#if CONDITION_NOT_CORRECT > 0	
    	return 1;
    	#else
    	return 0;
    	#endif
		  break;
    }
		case DCM_RIGHT_POSITION_LAMP_CONTROL:		
		{
    	#if CONDITION_NOT_CORRECT > 0	
    	return 1;
    	#else
    	return 0;
    	#endif
		  break;
    }
		default:
		{
		  return 0xFF;
		  break;
		}
	}
  return 0;  
}

/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
//Dcm_ApplIOCShortTermAdustment();
uint8_t Dcm_ApplIOCShortTermAdustment( uint16_t dataId, uint8_t par_size, uint8_t *par, uint8_t *exitStatus)
{   
  switch(dataId)
	{
		case DCM_LEFT_DRL_CONTROL://例子		
		{
    	#if CONDITION_NOT_CORRECT > 0	
    	return 1;
    	#else
    	return 0;
    	#endif
		  break;
    }
		case DCM_RIGHT_DRL_CONTROL:		
		{
    	#if CONDITION_NOT_CORRECT > 0	
    	return 1;
    	#else
    	return 0;
    	#endif
		  break;
    }
		case DCM_LEFT_POSITION_LAMP_CONTROL:		
		{
    	#if CONDITION_NOT_CORRECT > 0	
    	return 1;
    	#else
    	return 0;
    	#endif
		  break;
    }
		case DCM_RIGHT_POSITION_LAMP_CONTROL:		
		{
    	#if CONDITION_NOT_CORRECT > 0	
    	return 1;
    	#else
    	return 0;
    	#endif
		  break;
    }
		default:
		{
		  return 0xFF;
		  break;
		}
	}
  return 0;  
}

/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
//响应的参数记录长度与请求的参数记录长度可能不同
uint8 Appl_FillTransRespParRecord(uint8 reqParRecordLen, uint8 *reqParRecord)
{
  //EQC-773 P67
  uint8 i;
  uint8 respParRecordLen;
  
  
  // sample, may no need to have response parameter
  for(i=0; i<reqParRecordLen; i++){
    diag_tx_buff[2+i] = reqParRecord[i]+1;
  }
  respParRecordLen = reqParRecordLen;
  
  return respParRecordLen;
}

/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
uint8 Appl_FillTransExitRespParRecord(uint8 reqParRecordLen, uint8 *reqParRecord)
{
  //EQC-773 P67
  uint8 i;
  uint8 respParRecordLen;
  
  for(i=0; i<reqParRecordLen; i++){
    diag_tx_buff[1+i] = reqParRecord[i]+1;
  }
  respParRecordLen = reqParRecordLen;//sample
  
  return respParRecordLen;
}


/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
//与memSize memAddress有关
void UserFunc_StartCheckProgDepend(uint8_t par_size, uint8_t *par, uint8_t *exitStatus)
{
  *exitStatus = 0xFF;//sample, 还会用到参数，但不用添加到响应报文里。status需要加入响应报文
}
void UserFunc_StopCheckProgDepend(uint8_t par_size, uint8_t *par, uint8_t *exitStatus)
{
  *exitStatus = 0xFF;//sample, 还会用到参数，但不用添加到响应报文里。status需要加入响应报文
}
void UserFunc_ReqResultCheckProgDepend(uint8_t *exitStatus)
{
  *exitStatus = 0xFF;//sample, 还会用到参数，但不用添加到响应报文里。status需要加入响应报文
}


 


/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
uint8_t Appl_StartRoutineByLocalId(uint16_t routineId, uint8_t par_size, uint8_t *par, uint8_t *exitStatus)
{
	switch(routineId)
	{
		case 0xFF01: 
		{		  
		  UserFunc_StartCheckProgDepend(par_size, par, exitStatus);
		  break;
		}
		
		default: 
		{
		  return 0xFF;
		  break; 
		}
	}
		
	return 0;
}

/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
uint8_t Appl_StopRoutineByLocalId( uint16_t routineId, uint8_t par_size, uint8_t *par, uint8_t *exitStatus )
{
	switch(routineId)
	{
		case 0xFF01: 
		{		  
		  UserFunc_StopCheckProgDepend(par_size, par, exitStatus);
		  break;
		}
		
		default: 
		{
		  return 0xFF;
		  break; 
		}
	}		
	return 0;
}

/*
*********************************************************************************************************
*                                        
*********************************************************************************************************
*/
uint8_t Appl_ReqRoutineResultByLocalId(uint16_t routineId, uint8_t *exitStatus)     
{
	switch(routineId)
	{
		case 0xFF01: 
		{		  
		  UserFunc_ReqResultCheckProgDepend(exitStatus);
		  break;
		}
		
		default: 
		{
		  return 0xFF;
		  break; 
		}
	}		
	return 0;
}










/*
*********************************************************************************************************
*                       Bootloader跳转判断                 
*********************************************************************************************************
*/
void JumpBoot(void)
{
   uint8 j;
   uint8 buf[3];
                                      
   for(j=0;j<3;j++)
   {
     buf[j] = gMSG_DIAG_RX.data[j];
   }

   //if((buf[0] == 0x02)&&(buf[1] == 0x10)&&(buf[2] == 0x03))
   {
       {__asm SEI;}//DisableInterrupts;
       MSCAN_Stop();
       asm ldx  #$fffe;
       asm ldx  0,x;
       asm jmp  0,x;
   }
}

