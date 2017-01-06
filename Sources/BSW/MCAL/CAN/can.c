#include "can.h"
#include "can_app.h"

#if  DIAG_ENABLE > 0
#include "diag.h"
#endif

#define TX_SYS_STATUS_ID     0x18FF21FF
#define TX_ETC_INFO_ID       0x0CFF0546

  
#define XCP_CAN_ENABLE 0

#if  XCP_CAN_ENABLE > 0
/* CAN ID of CMD CTO packets */
/* Maybe not used, if Rx Indication is specfic for CMD_CTO */
#define XCP_CAN_ID_CMD_CTO                      0x18FFFEFF
 
/* CAN ID of RES/ERR/EV/SERV CTO packets */
#define XCP_CAN_ID_RES_CTO                      0x18FFFFFF
#endif



extern void Xcp_CanIfRxIndication(uint8_t* buff, uint8_t length);
extern uint8_t Xcp_CanIfTransmit(uint8_t* buff, uint8_t length);
/* this function should be called after one XCP message sent out successfully */
extern void __near Xcp_CanIfTxConfirmation( void );



#if  XCP_CAN_ENABLE > 0
uint8_t Xcp_Msg_Rx_buf[8];
uint8_t Xcp_Msg_Tx_buf[8];

void MsgXCP_RX_Decode(void)  
{
  Xcp_CanIfRxIndication(can_msgs_buf_array[XCP_RX_CMD].pbuffer, can_msgs_buf_array[XCP_RX_CMD].dlc);
}


uint8_t Xcp_CanIfTransmit(uint8_t* buff, uint8_t length)
{
  memcpy(can_msgs_buf_array[XCP_TX_RESP].pbuffer, buff, length);
  can_msgs_buf_array[XCP_TX_RESP].dlc = length;
  
  //return CAN_Tx(XCP_TX_RESP);
  
  CAN_Tx(XCP_TX_RESP);
  
  return IO_ERR_OK;
}
#endif



const can_msg_config_t can_msgs_cfg_array[] = 
{ 
  {TX_SYS_STATUS_ID, TRUE, TRUE, CAN_Send_SysStatus},  /*TX_SYS_STATUS_SEND*/
  {TX_ETC_INFO_ID, TRUE, TRUE, CAN_Send_EtcInfo},  /*TX_SYS_STATUS_SEND*/
#if  XCP_CAN_ENABLE > 0
  {XCP_CAN_ID_CMD_CTO, TRUE/*Extended ID*/, FALSE, MsgXCP_RX_Decode},//XCP_RX_CMD
  {XCP_CAN_ID_RES_CTO, TRUE/*Extended ID*/, TRUE, NULL},//XCP_TX_RESP
#endif
};

can_msg_buf_cfg_t can_msgs_buf_array[] =
{
  {0, 8, SysStatus_Send_buf,10, 0},        
  {0, 8, EtcInfo_Send_buf,1, 0},
#if  XCP_CAN_ENABLE > 0
  {CAN_MSG_EMPTY, 8, Xcp_Msg_Rx_buf, 0, 0},  //XCP_RX_CMD    event
  {CAN_MSG_EMPTY, 8, Xcp_Msg_Tx_buf, 0, 0},  //XCP_TX_RESP    event
#endif
};




