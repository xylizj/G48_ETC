#ifndef DCM_CBK_H
#define DCM_CBK_H

#include "Platform_Types.h"

#define SESSION_CHECK_ENABLE 1


#define COEF1_CONST 0xE5
#define COEF2_CONST 0x38
                         

//!<======================================================
//!<  use macro to instead sizeof
#define LEN_PRODUCT_PART_NUM            14
#define LEN_SYS_NAME                    3    
#define LEN_SUPPLYER_ID                 6    
#define LEN_ECU_SW_VER                  2    
#define LEN_ECU_HW_VER                  2    
#define LEN_SW_CHECK                    2    
#define LEN_PROG_DATE                   4    
#define LEN_REPAIR_INFO                 12    
#define LEN_ECU_SN                      12    
#define LEN_CALIBRA_SW_VER              3    
#define LEN_HW_VER                      1    
#define LEN_SW_VER                      2    
//!<======================================================


extern const uint8_t Dcm_ProductPartNumber[LEN_PRODUCT_PART_NUM];                          
extern const uint8_t Dcm_SystemName[LEN_SYS_NAME];                          
extern const uint8_t SupplierIdentifier[LEN_SUPPLYER_ID];                          
extern const uint8_t Dcm_ECUSoftwareVersionNumber[LEN_ECU_SW_VER];                          
extern const uint8_t Dcm_ECUHardtwareVersionNumber[LEN_ECU_HW_VER];                          
extern const uint8_t Dcm_SoftwareCheck[LEN_SW_CHECK];  
extern const uint8_t Dcm_ProgrammingDate[LEN_PROG_DATE];  
extern uint8_t Dcm_RepairInformation[LEN_REPAIR_INFO];//May be modified by Service WriteDataByLocalId 
extern uint8_t Dcm_ECUSerialNumber[LEN_ECU_SN];
extern const uint8_t Dcm_CalibrationSoftwareVersion[LEN_CALIBRA_SW_VER];  
extern const uint8_t Dcm_HardwareVersion[LEN_HW_VER];
extern const uint8_t Dcm_SoftwareVersion[LEN_SW_VER]; 



extern uint8_t diagSessionMode;

extern uint8_t EcuHardResetConditionOk;
extern uint8_t EcuSoftResetConditionOk;


extern uint8_t leftDRLInfo;
extern uint8_t rightDRLInfo;
extern uint8_t leftPosLampInfo;
extern uint8_t rightPosLampInfo;
extern uint32_t memSize;
extern uint32_t memAddress;

extern uint8_t leftDRLControl;
extern uint8_t rightDRLControl;
extern uint8_t leftPosLampControl;
extern uint8_t rightPosLampControl;




extern void Appl_EcuHardReset(void);
extern void Appl_EcuSotfReset(void);

extern uint8_t Appl_EcuHardReset_ConditionCheck(void);
extern uint8_t Appl_EcuSoftResetCheck(void);
extern uint8_t Appl_ClearFaultMemory_ConditionCheck(void);
//extern uint8_t Appl_SeedKeyMatched(uint32_t seed, uint32_t key);
extern uint8_t Appl_SeedKeyMatched(uint16_t seed, uint16_t key);  
extern uint8_t Appl_ReadDataConditionCheck(void);
extern uint8_t Appl_ReadECUSeriNum(uint8_t *data_ptr);
extern uint8_t Appl_ReadRepairInfo(uint8_t *data_ptr);
extern uint8_t Appl_WriteDataConditionCheck(void);
extern uint8_t Appl_WriteRepairInfo(uint8_t *data_ptr);
extern uint8_t Appl_ControlDTCSetting_ConditionCheck(void);

extern void Appl_EnableRxAndTx_NCM(void);
extern void Appl_EnableRxAndTx_NWMCM(void);
extern void Appl_EnableRxAndTx_NCM_NWMCM(void);
extern void Appl_DisableRxAndTx_NCM(void);
extern void Appl_DisableRxAndTx_NWMCM(void);
extern void Appl_DisableRxAndTx_NCM_NWMCM(void);

extern uint8_t Appl_RequestDownload_ConditionCheck(void);
extern uint8_t Appl_RequestTransferExit_ConditionCheck(void);

extern uint8_t Appl_StartRoutineByLocalId( uint16_t lid, uint8_t par_size, uint8_t *par, uint8_t *exitStatus );
extern uint8_t Appl_StopRoutineByLocalId( uint16_t lid, uint8_t par_size, uint8_t *par, uint8_t *exitStatus );
extern uint8_t Appl_ReqRoutineResultByLocalId(uint16_t lid, uint8_t *exitStatus);

extern uint8_t Dcm_ApplIOCReturnControlToECU( uint16_t dataId, uint8_t par_size, uint8_t *par, uint8_t *exitStatus );
extern uint8_t Dcm_ApplIOCResetToDefault( uint16_t dataId, uint8_t par_size, uint8_t *par, uint8_t *exitStatus);
extern uint8_t Dcm_ApplIOCShortTermAdustment( uint16_t dataId, uint8_t par_size, uint8_t *par, uint8_t *exitStatus);

extern uint8_t Appl_FillTransRespParRecord(uint8_t reqParRecordLen, uint8_t *reqParRecord);
extern uint8_t Appl_FillTransExitRespParRecord(uint8_t reqParRecordLen, uint8_t *reqParRecord);



extern void JumpBoot(void);

































#endif
