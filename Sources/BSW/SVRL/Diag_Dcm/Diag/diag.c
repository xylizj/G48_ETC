/************************************************************************
File Name:      diag.c
Project:		

Last Modified:	by xyl on 2015-04-15

Records:

  xyl, some been modified and some been added to 
    adapt for G64 DRL from ISO15765 TP for MPC5633M platform,
    combined with previous CAN config and Tx Rx functions   

************************************************************************/

#include "diag.h"



//void memcpy(uint8_t *dest, uint8_t *source, uint16_t count);

//volatile RX_tag_type RxMsgBuffA[CAN0_RX_MSG_NUM];
//volatile TX_tag_type TxMsgBuffA[CAN0_TX_MSG_NUM];

vuint8_t diag_rx_buff[DIAG_RX_BUFF_SIZE];
vuint8_t diag_tx_buff[DIAG_TX_BUFF_SIZE];

volatile struct RX_TP_tag rx_state;
volatile struct TX_TP_tag tx_state;

volatile struct DIAG_RX_tag 	gMSG_DIAG_RX;
volatile struct DIAG_TX_tag 	gMSG_DIAG_TX;

vuint8_t Diag_state;
vuint8_t ucRxTSmin;
vuint8_t ucRxBSmax;
vuint8_t ucTxTSmin;
vuint8_t ucTxBSmax;
uint8_t bs_sn;


void Diag_InitTP(void);
void Diag_SendFlowControl(void); 
void Diag_State(void);
void Diag_TPTransmit(void);
void Diag_MsgIndication(void); 
void MsgDIAG_RX_Decode(void);  


/*void ConfigTxMsgA(uint8_t index, uint32_t id, uint8_t dl, uint32_t* data)
{
	if( index >= CAN0_TX_MSG_NUM )
	{
		return;
	}	
	TxMsgBuffA[index].id 		= id;	
	TxMsgBuffA[index].dl 		= dl;	
	TxMsgBuffA[index].data 		= data;	
	TxMsgBuffA[index].state 	= PG_TX_FREE;	
	TxMsgBuffA[index].timer 	= read_tbl();//Modified by xyl 2014.11.28
}

void ConfigRxMsgA(uint8_t index, uint32_t id, uint8_t dl, uint32_t* data)
{
	if( index >= CAN0_RX_MSG_NUM )
	{
		return;
	}	
	RxMsgBuffA[index].id 		= id;	
	RxMsgBuffA[index].dl 		= dl;	
	RxMsgBuffA[index].data 		= data;	
	RxMsgBuffA[index].state 	= PG_RX_FREE;	
	RxMsgBuffA[index].timer 	=  read_tbl();//Modified by xyl 2014.11.28
}*/





void Diag_InitTP(void)
{
	rx_state.timer = read_tbl();//Modified by xyl 2014.11.28
	rx_state.rx_buffer = diag_rx_buff;
	rx_state.data_length = 0;
	rx_state.index = 0;
	rx_state.sn = 0;
	rx_state.st = kRxState_Idle;
	rx_state.target_address = 0xff;
	rx_state.source_address = 0xff;

	tx_state.timer = read_tbl();//Modified by xyl 2014.11.28
	tx_state.tx_buffer = diag_tx_buff;
	tx_state.data_length = 0;
	tx_state.index = 0;
	tx_state.sn = 0;
	tx_state.st = kTxState_Idle;
	tx_state.target_address = 0xff;
	tx_state.source_address = 0xff;
  
  Diag_state = DIAG_STATE_INACTIVE;
  ucRxTSmin = 20;//150ms/50ms
	ucRxBSmax = 0;
	bs_sn = 1;
	
	diagSessionMode = DEFAULT_SESSION_MODE;

  //ConfigRxMsgA(MSG_DIAG_RX_INDEX, MSG_DIAG_RX_PHY_ID, 8, (uint32_t*)&gMSG_DIAG_RX);	
	//ConfigTxMsgA(MSG_DIAG_TX_INDEX, MSG_DIAG_TX_ID, 8, (uint32_t*)&gMSG_DIAG_TX);
}


