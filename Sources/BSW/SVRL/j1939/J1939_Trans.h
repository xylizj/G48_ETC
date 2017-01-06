#ifndef _J1939_TRANS_H_
#define _J1939_TRANS_H_

#include "typedefs.h"
#include "J1939_PGN.h"

/*! \brief BAM state machine states and Connection mode data transfer states */
#define CMDT_STATE_CLOSE   0
#define BAM_STATE_CLOSE    0
#define BAM_RX_STATE_DATA  1
#define BAM_TX_STATE_DATA  2
#define BAM_TX_STATE_WAIT  3
#define CMDT_RX_STATE_OPEN 4
#define CMDT_RX_STATE_DATA 5
#define CMDT_TX_STATE_OPEN 6
#define CMDT_TX_STATE_DATA 7
#define BAM_TX_STATE_LAST_PACKET 8

#define TPCM_RTS 16
#define TPCM_CTS 17
#define TPCM_EndOfMsgAck 19
#define TPCM_ABORT 255
#define TPCM_BAM 32

/*! \brief Timebases of the different TP protocols */
#define CMDT_TIMEOUT_4  550  /*!<  550ms timeout after CTS with 0 packets */
#define CMDT_TIMEOUT_3 1250  /*!<  1250ms timeout between data and next CTS */
#define CMDT_TIMEOUT_2 1250  /*!< 1250ms timeout between RTS/CTS and next data */
#define CMDT_TIMEOUT_1  250  /*!<  250ms timeout between two data packets */
#define BAM_RX_TIMEOUT  250	/*!< 250ms BAM timeout */
#define BAM_TX_TIMEOUT   50	/*!<  50ms BAM timeout between two messges */


/*******************************************************************************
* Defines                                                                
*******************************************************************************/
struct TP_Manager_tag
{
  vuint8_t   State;               
  vuint8_t   Sequence;        
  vuint16_t  Bytes;           
  vuint32_t  Timer;
  vuint16_t  Offset;            
  vuint8_t   Packets; 
};


struct TPCM_tag 
{ 
  vuint8_t Control;    
  vuint8_t BytesLsb;
  vuint8_t BytesMsb;
  vuint8_t Packets;
  vuint8_t MaxPackets;
  vuint8_t PGNlsb;
  vuint8_t PGN;
  vuint8_t PGNmsb;
};

struct TPDT_tag 
{
  vuint8_t Sequence;          
  vuint8_t Data[7];            
};


struct TP_data_tag 
{ 
	vuint16_t  Bytes;
	vuint8_t data[64]; 
};



extern volatile struct TPCM_tag gMSG_TPCM;
extern volatile struct TPDT_tag gMSG_TPDT;
extern volatile struct TP_Manager_tag TP;
extern volatile struct TP_data_tag 	TP_data;


extern void	J1939TP_State(void);



#endif