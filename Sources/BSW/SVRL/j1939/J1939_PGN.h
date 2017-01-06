/***************************************************************************************
File name: J1939PGN.h

2012-02-01, lzy, created, added channel for HCU
2012-10-27, lzy, modify PGN_TX/RX_tag

***************************************************************************************/

#ifndef _J1939_PGN_H_
#define _J1939_PGN_H_



#include "typedefs.h"
#include "J1939_Cfg.h"
#include "J1939_Trans.h"

#ifndef NULL
  #define NULL ((void *) 0)
#endif


typedef void  (* FuncType ) (void);


struct PGN_TX_tag
{
  vuint32_t id;         //4  
  uint32_t  *data;      //8   
  vuint8_t  dl;   		//9
  vuint8_t  enable;     //10    
  vuint8_t  state;      //11    
  vuint8_t 	tt;			//12
  vuint32_t rate;    	//16
  vuint32_t timer;      //20 
  FuncType encode_func; //24
  FuncType tx_err_func; //28
} ;				//size = 28 bytes


struct PGN_RX_tag
{
  vuint32_t id;         //4  
  uint32_t  *data;      //8   
  vuint8_t  dl;   		//9
  vuint8_t  enable;     //10    
  vuint8_t  state;      //11    
  vuint8_t 	tt;			//12
  vuint32_t rate;    	//16
  vuint32_t timer;      //20 
  FuncType decode_func; //24
  FuncType rx_timeout_func; //28 
} ;				//size = 28 bytes





struct EtcInfo_tag{
	union 
	{
	    vuint8_t ActualPos_lo;
	} Byte1;

	union 
	{
	    vuint8_t ActualPos_hi;
	} Byte2;

	union 
	{
	    vuint8_t HBridge;
	} Byte3;

	union 
	{
		vuint8_t R;
		struct {
  		uint16_t reserved:7;
  		vuint16_t PosSensorPlau:1;	
		} B;
	}Byte4;

	union 
	{
	    vuint8_t SetPos_lo;
	} Byte5;

	union 
	{
	    vuint8_t SetPos_hi;
	} Byte6;

	union 
	{
	    vuint8_t GovDvt_lo;
	} Byte7;

	union 
	{
	    vuint8_t GovDvt_hi;
	} Byte8;
};



/*DM1_EDC_tag*/
struct DM1_EDC_tag
{
    union 
	{
		vuint8_t R;
		struct {
		vuint8_t MILLmpSt:2;
		vuint8_t RedStopLmpSt:2;
		vuint8_t AmberWarnLmpSt:2;
		vuint8_t ProtectLmpSt:2;
		} B;
	} Byte1;
	
	vuint8_t Byte2;
	
	union 
	{
		vuint8_t R;
		struct {
		vuint8_t MSB:3;
		vuint8_t FailureModeIdentifier:5;
		} B;
	} Byte3;
	
	union 
	{
	    vuint8_t SPN_lo;
	} Byte4;
	
	union 
	{
	    vuint8_t SPN_hi;
	} Byte5;
	
	union 
	{
		vuint8_t R;
		struct {
		vuint8_t SPNConversionMethod:1;
		vuint8_t OccurenceCnt:7;
		} B;
	} Byte6;
	
	vuint8_t Byte7;
    vuint8_t Byte8;
		
};

/*DM1_VECU_tag*/
struct DM1_VECU_tag
{
    union 
	{
		vuint8_t R;
		struct {
		vuint8_t MILLmpSt:2;
		vuint8_t RedStopLmpSt:2;
		vuint8_t AmberWarnLmpSt:2;
		vuint8_t ProtectLmpSt:2;
		} B;
	} Byte1;
	
	vuint8_t Byte2;
	
	union 
	{
	    vuint8_t SPN_lo;
	} Byte3;
	
	union 
	{
	    vuint8_t SPN_hi;
	} Byte4;
	
	
	union 
	{
		vuint8_t R;
		struct {
		vuint8_t MSB:3;
		vuint8_t FailureModeIdentifier:5;
		} B;
	} Byte5;
	
	union 
	{
		vuint8_t R;
		struct {
		vuint8_t SPNConversionMethod:1;
		vuint8_t OccurenceCnt:7;
		} B;
	} Byte6;
	
	vuint8_t Byte7;
    vuint8_t Byte8;
		
};



struct XCP_RX_tag 
{ 
	vuint32_t data[2]; 
};

struct CAP_TX_tag 
{ 
	vuint16_t data[4]; 
};



/*BAM__tag*/
struct BAM_ECU_tag
{
    union 
	{
	    vuint8_t CNFBAM_CtlByte;
	} Byte1;
	
	union 
	{
	    vuint8_t CNFBAM_SizeByte;
	} Byte2;
	
	union 
	{
	    vuint8_t CNFBAM_SizeByte;
	} Byte3;
	
	union 
	{
	    vuint8_t CNFBAM_SizePck;
	} Byte4;
	
	vuint8_t Byte5;
	
        union
        {
           vuint8_t CNFBAM_ParmGrpNr;
        }Byte6;
	
	 union
        {
           vuint8_t CNFBAM_ParmGrpNr;
        }Byte7;

	union 
	{
	   vuint8_t CNFBAM_ParmGrpNr;
	} Byte8;
		
};






struct DM1_tag { vuint8_t data[4*10+2]; };

struct XCP_TX_tag { vuint8_t data[8]; };





























#endif