/*
Single Frame
*/
uint32_t idSingleFrame;
static void SingleFrameHandle(void)
{
  uint16_t dl;
  
  idSingleFrame = real_id;

  if( rx_state.st != kRxState_Idle )//remove for 9.18	单帧中断请求
  {
  	return ;
  }
  if( (0x0f & RxMsgBuffA[MSG_DIAG_RX_INDEX].dl) != 8 )   //for DFEV company, dl must be 8, xyl 20151208 add
  {
  	rx_state.st = kRxState_Idle;
  	return ;
  }

  dl = gMSG_DIAG_RX.data[0];
  if( (dl>7) || (dl==0) )
  {
  	return ;
  }
  rx_state.timer = RxMsgBuffA[MSG_DIAG_RX_INDEX].timer;
  rx_state.data_length = dl;
  rx_state.index = 0;
  memcpy((uint8_t *)&diag_rx_buff[0],(uint8_t *)&gMSG_DIAG_RX.data[1], dl);
  rx_state.st = kRxState_ApplInformed;
  Diag_MsgIndication();
  rx_state.st = kRxState_Idle;
}
/*
First Frame
*/
uint32_t idFirstFrame;
static void FirstFrameHandle(void)
{    		
  uint16_t dl;
  
  idFirstFrame = real_id;
  if(MSG_DIAG_RX_FUN_ID==idFirstFrame)
  {
    return;
  }
  if( rx_state.st != kRxState_Idle )
  {
  	return ;
  }
  
  
  if( (0x0f & RxMsgBuffA[MSG_DIAG_RX_INDEX].dl) != 8 )   //for FF, dl must be 8
  {
  	rx_state.st = kRxState_Idle;
  	return ;
  }

  dl =  (uint16_t)((gMSG_DIAG_RX.data[0] & 0x0f)<<8) + (uint16_t)gMSG_DIAG_RX.data[1];
  if(dl<8) 
  {
  	return ;
  }
  memcpy((uint8_t *)&diag_rx_buff[0],(uint8_t *)&gMSG_DIAG_RX.data[2], 6);
  rx_state.data_length = dl;
  rx_state.index = 6;
  rx_state.sn = 1;
  rx_state.timer = RxMsgBuffA[MSG_DIAG_RX_INDEX].timer;
  Diag_SendFlowControl();
  rx_state.st = kRxState_TxFC;
}
/*
Consecutive Frame
*/
static void ConsecutiveFrameHandle(void)
{
  uint16_t dl;
  uint32_t tmp;
  
  if( rx_state.st != kRxState_WaitCF ) 
  {
  	rx_state.st = kRxState_Idle;
    return ;
  }

  if( (0x0f & RxMsgBuffA[MSG_DIAG_RX_INDEX].dl) != 8 )   //for DFEV company, dl must be 8, xyl 20151208 add
  {
  	rx_state.st = kRxState_Idle;
  	return ;
  }
  
  if( (gMSG_DIAG_RX.data[0] & 0x0f) != rx_state.sn )//丢弃重复的连续帧
  {
  	return ;
  }
  
  dl = rx_state.data_length - rx_state.index;
  dl = (dl>7)?7:dl;
 	memcpy((uint8_t *)&diag_rx_buff[rx_state.index],(uint8_t *)&gMSG_DIAG_RX.data[1],dl);  rx_state.index += 7;
  if( rx_state.index >= rx_state.data_length ) 
  {
  	rx_state.st = kRxState_ApplInformed;
  	Diag_MsgIndication();
  	rx_state.st = kRxState_Idle;
  } 
  else 
  {
  	rx_state.st = kRxState_WaitCF;
  	rx_state.sn++;
  	if( rx_state.sn == 16 ) 
  	{
  		rx_state.sn = 0;
  	}
  }
}
/*
FlowControl Frame
*/
uint32_t idFlowControlFrame;
static void FlowControlFrameHandle(void)
{
	idFlowControlFrame = real_id;
	if(MSG_DIAG_RX_PHY_ID==idSingleFrame && MSG_DIAG_RX_FUN_ID==idFlowControlFrame)
	{
	  return;
	}
	  
  if(tx_state.st != kTxState_WaitFC)//20151209 xyl
  {
    //tx_state.st = kTxState_Idle;
    return;
  }

  if( (0x0f & RxMsgBuffA[MSG_DIAG_RX_INDEX].dl) != 8 )   //for DFEV company, dl must be 8, xyl 20151208 add
  {
  	rx_state.st = kRxState_Idle;
  	return ;
  }

  if( tx_state.st == kTxState_WaitFC )
	{    			
		if( (gMSG_DIAG_RX.data[0] & 0x0f) != 0) 
		{
	     return;
	  }     			      		
	  ucTxBSmax = gMSG_DIAG_RX.data[1];
		ucTxTSmin = gMSG_DIAG_RX.data[2];
		tx_state.st = kTxState_WaitTxCF;
		tx_state.timer = RxMsgBuffA[MSG_DIAG_RX_INDEX].timer;
		bs_sn = 1;
	}
}


