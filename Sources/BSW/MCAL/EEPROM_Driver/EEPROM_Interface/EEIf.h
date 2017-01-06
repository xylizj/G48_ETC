#ifndef _EE_INTERFACE_H_
#define _EE_INTERFACE_H_


#include "SSD_Types.h"
#include "SSD_SGF18.h"
#include "SSD_SGF18_Internal.h"


/* Base address of MCU register block */
#define MCU_REGISTER_BASE               0x00000000

/* Bus clock. The unit is 10KHz */
#define BUS_CLOCK_EE_USE                       2400

/* Ignore Single Fault Flag */
#define IGNORE_SINGLE_FAULT             FALSE

#define TARGET_ADDRESS_OFFSET           0x00000000

//#define FLASH_SECTOR_SIZE               0x200 /* 512 B */



extern FLASH_SSD_CONFIG * SSD_SGF18_NEAR PSSDConfig;

extern UINT8 FindActiveUserSector(void);
extern UINT16 FindFreeSpace(UINT8 g_eeActiveSectorNum);

#endif