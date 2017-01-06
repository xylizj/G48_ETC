/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2010, All Rights Reserved  *
*************************************************************************

************************************************************************
*                                                                      *
*        Standard Software Flash Driver For S12G                       *
*                                                                      *
* FILE NAME     :  EEErase.c                                           *
* DATE          :  April 10,2010                                       *
*                                                                      *
* AUTHOR        :  FPT Team			                                   *
* E-mail        :  b28216@freescale.com                                *
*                                                                      *
************************************************************************/
/************************** CHANGES ***********************************
 0.1.0    04.10.2010	 FPT Team			Port to S12G from S12P SSD
 1.1.1	  04.15.2010	 FPT Team			Update check for valid range
***********************************************************************/

/* include the header files */
#include "SSD_Types.h"
#include "SSD_SGF18.h"
#include "SSD_SGF18_Internal.h"

/*************************************************************************
*
*  Function Name    : EEErase
*  Description      : Perform erase operation on EEPROM
*
*  Arguments        : FLASH_SSD_CONFIG *, UINT16. UINT16, pFLASHCOMMANDSEQUENCE
*  Return Value     : UINT16
*
**************************************************************************/


UINT16 SSD_SGF18_FAR EEErase(FLASH_SSD_CONFIG *SSD_SGF18_NEAR PSSDConfig, \
                                  UINT16 destination, UINT16 size,
                                  pFLASHCOMMANDSEQUENCE FlashCommandSequence)
{
    UINT16 commandArray[2];  /* command sequence array */
    UINT16 returnCode;       /* return code variable */
    UINT32 endAddress;       /* storing end address */

    /* set the default return code as SGF_OK */
    returnCode = SGF_OK;

    /* calculating EEPROM end address */
    endAddress = destination + size;

    /* checking size alignment (sector 4 Bytes) */
    if(0 != (size % EE_SECTOR_SIZE))
    {
        /* set the return code as SGF_ERR_SIZE */
        returnCode = SGF_ERR_SIZE;
    }
    /* range checking */
    else if((destination < EE_START_ADDR) || \
            (destination > EE_END_ADDR) || \
            (endAddress > EE_END_ADDR))
    {
        /* set the return code as SGF_ERR_RANGE */
        returnCode = SGF_ERR_RANGE;
    }
    else
    {
        /* preparing passing parameter for 4 Bytes sector erase */
        /* 1st element for the FCCOB register */
        /* set the command value for EEPROM erase */
        commandArray[0] = ((((UINT16)(EE_ERASE_SECTOR)) << 8)|GLOBAL_ADDRESS_EE);

        while(0 != size)
        {
            /* 2nd element fot the FCCOB register */
            commandArray[1] = destination;

            /* update size and destination address */
            size -= EE_SECTOR_SIZE;
            destination += EE_SECTOR_SIZE;

            /* calling flash command sequence function to execute the command */
            returnCode = FlashCommandSequence(PSSDConfig, COMMAND_INDEX1, \
                                              commandArray);

            /* checking the success of command execution */
            if(SGF_OK != returnCode)
            {
                break;
            }
            else
            {
                /* do nothing */
            }
        }
    }



    return(returnCode);
}


/* end of file */
