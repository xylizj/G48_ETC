
#ifndef DCM_H
#define DCM_H

#include "diag.h"
#include "dem.h"
#include "dcm_cbk.h"


typedef enum
{
  DEFAULT_SESSION_MODE = 1,
  PROGRAMMING_SESSION_MODE = 2,
  EXTENDED_SESSION_MODE = 3,  
}Dem_ErrMemStatusType;


#define NCM       0x01//常规应用报文
#define NWMCM     0x02//网络管理报文
#define NCM_NWMCM 0x03//both

#define DCM_PRODUCT_PART_NUM         0xF187U //Product Part Number
#define DCM_SYS_NAME                 0xF197U
#define DCM_SUPPLIER_ID              0xF18AU//Supplier Identifier
#define DCM_ECUSW_VERSION            0xF189U
#define DCM_ECUHW_VERSION            0xF179U
#define DCM_SW_CHECK                 0xF17FU
#define DCM_SW_PROGRAMMING_DATE      0xF199U
#define DCM_REPAIR_INFO              0xF1A1U
#define DCM_ECU_SN                   0xF18CU
#define DCM_CALIBRA_SW_VERSION       0xF19CU
#define DCM_HW_VERSION               0xF193U
#define DCM_SW_VERSION               0xF195U

#define DCM_LEFT_DRL_INFO                0xFD01U
#define DCM_RIGHT_DRL_INFO               0xFD02U
#define DCM_LEFT_POSITION_LAMP_INFO      0xFD03U
#define DCM_RIGHT_POSITION_LAMP_INFO     0xFD04U
#define DCM_LEFT_DRL_CONTROL             0xFD05U
#define DCM_RIGHT_DRL_CONTROL            0xFD06U
#define DCM_LEFT_POSITION_LAMP_CONTROL   0xFD07U
#define DCM_RIGHT_POSITION_LAMP_CONTROL  0xFD08U

#define DCM_CHECK_PROGRAMMING_DEPENDENCE 0xFF01U



extern uint8_t DCM_m_ct_SecurityFailCnt;



extern void Dcm_ApplStartDefaultSession(void);
extern void Dcm_ApplStartProgSession(void);
extern void Dcm_ApplStartExtSession(void);
extern void Dcm_DiagSessionControl(uint8 diagSessionType);
//extern void Dcm_EcuReset(void);
extern void Dcm_ApplEcuHardReset(void);
extern void Dcm_ApplEcuSoftReset(void);
extern void Dcm_ApplClearFaultMemory(void);
//extern void Dcm_ApplReadECUId(uint8_t id_option);
extern void Dcm_ApplReportNumberOfDTCByStatusMask(Dem_DTCStatusMaskType DTCStsMask);
extern void Dcm_ApplReportDTCByStatusMask(Dem_DTCStatusMaskType DTCStsMask);
extern void Dcm_ApplReportSupportedDTC(void);
extern void Dcm_ApplReadDataByLocalId(uint16_t lid);
extern void Dcm_ApplWriteDataByLocalId(uint16_t lid);
extern void Dcm_ApplIOCById( vuint16_t dataId, vuint8_t par_size, vuint8_t *par );
extern void Dcm_ApplControlDTCSettingOn(void);
extern void Dcm_ApplControlDTCSettingOff(void);
extern void Dcm_ApplEnableRxAndTx(uint8 communicationType);
extern void Dcm_ApplDisableRxAndTx(uint8 communicationType);
//extern void Dcm_ApplEnableRxAndTx(void);
//extern void Dcm_ApplDisableRxAndTx(void);
extern void Dcm_ApplRequestSeed(void);
extern void Dcm_ApplSendKey(uint16_t key);
extern void Dcm_ApplStartRoutineByLocalId( uint16_t routineId, uint8_t par_size, uint8_t *par );
extern void Dcm_ApplStopRoutineByLocalId( uint16_t routineId, uint8_t par_size, uint8_t *par );
extern void Dcm_ApplReqRoutineResultByLocalId(uint16_t routineId);
extern void Dcm_DcmTesterPresent();
extern void Dcm_ReadMemoryByAddress(uint32_t address, uint8_t size);
extern void Dcm_ApplRequestDownload(void);
extern void Dcm_ApplTransferData(uint8 blcokSeqCnt, uint8 reqParRecordLen, uint8 *reqParRecord);
extern void Dcm_ApplRequestTransferExit(uint8 reqParRecordLen, uint8 *reqParRecord);

extern void Dcm_SetNegResponse(uint8_t errorCode);



extern uint8_t Appl_SeedKeyMatched(uint16_t seed, uint16_t key);








#endif /* #ifndef DCM_H */