extern uint8_t CAN_DLC;
/*
After receive Diag message, 
*/
void MsgDIAG_RX_Decode(void)  
{
    RxMsgBuffA[MSG_DIAG_RX_INDEX].timer = read_tbl();//ADDED by xyl 2014.11.28
	  
	  Diag_state = DIAG_STATE_ACTIVE;
	
    switch( gMSG_DIAG_RX.data[0] & 0xf0 ) 
    {
    	case kSingleFrame:
    	{	
        SingleFrameHandle();
    		break;
      }
      		
    	case kFirstFrame:
    	{
        FirstFrameHandle();
    		break;
      }
    		
    	case kConsecutiveFrame:
    	{
        ConsecutiveFrameHandle();
    		break;
    	}
    		
    	case kFlowControlFrame:
    	{      		
        FlowControlFrameHandle();
    		break;
    	}
    		
    	default:
    		break;
    }  
} 
      

void Diag_SendFlowControl(void) 
{
	/*if( tx_state.st != kTxState_Idle )
	{
		return;
	}*/
    gMSG_DIAG_TX.data[0] = kFlowControlFrame;
    gMSG_DIAG_TX.data[1] = ucRxBSmax;
    gMSG_DIAG_TX.data[2] = ucRxTSmin;
    gMSG_DIAG_TX.data[3] = 0;
    gMSG_DIAG_TX.data[4] = 0;
    gMSG_DIAG_TX.data[5] = 0;
    gMSG_DIAG_TX.data[6] = 0; 
    gMSG_DIAG_TX.data[7] = 0;
    TxMsgBuffA[MSG_DIAG_TX_INDEX].state = PG_TX_REQ;
    return ;
}






void Diag_TPTransmit(void)
{
	uint8 i;
	
	if( 0 == tx_state.data_length){
	  return;
	}
	
	if( tx_state.data_length < 0x08 )
	{
	    gMSG_DIAG_TX.data[0] = tx_state.data_length;
	    for(i=1; i<=tx_state.data_length;i++){
	      gMSG_DIAG_TX.data[i] = diag_tx_buff[i-1];
	    }
	    for(i=7; i>tx_state.data_length;i--){
	      gMSG_DIAG_TX.data[i] = 0;
	    }	    
	    
	    TxMsgBuffA[MSG_DIAG_TX_INDEX].state = PG_TX_REQ;
	    tx_state.st = kTxState_ReqTx;
	    tx_state.index = tx_state.data_length;
	}
	else
	{
		/* Send First Frame */
	    gMSG_DIAG_TX.data[0] = kFirstFrame + (0x0f & (tx_state.data_length>>8));
	    gMSG_DIAG_TX.data[1] = (uint8_t)(0xff & (tx_state.data_length) );
	    gMSG_DIAG_TX.data[2] = diag_tx_buff[0];
	    gMSG_DIAG_TX.data[3] = diag_tx_buff[1];
	    gMSG_DIAG_TX.data[4] = diag_tx_buff[2];
	    gMSG_DIAG_TX.data[5] = diag_tx_buff[3];
	    gMSG_DIAG_TX.data[6] = diag_tx_buff[4];
	    gMSG_DIAG_TX.data[7] = diag_tx_buff[5];
	    tx_state.index = 6;
	    tx_state.sn = 1;
	    //bs_sn = 1;
	    TxMsgBuffA[MSG_DIAG_TX_INDEX].state = PG_TX_REQ;
	    rx_state.timer = read_tbl();//ADDED BY XYL 2015.4.15
	    tx_state.st = kTxState_WaitFC;
	}
}

