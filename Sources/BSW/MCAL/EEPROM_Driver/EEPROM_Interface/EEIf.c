#include "EEIf.h"


/* Initialize the flash parameter structure */
/* SSD configuration structure */
FLASH_SSD_CONFIG SSD_SGF18_NEAR flashSSDConfig =
{
    MCU_REGISTER_BASE,   /* base address of MCU register block */
    BUS_CLOCK_EE_USE,           /* target bus clock */
    IGNORE_SINGLE_FAULT, /* ignore single fault setting flag */
    //NULL_CALLBACK        /* pointer to callback function(Demo: NULL) */
};
FLASH_SSD_CONFIG * SSD_SGF18_NEAR PSSDConfig = &flashSSDConfig;



/*********************************************************************
*
*  Function Name    : FindFreeSpace
*  Description      : To find the address been written last time. 
*                                         
*  Arguments        : UINT16
*  Return Value     :
*
*********************************************************************/
UINT8 FindActiveUserSector(void)
{
  UINT16 addr;
  UINT8 sectorNum;
  
  addr = EE_START_ADDR;
  for(sectorNum=0; sectorNum<USER_DEF_SECTOR_NUM; sectorNum++)
  {
     if((UINT32)USER_DEF_ACTIVE_FLAG == READ32(addr))
      return sectorNum;
     addr += USER_DEF_SECTOR_SIZE;
  }
  
  return NO_ACTIVE_USER_SECTOR;
}

/*********************************************************************
*
*  Function Name    : FindFreeSpace
*  Description      : To find the address been written last time. 
*                                         
*  Arguments        : UINT16
*  Return Value     :
*
*********************************************************************/
UINT16 FindFreeSpace(UINT8 g_eeActiveSectorNum)
{
  UINT16 activeSectorStartAddr;
  UINT16 offset;
  
  activeSectorStartAddr = EE_START_ADDR+g_eeActiveSectorNum*USER_DEF_SECTOR_SIZE+8;
  offset = 0;
  while(offset<USER_DEF_SECTOR_SIZE)
  {
    if((UINT32)USER_DEF_FREESPACE_FLAG == READ32(activeSectorStartAddr+offset))
    {
      break;
    }
    offset += EE_SECTOR_SIZE;             
  }
  
  return (activeSectorStartAddr+offset);
}