io_err_t CAN_Init(void)
{
    CAN_REG(CTL1_CANE) = 1;  /*Enable MSCAN*/

    CAN_REG(CTL0_INITRQ) = 1; /*Request initial module CANCTL0_INITRQ*/    
    while(!CAN_REG(CTL1_INITAK));/*wait for enter intitial module mode*/
    
    CAN_REG(CTL1_CLKSRC) = 0; /*0 for osc clk(here is 4M),1 for bus clk*/
    CAN_REG(CTL1_LISTEN) = 0; /*don't used listen mode*/
    CAN_REG(CTL1_LOOPB)  = 0; /*self test disable*/
    CAN_REG(CTL1_BORM)   = 0; /*enable automatic bus-off recovery*/

    CAN_REG(BTR0_BRP)     = 1;  /*Prescale value = BRP + 1*/
    CAN_REG(BTR0_SJW)     = 3;  /*eg. 2: Resync Jump Width = (2 + 1) * Tq = 375 ns*/
    CAN_REG(BTR1_TSEG_10) = 10; /*eg. Tseg1 = Tq * (TSEG_1 + 1) = 1500 ns,81.25% if TSEG_10 = 10 and TSEG_20 = 3, 75%*/
    CAN_REG(BTR1_TSEG_20) = 3;  /*eg. Tseg2 = Tq * (TSEG_1 + 1) = 375 ns*/
    //(10+1)+(3+1)+1=16, 4M/16=250K
    CAN_REG(BTR1_SAMP)    = 0;  /*1 for Three samples per bit, else One*/

    CAN_REG(IDAC_IDAM)   = 1; /*0 for Two 32-bit acceptance filters,but not mask any message*/
    CAN_REG(IDAR0) = 0x00;
    CAN_REG(IDAR1) = 0x00;
    CAN_REG(IDAR2) = 0x00;
    CAN_REG(IDAR3) = 0x00;
    CAN_REG(IDAR4) = 0x00;
    CAN_REG(IDAR5) = 0x00;
    CAN_REG(IDAR6) = 0x00;
    CAN_REG(IDAR7) = 0x00;
    CAN_REG(IDMR0) = 0xFF;
    CAN_REG(IDMR1) = 0xFF;
    CAN_REG(IDMR2) = 0xFF;
    CAN_REG(IDMR3) = 0xFF;
    CAN_REG(IDMR4) = 0xFF;
    CAN_REG(IDMR5) = 0xFF;
    CAN_REG(IDMR6) = 0xFF;
    CAN_REG(IDMR7) = 0xFF;

    /*Exit initial module mode*/
    CAN_REG(CTL0_INITRQ) = 0;             
    while(CAN_REG(CTL1_INITAK));/*wait for exit intitial module mode*/
    
    CAN_REG(RFLG_RXF) = 1;   /*Clear rx full flag*/
    CAN_REG(RIER_RXFIE) = 0; /*Disable rx interrupt*/
    CAN_REG(TIER_TXEIE) = 0; /*Disable tx interrupts,but enabled when start tx*/
    /*Enable CAN error and bus-off Interrupt (When Enter or Leave Thoese Error, Interrupt occur)*/
    //CAN_REG(RIER_RSTATE) = 2; /*1 for bus-off only, 2 for Receive-error and bus-off*/
    //CAN_REG(RIER_TSTATE) = 2;
    //CAN_REG(RIER_CSCIE) = 1;
    return IO_ERR_OK;
}






io_err_t CAN_Tx(can_msg_t msg_index)
{
    uint8_t msg_buffer_index = 0;
    uint8_t idle_buf = 0;

    idle_buf = CAN_REG(TFLG_TXE);
    /*check tx buffer only*/
    if (0 == idle_buf)
    {
        if (CAN_MSG_EMPTY_CHECK(msg_index))
        {
            can_msgs_buf_array[msg_index].msg_st = CAN_MSG_NEW_MASK;
        }
        else
        {
            return IO_ERR_BUSY;
        }
    }                                      
    else
    {        
        CAN_REG(TBSEL_TX) = idle_buf; /*The lowest numbered bit places the respective transmit buffer in the CANTXFG register space*/
         /*set ID*/
        if (TRUE == can_msgs_cfg_array[msg_index].is_ext_id)
        {
            /*data frame*/
            *((uint32_t *)(&CAN_REG(TXIDR0))) = (((can_msgs_cfg_array[msg_index].id & 0x1FFC0000) << 3) |0x00180000|((can_msgs_cfg_array[msg_index].id & 0x0003FFFF)<<1));
        }                    
        else
        {
            /*Data frame*/
            *((uint16_t *)(&CAN_REG(TXIDR0))) = (uint16_t)(can_msgs_cfg_array[msg_index].id << 5);
        }              
     
        /*load buffer*/
        if (can_msgs_cfg_array[msg_index].process_fun != NULL)
        {
            can_msgs_cfg_array[msg_index].process_fun();
        } 
        
        CAN_REG(TXDLR) = can_msgs_buf_array[msg_index].dlc;
        for (msg_buffer_index = 0; msg_buffer_index < can_msgs_buf_array[msg_index].dlc; msg_buffer_index++)
        {
            *((uint8_t *)(&CAN_REG(TXDSR0) + msg_buffer_index)) = can_msgs_buf_array[msg_index].pbuffer[msg_buffer_index];
        }
        
        can_msgs_buf_array[msg_index].msg_st = CAN_MSG_EMPTY;
         
        /*Start transmit message*/
        CAN_REG(TFLG_TXE) = 1;
        //CAN_REG(TIER_TXEIE) = 1;
    }
   
    return IO_ERR_OK;
}