static void ConsecutiveFrameTramsmittingHandle(void)	
{
	uint32_t ct;
	uint32_t tmp;
	uint16_t dl;
	uint8 i;
	
	ct = read_tbl();
	
	if(	tx_state.st == kTxState_WaitTxCF )
	{
		if(ct >= tx_state.timer)
		  tmp = ct - tx_state.timer;
		else
		  tmp = 0xFFFFFFFF - tx_state.timer + ct;
		//tmp = ct - tx_state.timer;//20161116 xyl modify
		
		/* Check P2 Timeout */
		if( tmp > DIAG_P2_TIMEOUT_COUNT )
		{
			Diag_InitTP();
		}
    
    if(ucTxBSmax!=0 && ucTxBSmax<bs_sn)//added by xyl 2015.4.14
    {
      tx_state.st = kTxState_WaitFC;
      return;
    }
		
		if( ( tmp > (ucTxTSmin*TBL_COUNT_1MS_DIAG) ) && (TxMsgBuffA[MSG_DIAG_TX_INDEX].state == PG_TX_FREE ) )
		{      
      dl = tx_state.data_length - tx_state.index;
      if( dl <= 7 )
      {
        gMSG_DIAG_TX.data[0] = kConsecutiveFrame + tx_state.sn;
      	for(i=7; i>dl;i--)//added by xyl 2015.2.13
      	{
	        gMSG_DIAG_TX.data[i] = 0;
	      }
	      memcpy((uint8_t *)&gMSG_DIAG_TX.data[1],(uint8_t *)&diag_tx_buff[tx_state.index],dl);
      	tx_state.index += dl;
      	tx_state.st = kTxState_Idle;
      }
      else
      {
        gMSG_DIAG_TX.data[0] = kConsecutiveFrame + tx_state.sn;
      	memcpy((uint8_t *)&gMSG_DIAG_TX.data[1],(uint8_t *)&diag_tx_buff[tx_state.index],7);
      	tx_state.index += 7;
        tx_state.sn++;
        bs_sn++;
        if( tx_state.sn == 16 ) 
          tx_state.sn = 0;
      }
            
			TxMsgBuffA[MSG_DIAG_TX_INDEX].state = PG_TX_REQ;
      tx_state.timer = ct;
		}		  	
	}        
}



void Diag_State(void)
{
	uint32_t ct;
	uint32_t tmp;
	uint16_t dl;
	uint8 i;
	
	ct = read_tbl();

	/* Check TX message */
	if( TxMsgBuffA[MSG_DIAG_TX_INDEX].state == PG_TX_REQ ) 
	{ 
		//if(IO_ERR_OK == CAN_Tx(MSG_DIAG_TX_SEND))
		if( !CAN_A_Transmit(TxMsgBuffA[MSG_DIAG_TX_INDEX].id, TxMsgBuffA[MSG_DIAG_TX_INDEX].data, TxMsgBuffA[MSG_DIAG_TX_INDEX].dl) )
		{
			TxMsgBuffA[MSG_DIAG_TX_INDEX].state = PG_TX_FREE;			
			if( rx_state.st == kRxState_TxFC )
			{
				rx_state.st = kRxState_WaitCF;
			}			
			if(tx_state.st == kTxState_ReqTx)
			{
				tx_state.st = kTxState_Idle;
			}
		}
	}
    		
	ConsecutiveFrameTramsmittingHandle();

	/* Check P3 Timeout */
	if( Diag_state == DIAG_STATE_ACTIVE )
	{
		if(ct >= RxMsgBuffA[MSG_DIAG_RX_INDEX].timer)
		  tmp = ct - RxMsgBuffA[MSG_DIAG_RX_INDEX].timer;
		else
		  tmp = 0xFFFFFFFF - RxMsgBuffA[MSG_DIAG_RX_INDEX].timer + ct;
		//tmp =  ct - RxMsgBuffA[MSG_DIAG_RX_INDEX].timer;//20161116 xyl modify
		
		if(rx_state.st == kRxState_WaitCF)//xyl add 20151210
		{
		  if(tmp>DIAG_N_Cr_TIMEOUT_COUNT)
		  {
		    rx_state.st = kRxState_Idle; 
		  }
		}
    
    if(tx_state.st == kTxState_WaitFC)
    {
      if( tmp > DIAG_N_Bs_TIMEOUT_COUNT )
      {
        tx_state.st = kTxState_Idle;
      }    
    }
		
		if( tmp > DIAG_P3_TIMEOUT_COUNT )
		{
			Diag_InitTP();
		}
	}	
}








