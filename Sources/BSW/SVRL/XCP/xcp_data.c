#include "derivative.h" /* include peripheral declarations */
#include <hidef.h>      /* common defines and macros */
#include "xcp_data.h"



#pragma CONST_SEG CAL_RAM
const unsigned int test_eep_cal = 0x2345;
const volatile unsigned char pwm_duty_threshold = 20;
#pragma CONST_SEG DEFAULT

void xcp_data_ram2eep(void) {
  unsigned int addr_offset;
  unsigned long int chksum_save;
  
  addr_offset = 0;
  chksum_save = 0;
  while(addr_offset < XCP_DATA_LEN){        
    chksum_save += *((UINT32 *)(XCP_RAM_START_ADDR+addr_offset));
    addr_offset += 4;
  }
  *((UINT32 *)(XCP_RAM_START_ADDR+XCP_DATA_LEN)) = chksum_save;

  DisableInterrupts;
  (void)EEErase(PSSDConfig, XCP_EEP_START_ADDR, (XCP_DATA_LEN+4), FlashCommandSequence);      
  (void)EEProgram(PSSDConfig, \
                             XCP_EEP_START_ADDR, \
                             (XCP_DATA_LEN+4), \
                             (UINT32)XCP_RAM_START_ADDR, \
                             FlashCommandSequence);  
  EnableInterrupts;                             
}

/*void xcp_data_ram2eep(UINT16 xcp_eep_start_addr, UINT16 xcp_data_len, UINT32 xcp_ram_start_addr){
  unsigned int addr_offset;
  unsigned long int chksum_save;
  
  addr_offset = 0;
  chksum_save = 0;
  while(addr_offset < XCP_DATA_LEN){        
    chksum_save += *((UINT32 *)(XCP_RAM_START_ADDR+addr_offset));
    addr_offset += 4;
  }
  *((UINT32 *)(XCP_RAM_START_ADDR+XCP_DATA_LEN)) = chksum_save;

  (void)EEProgram(PSSDConfig, \
                             xcp_eep_start_addr, \
                             (xcp_data_len+4), \
                             (UINT32)xcp_ram_start_addr, \
                             FlashCommandSequence);                               
}*/


UINT8 xcp_data_eep2ram(void) {
  unsigned int offset;
  unsigned long int chksum_read;
  unsigned long int chksum_cal;
  
  offset = 0;
  while(offset < XCP_DATA_LEN){    
    *((UINT32 *)(XCP_RAM_START_ADDR+offset)) = READ32(XCP_EEP_START_ADDR+offset);
    offset += 4;
  }

  chksum_read = READ32(XCP_EEP_START_ADDR+XCP_DATA_LEN);

  offset = 0;
  chksum_cal = 0;
  while(offset < XCP_DATA_LEN){        
    chksum_cal += *((UINT32 *)(XCP_RAM_START_ADDR+offset));
    offset += 4;
  }
  
  if(chksum_cal == chksum_read){
    return 0;
  }else{
    return 1; 
  }            
} 



