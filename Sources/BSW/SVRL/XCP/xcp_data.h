#ifndef __XCP_DATA_H_
#define __XCP_DATA_H_

#include "EEIf.h"


#define XCP_RAM_START_ADDR 0x3000
#define XCP_EEP_START_ADDR EE_START_ADDR
#define XCP_DATA_LEN       (UINT16)(0x0400-4) //max length, 4 bytes for checksum, 4 bytes aligned




extern void xcp_data_ram2eep(void);
extern UINT8 xcp_data_eep2ram(void);





















#endif/*__XCP_DATA_H_*/