/*void memcpy(uint8_t *dest, uint8_t *source, uint16_t count)
{
   uint8_t* sd = dest;
   uint8_t* ss = source;
   while (count > 0)
   {
      *sd++ = *ss++;
      count--;
   }
}*/


void Diag_MsgIndication(void) 
{
    switch( diag_rx_buff[0] )//先不处理报文长度错误的问题
    {
      case SID_StartDiagnosticSession:          
      {//0x10  三种会话都需要实现，需要指示当前模式，服务在不同模式下响应可能不同      
      	/* Service ID session check if support in current session?  */
        //if ((refDescSvcHead->checkSessionState & dcm_CurrentState.stateSession) != 0)

        /* check the request data length, must at least the request header length */
        //gMSG_DIAG_RX.data[0]; //2
      	if( rx_state.data_length == 2 )
    		{
    			//Dcm_DiagSessionControl(diag_rx_buff[1]);//diagSessionType
        	switch( diag_rx_buff[1] )
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
    		}
    		else
        {
          Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
        }     
      	break;
      } 
       
      case SID_EcuReset:
    	{
        if( rx_state.data_length == 2 )
    		{    			
     	  	switch( diag_rx_buff[1] )
      		{
      		  case LID_HardReset:         //0X01
        		{
        			Dcm_ApplEcuHardReset();
        			break;
        		}
        		case LID_SoftReset:      	  //0x03
        		{
        			Dcm_ApplEcuSoftReset();
        			break;
        		}      		
        		default:
        		{
              Dcm_SetNegResponse(NEG_ACK_SUBFUNCTIONNOTSUPPORTED);
              break;
        		}
      		}
    		}
    		else
        {
          Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
        }
    		break;
    	}       
       
      case SID_ClearFaultMemory://1
    	{
    		if( rx_state.data_length == 4)
    		{
      	  Dcm_ApplClearFaultMemory();
    		}
    		else
      	{
    			Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
      	}
    		break;
    	}
    	
    	case SID_ReadDTCInfo:  //added by xyl 2014.10.13
    	{
        switch( diag_rx_buff[1] )
    		{
    			case LID_ReportNumOfDTC:
    			if( rx_state.data_length == 3)
      		{
		        Dcm_ApplReportNumberOfDTCByStatusMask(diag_rx_buff[2]); //DTCStsMask
      		}
      		else
      		{          		  
       			Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
       		}
    			break;
    			
    			case LID_ReportDTCByStatusMask:
     			if( rx_state.data_length == 3)
      		{
   			    Dcm_ApplReportDTCByStatusMask(diag_rx_buff[2]);     //DTCStsMask
      		}
      		else
      		{          		  
      			Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
      		}
    			break;
    			
    			case LID_ReadSupportedDTC:
      		if( rx_state.data_length == 2)
      		{
   			    Dcm_ApplReportSupportedDTC();
      		}
      		else
      		{          		  
      			Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
      		}
    			break;
    			
    			default:    			
    			{
            Dcm_SetNegResponse(NEG_ACK_SUBFUNCTIONNOTSUPPORTED);
            break;
      		}
    		}    	
    		break;
    	}
    	
    	/*case SID_ReadDTC:
    	{
    		switch( diag_rx_buff[1] )
    		{
    			case LID_RDTC_ActiveDTC:
    			Dcm_ApplReadActiveDTC();
    			break;
    			
    			case LID_RDTC_InactiveDTC:
    			Dcm_ApplReadInactiveDTC();
    			break;
    			
    			case LID_RDTC_AllDTC:
    			Dcm_ApplReadAllDTC();
    			break;
    			
    			default:
    			
    			break;
    		}
    	
    		break;
    	} */
    	/*case SID_ReadECUId:
    	{
    		//Dcm_ApplReadECUId(diag_rx_buff[1]);//MODIFIED by xyl 2014.11.13
    		Dcm_ApplReadECUId((diag_rx_buff[1])<<8 + diag_rx_buff[2]);
    		break;
    	} */
    	
    	case SID_SecurityAccess:
    	{
        /*if(MSG_DIAG_RX_FUN_ID == (uint16)real_id){
          tx_state.data_length = 0;//该服务不支持功能寻址
        }
        else*/
        {            
          switch( diag_rx_buff[1] )
      		{
      			case LID_SA_RequestSeed: 
      			{
      			  if( rx_state.data_length != 2 ){
          			Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
          		}
          		else{          		  
          		  Dcm_ApplRequestSeed();
          		}
      			  break;
      			}    			
      			case LID_SA_SendKey: 
      			{
      			  //key = *(uint32_t*)&diag_rx_buff[2];
        			if( rx_state.data_length != 4){
            			Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
            		}
            	else{
            		Dcm_ApplSendKey(*(uint16_t*)&diag_rx_buff[2]);//check key and send response
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
    	}
  		break;

    	case SID_ReadDataByLocalId://可以一个报文几个ID
    	{
    		//Dcm_ApplReadDataByLocalId(diag_rx_buff[1]);//MODIFIED by xyl 2014.11.13
    		if((rx_state.data_length >= 3) && ((rx_state.data_length%2)==1))
    		{
    		  Dcm_ApplReadDataByLocalId((uint16)((diag_rx_buff[1])<<8) + diag_rx_buff[2]);
    		}
    		else
    		{
          Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
    		}
    		break;
    	}
    	
    #if 1
    	case SID_WriteDataByLocalId:
    	{
    		if(MSG_DIAG_RX_FUN_ID == (uint16)real_id){
          tx_state.data_length = 0;//该服务不支持功能寻址
        }
        else
        {
          if( rx_state.data_length >= 4)
      		{
        		Dcm_ApplWriteDataByLocalId((uint16)((diag_rx_buff[1])<<8) + diag_rx_buff[2]);
      		}
      		else
      		{
            Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
      		}
        }
    		break;
    	}
    	
    	case SID_InputOutputControlByIdentifier:          //0x2F   
      {
      	if(MSG_DIAG_RX_FUN_ID == (uint16)real_id){
          tx_state.data_length = 0;//该服务不支持功能寻址
        }
        else
        { 
          if( rx_state.data_length >= 5)
      		{
        		Dcm_ApplIOCById((uint16)((diag_rx_buff[1])<<8) + diag_rx_buff[2], (rx_state.data_length-1), &diag_rx_buff[1] );      	
      		}
      		else
      		{
            Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
      		}
    		}
      	break;
      } 
      
      case SID_ControlDTCSetting:    //0x85
    	{
    		if( rx_state.data_length >= 2)
    		{
      		switch( diag_rx_buff[1] )
        	{
        		case LID_CDTCS_ON:         //0X00
        		{
        			Dcm_ApplControlDTCSettingOn();
        			break;
        		}
        		case LID_CDTCS_OFF:      	  //0x01
        		{
        			Dcm_ApplControlDTCSettingOff();
        			break;
        		}       		
        		default:
        		{
              Dcm_SetNegResponse(NEG_ACK_SUBFUNCTIONNOTSUPPORTED);
              break;
        		}
        	} 
    		}
    		else
      	{
      	  Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
        }
      	break;
    	}
    	
    	case SID_CommunicationControlService:    //0x28
    	{
    		if( rx_state.data_length == 3 )
    		{
      		switch( diag_rx_buff[1] )//control type
        	{
        	   //diag_rx_buff[2] 还有communication type
        		case LID_CCS_EnableRxAndTx:         //0X00
        		{
        			Dcm_ApplEnableRxAndTx( diag_rx_buff[2] );//commnunication type
        			break;
        		}
        		case LID_CCS_DisableRxAndTx:      	  //0x03
        		{
        			Dcm_ApplDisableRxAndTx( diag_rx_buff[2] );//commnunication type
        			break;
        		}       		
        		default:
        		{
              Dcm_SetNegResponse(NEG_ACK_SUBFUNCTIONNOTSUPPORTED);
              break;
        		}
        	} 
    		}
    		else
        {
          Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
        }
      	break;
    	}
    	
    	/*case SID_StartRoutineByLocalId:
    	{
    		//2013-02-18, need to receive parameters, par_size = (rx_state.data_length-2)
    		Dcm_ApplStartRoutineByLocalId( diag_rx_buff[1], (rx_state.data_length-2), &diag_rx_buff[2] );
    		break;
    	} 
    	case SID_StopRoutineByLocalId:
    	{
    		Dcm_ApplStopRoutineByLocalId( diag_rx_buff[1] );
    		break;
    	}    	
    	case SID_ReqRoutineResultByLocalId:
    	{
    		Dcm_ApplReqRoutineResultByLocalId( diag_rx_buff[1] );
    		break;
    	}*/
    	
    	case SID_RoutineControl:    //0x31   //可以一个报文几个ID
    	{
    		if(MSG_DIAG_RX_FUN_ID == (uint16)real_id){
          tx_state.data_length = 0;//该服务不支持功能寻址
        }
        else
        {
          if( rx_state.data_length >= 5)
      		{
      			switch( diag_rx_buff[1] )
          	{
          		case LID_RC_StartRoutine:         //0X01
          		{
             		//2013-02-18, need to receive parameters, par_size = (rx_state.data_length-2)
            		Dcm_ApplStartRoutineByLocalId(((diag_rx_buff[2])<<8)+diag_rx_buff[3], (rx_state.data_length-4), &diag_rx_buff[4] );
           			break;
          		}
          		case LID_RC_StopRoutine:      	  //0x02
          		{
            		Dcm_ApplStopRoutineByLocalId((uint16)((diag_rx_buff[2])<<8)+diag_rx_buff[3], (rx_state.data_length-4), &diag_rx_buff[4] );
          			break;
          		}
          		case LID_RC_ReqRoutineResult:      	  //0x03
          		{
            		Dcm_ApplReqRoutineResultByLocalId((uint16)((diag_rx_buff[2])<<8)+diag_rx_buff[3]);
          			break;
          		}       		
          		default:
          		{
                Dcm_SetNegResponse(NEG_ACK_SUBFUNCTIONNOTSUPPORTED);
                break;
          		}
        		}
      		}
      		else
      		{
      		  Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
          }
        }      	   		              	
      	break;
    	}
    	
    	case SID_RequestDownload:     //参数个数不定
    	{
    		if(MSG_DIAG_RX_FUN_ID == (uint16)real_id){
          tx_state.data_length = 0;//该服务不支持功能寻址
        }
        else
        {
          if( rx_state.data_length >= 5)
      		{
      		  Dcm_ApplRequestDownload();
      		}
      		else
      		{
            Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
      		}
        }
    		break;
    	}
    	
    	case SID_TransferData:
    	{
    		if(MSG_DIAG_RX_FUN_ID == (uint16)real_id){
          tx_state.data_length = 0;//该服务不支持功能寻址
        }
        else
        {
          if( rx_state.data_length >= 3)
      		{
       		  Dcm_ApplTransferData( diag_rx_buff[1], (rx_state.data_length-2), &diag_rx_buff[2] );
      		}
      		else
      		{
            Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
      		}
      	}
    		break;
    	}
    #endif	
    	case SID_RequestTransferExit:  //参数个数不定
    	{
    		if(MSG_DIAG_RX_FUN_ID == (uint16)real_id){
          tx_state.data_length = 0;//该服务不支持功能寻址
        }
        else
        {
          if( rx_state.data_length >= 2)
      		{
        		Dcm_ApplRequestTransferExit((rx_state.data_length-1), &diag_rx_buff[1] );
      		}
      		else
      		{
             Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
       		}
      	}
    		break;
    	}
   	
    	case SID_TesterPresent:     
    	{
    	  if( rx_state.data_length == 2 )
    		{
        	switch( diag_rx_buff[1] )
          {
        		case LID_TP_ZEROSUBFUNCTION:
        		{
        		  Dcm_DcmTesterPresent();
        		  break;
        		}
        		default:
        		{
        		  if(MSG_DIAG_RX_FUN_ID == (uint16)real_id){
                tx_state.data_length = 0;//该服务特殊，仅在是寻址方式是功能寻址，且子功能不正确的情况下不响应
              }
              else
              {
                Dcm_SetNegResponse(NEG_ACK_SUBFUNCTIONNOTSUPPORTED);
              } 
      	      break;
        		}
        	}
    		}
    		else
    		{
    		  Dcm_SetNegResponse(NEG_ACK_INCORRECTMESSAGELENGTH_OR_INVALIDFORMAT);
        }
    		break;
    	}
    	default: //Service not supported
    	{
        Dcm_SetNegResponse(NEG_ACK_SERVICENOTSUPPORTED);  	  	
    	}
    	break;
    }
  Diag_TPTransmit();    
}