/*
 * Function:CAN_TxCheck
 * Param<void>:
 * Return<void>:
 * REQ IDs:TPMS_SW_BASE_0059
 * Discription:
 * Note: 
*/
void CAN_TxCheck(void)
{
    can_msg_t msg_index;
    
    for (msg_index = 0; msg_index < CAN_MSGS_COUNT; msg_index++)
    {
        if ((TRUE == can_msgs_cfg_array[msg_index].is_tx) && (can_msgs_buf_array[msg_index].msg_st != CAN_MSG_EMPTY))
        {
            (void)CAN_Tx(msg_index);
        }
    }
}

/*
 * Function:CAN_PeriodHandle
 * Param<BOOL tx_rx>:TRUE for handle TX else handle RX
 * Return<void>:
 * REQ IDs:TPMS_SW_BASE_0457
 * Discription:
 * Note: 
*/
void CAN_PeriodHandle(BOOL tx_rx)
{
    can_msg_t msg_index;
    
    for (msg_index = 0; msg_index < CAN_MSGS_COUNT; msg_index++)
    {
      if (can_msgs_buf_array[msg_index].period > 0)
      {
          if (tx_rx == can_msgs_cfg_array[msg_index].is_tx)
          {
              if (can_msgs_buf_array[msg_index].tick > 0)
              {
                  can_msgs_buf_array[msg_index].tick--;
              }

              if (0 == can_msgs_buf_array[msg_index].tick)
              {
                  can_msgs_buf_array[msg_index].tick = can_msgs_buf_array[msg_index].period;
                  if (IO_ERR_OK == CAN_Tx(msg_index))
                  {
                      CAN_MSG_TIMEOUT_CLEAR(msg_index);
                  }
                  else
                  {
                      CAN_MSG_TIMEOUT_SET(msg_index);
                  }
              }
          }
          else
          {
              if (0 != CAN_MSG_CHECK(msg_index))
              {
                  can_msgs_buf_array[msg_index].tick = can_msgs_buf_array[msg_index].period;
                  CAN_MSG_TIMEOUT_CLEAR(msg_index);
              }
              else
              {
                  if (can_msgs_buf_array[msg_index].tick > 0)
                  {
                      can_msgs_buf_array[msg_index].tick--;
                  }
                  if (0 == can_msgs_buf_array[msg_index].tick)
                  {
                      can_msgs_buf_array[msg_index].tick = can_msgs_buf_array[msg_index].period;
                      CAN_MSG_TIMEOUT_SET(msg_index);
                  }
              }
          }
      }
    }
}

/*
 * Function:CAN_Rx
 * Param<void>:
 * Return<void>:
 * REQ IDs:TPMS_SW_BASE_0052,TPMS_SW_BASE_0053,TPMS_SW_BASE_0054,TPMS_SW_BASE_0055,TPMS_SW_BASE_0056,
 *         TPMS_SW_BASE_0057,TPMS_SW_BASE_0058
 * Discription: Receive all message from rx-buffer.
 *              If ID match, store message in corresponding message buffer.
 *              Only check messages in can_msgs_cfg_array, other messages ignore.
 * Note: 
*/
uint32_t real_id = 0;

void CAN_Rx(void)
{
  uint8_t  msg_index;
  uint8_t  dt_index;
  uint32_t id = 0;
  bool     ext_id = FALSE;

  while (1 == CAN_REG(RFLG_RXF))
  {
      ext_id = CAN_REG(RXIDR1_IDE);
      if (TRUE == ext_id) /*CAN ID type match*/
      {
          id = *((uint32_t *)(uint16_t)(&CAN_REG(RXIDR0))), id = ((id >> 1) & 0x0003FFFF)|((id >> 3) & 0x1FFC0000);
      }
      else
      {
          id = *((uint16_t *)(uint16_t)(&CAN_REG(RXIDR0))) >> 5;
      }
      real_id = id;
      /*Search ID config array*/
      for (msg_index = 0; msg_index < CAN_MSGS_COUNT; msg_index++)
      {
          if ((FALSE == can_msgs_cfg_array[msg_index].is_tx) && (can_msgs_cfg_array[msg_index].is_ext_id == ext_id) && (can_msgs_cfg_array[msg_index].id == id))
          {
              can_msgs_buf_array[msg_index].dlc = CAN_REG(RXDLR) & 0x0F;
              for (dt_index = 0; dt_index < can_msgs_buf_array[msg_index].dlc; dt_index++)
              {
                  can_msgs_buf_array[msg_index].pbuffer[dt_index] = *((uint8_t *)(&CAN_REG(RXDSR0) + dt_index));
              }
              
              if (can_msgs_cfg_array[msg_index].process_fun != NULL)
              {
                  can_msgs_cfg_array[msg_index].process_fun();
              }
              
              if (0 != (can_msgs_buf_array[msg_index].msg_st & CAN_MSG_NEW_MASK))
              {
                  can_msgs_buf_array[msg_index].msg_st = CAN_MSG_NEW_OVER;
              }
              else
              {
                  can_msgs_buf_array[msg_index].msg_st = CAN_MSG_NEW_MASK;
              }
          }
      }
      CAN_REG(RFLG_RXF) = 1;
  }
}



