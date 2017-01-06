#include "J1939_Trans.h"
#include "J1939_Ext.h"


volatile struct TPCM_tag 		gMSG_TPCM;
volatile struct TPDT_tag 		gMSG_TPDT;
volatile struct TP_Manager_tag 	TP;//xyl modify 20160719
volatile struct TP_data_tag 	TP_data;//xyl add 20160719

void J1939TP_Init(void)
{
	TP.State = BAM_STATE_CLOSE;
}

/*void J1939TP_State(void)
{
	vuint8_t state;
	vuint32_t ct;
	uint8_t i;
	
	state = TP.State;  
	
	switch(state)
	{
		case CMDT_TX_STATE_OPEN:
		{
			if(TxMsgBuffA[MSG_DM1_TX_INDEX].dl > 42)
			{
				TP.State = BAM_STATE_CLOSE;//xyl add 20160719
				return;
			}
  			//TP.Bytes = TxMsgBuffA[MSG_DM1_TX_INDEX].dl;
   			TP.Bytes = TP_data.Bytes;//xyl modify 20160719
 			TP.Sequence = 1;
  			TP.Packets = (uint8_t)((TP.Bytes / 7) + ((TP.Bytes % 7)?1:0));
  			TP.State = BAM_TX_STATE_WAIT; 
  			TP.Offset = 0;
  			TP.Timer = read_tbl(); 
  			TxMsgBuffA[MSG_TPDT_TX_INDEX].timer = TP.Timer;
  			
			gMSG_TPCM.Control    = TPCM_BAM;
			gMSG_TPCM.BytesLsb   = (uint8_t)(TP.Bytes & 0xFF); 
			gMSG_TPCM.BytesMsb   = (uint8_t)(TP.Bytes >> 8);
			gMSG_TPCM.Packets    = TP.Packets;
			gMSG_TPCM.MaxPackets = 0xFF;  
			gMSG_TPCM.PGN        = 0xFE;
			gMSG_TPCM.PGNlsb   = 0xCA;
			gMSG_TPCM.PGNmsb   = 00;
			TxMsgBuffA[MSG_TPCM_TX_INDEX].state = PG_TX_REQ;
			
			break;
		}
		
		case BAM_TX_STATE_DATA:
		{
			gMSG_TPDT.Sequence = TP.Sequence;
			
			if( TP.Sequence == TP.Packets )
			{
				uint8_t n = TP.Bytes%7;
				
				if( n == 0 ) 
				{
					n = 7;
				}
				
				for( i=0; i<n; i++ )
				{
					gMSG_TPDT.Data[i] = TP_data.data[TP.Offset];//xyl modify 20160719
					TP.Offset++;
				}
				
				if( n < 7 )
				{
					for( i=n; i<7; i++ )
					{
						gMSG_TPDT.Data[i] = 0xFF;
					}
				}
				TxMsgBuffA[MSG_TPDT_TX_INDEX].state = PG_TX_REQ;
				TP.State = BAM_TX_STATE_LAST_PACKET;				
			}
			else
			{
				for( i=0; i<7; i++ )
				{
					gMSG_TPDT.Data[i] = TP_data.data[TP.Offset];//xyl modify 20160719
					TP.Offset++;
				}
				TxMsgBuffA[MSG_TPDT_TX_INDEX].state = PG_TX_REQ;
				TP.State = BAM_TX_STATE_WAIT;
				TP.Sequence++;			
			}
			break;
		}
		  	
		case BAM_TX_STATE_LAST_PACKET:
		{
			if(TxMsgBuffA[MSG_TPDT_TX_INDEX].state != PG_TX_REQ)
			{
				TP.State = BAM_STATE_CLOSE;
				TxMsgBuffA[MSG_DM1_TX_INDEX].state = PG_TX_FREE;				
				TxMsgBuffA[MSG_EC1_TX_INDEX].state = PG_TX_FREE;//xyl add 20160719				
			}
			break;
		}
		
		default:
			break;
	}	
}



void J1939MsgTPCM_TxErrorIndication(void)
{ 
	TP.State = BAM_STATE_CLOSE;
	TxMsgBuffA[MSG_DM1_TX_INDEX].state = PG_TX_FREE;
	TxMsgBuffA[MSG_EC1_TX_INDEX].state = PG_TX_FREE;//xyl add 20160719				
}


void J1939MsgTPDT_TxErrorIndication(void)  
{ 
	TP.State = BAM_STATE_CLOSE;
	TxMsgBuffA[MSG_DM1_TX_INDEX].state = PG_TX_FREE; 
	TxMsgBuffA[MSG_EC1_TX_INDEX].state = PG_TX_FREE;//xyl add 20160719				
}*/

