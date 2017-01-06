/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2010, All Rights Reserved  *
*************************************************************************

************************************************************************
*                                                                      *
*        Standard Software Flash Driver For S12G                       *
*                                                                      *
* FILE NAME     :  EEEraseVerify.c                                     *
* DATE          :  April 10,2010                                       *
*                                                                      *
* AUTHOR        :  FPT Team			                                   *
* E-mail        :  b28216@freescale.com                                *
*                                                                      *
************************************************************************/
/************************** CHANGES ***********************************
 0.1.0    04.10.2010	 FPT Team			Port to S12G from S12P SSD
 1.1.1	  04.15.2010	 FPT Team			No change
***********************************************************************/

/* include the header files */
#include "SSD_Types.h"
#include "SSD_SGF18.h"
#include "SSD_SGF18_Internal.h"

/****************************************************************************
*
*  Function Name    : EEEraseVerify
*  Description      : Perform erase verify operation on EEPROM
*  Arguments        : FLASH_SSD_CONFIG *, UINT16, UINT16, pFLASHCOMMANDSEQUENCE
*  Return Value     : UINT16
*
*****************************************************************************/

UINT16 SSD_SGF18_FAR EEEraseVerify(FLASH_SSD_CONFIG * SSD_SGF18_NEAR PSSDConfig, \
                                        UINT16 destination, UINT16 size, \
                                        pFLASHCOMMANDSEQUENCE FlashCommandSequence)
{
    UINT8  index;           /* command index */
    UINT16 commandArray[3]; /* command sequence array */
    UINT16 commandValue;    /* store command value */
    UINT16 returnCode;      /* return code variable */
    UINT16 wordCount;       /* no. of words */

    /* set the default return code as SGF_OK */
    returnCode = SGF_OK;

    /* checking size alignment (section 2 Bytes) and correctness of size */
    if((0 != (size % HCS12G_WORD_SIZE)) || (size > EE_MAXIMUM_SIZE))
    {
        /* set the return code as SGF_ERR_SIZE */
        returnCode = SGF_ERR_SIZE;
    }
    /* checking size zero */
    else if(0 == size)
    {
        /* set the return code as SGF_OK */
        returnCode = SGF_OK;
    }
    else
    {
        /* checking for block erase verify */
        if((EE_START_ADDR == destination) && \
           ((EE_MAXIMUM_SIZE) == size))
        {
            /* preparing passing parameter for block erase verify */
            index = COMMAND_INDEX0;

            /* set the command value for EEPROM erase verify */
            commandValue = (((UINT16)(FLASH_ERASE_VERIFY_BLOCK)) << 8);
        }
        else
        {
            /* preparing passing parameter for section erase verify */
            index = COMMAND_INDEX2;
            wordCount = size / HCS12G_WORD_SIZE;

            /* set the command value for EEPROM erase verify */
            commandValue = (((UINT16)(EE_ERASE_VERIFY)) << 8);

            /* 2nd element for the FCCOB register */
            commandArray[1] = destination;
            /* 3rd element for the FCCOB register */
            commandArray[2] = wordCount;
        }

        /* 1st element for the FCCOB register */
        commandArray[0] = (commandValue|GLOBAL_ADDRESS_EE);

        /* calling flash command sequence function to execute the command */
        returnCode = FlashCommandSequence(PSSDConfig, index, commandArray);

    }


    return(returnCode);
}

/* end of file */