//!<===========================================================================================
//!<  Add a interface for J1939, so 
#include "J1939_Ext.h"
#pragma INLINE
void CAN_A_Receive(void){
  uint32_t id = 0;
  uint8_t  i;
  uint8_t  dt_index;
  bool     ext_id = FALSE;
  
  while (1 == CAN_REG(RFLG_RXF)){
      ext_id = CAN_REG(RXIDR1_IDE);
      if (TRUE == ext_id) /*CAN ID type match*/
      {
          id = *((uint32_t *)(uint16_t)(&CAN_REG(RXIDR0)));
          id = ((id >> 1) & 0x0003FFFF)|((id >> 3) & 0x1FFC0000);
      }
      else
      {
          id = *((uint16_t *)(uint16_t)(&CAN_REG(RXIDR0))) >> 5;
      }
      real_id = id;
      /*Search ID config array*/
      for (i = 0; i < CANA_RX_MSG_NUM; i++){
        if(RxMsgBuffA[i].id == id){
          RxMsgBuffA[i].dl = CAN_REG(RXDLR) & 0x0F;
          for (dt_index = 0; dt_index < RxMsgBuffA[i].dl; dt_index++)
          {
              *(((uint8_t *)&RxMsgBuffA[i].data[0])+dt_index) = *((uint8_t *)(&CAN_REG(RXDSR0) + dt_index));
          }
  	    	RxMsgBuffA[i].timer = read_tbl();
	        if( RxMsgBuffA[i].decode_func != NULL )
	        {
	        	RxMsgBuffA[i].decode_func();
	        }

        }
      }
      
     CAN_REG(RFLG_RXF) = 1; 
  }
}


uint8_t CAN_A_Transmit(uint32_t id, uint8_t* data, uint8_t length) { 
    uint8_t idle_buf = 0;     
    uint8_t msg_buffer_index;
    uint16_t tx_cnt;
    
    idle_buf = CAN_REG(TFLG_TXE);
    //idle_buf = idle_buf & 0b001;//20170104 xyl
    
    if (0 == idle_buf){
      return 1;  
    }

    CAN_REG(TBSEL_TX) = idle_buf; /*The lowest numbered bit places the respective transmit buffer in the CANTXFG register space*/
    idle_buf = CAN_REG(TBSEL_TX);
    //idle_buf = idle_buf & 0b001;//20170104 xyl
    
    *((uint32_t *)(&CAN_REG(TXIDR0))) = (((id & 0x1FFC0000) << 3) |0x00180000|((id & 0x0003FFFF)<<1));
    
    CAN_REG(TXDLR) = length;
    for (msg_buffer_index = 0; msg_buffer_index < length; msg_buffer_index++)
    {
        *((uint8_t *)(&CAN_REG(TXDSR0) + msg_buffer_index)) = data[msg_buffer_index];
    }       
         
    if(id == 0x18FFFFFF)//for XCP Xcp_CanIfTxConfirmation
      CAN_REG(TIER_TXEIE) = 1;
    else 
      CAN_REG(TIER_TXEIE) = 0;
     
    /*Start transmit message*/
    CAN_REG(TFLG_TXE) = idle_buf;
    
    //tx_cnt = 10000;
    //while ( ((CANTFLG & idle_buf) != idle_buf) && tx_cnt)
            //;  /* Wait for Transmission completion */
    
    return 0;
}


#pragma CODE_SEG NON_BANKED
/*
 * Function:can_tx_isr
 * Param<void>:
 * Return<interrupt void>:
 * REQ IDs:TPMS_SW_BASE_0378(TBD),
 * Discription:
 * Note: 
*/
interrupt void can_tx_isr(void)
{
  CAN_REG(TIER_TXEIE) = 0;
  //CAN_REG(TFLG_TXE) = CANTFLG_TXE_MASK;
  Xcp_CanIfTxConfirmation();
}

#pragma CODE_SEG DEFAULT