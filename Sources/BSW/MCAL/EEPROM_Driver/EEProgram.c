/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2010, All Rights Reserved  *
*************************************************************************

************************************************************************
*                                                                      *
*        Standard Software Flash Driver For S12G                       *
*                                                                      *
* FILE NAME     :  EEProgram.c                                         *
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


/************************************************************************
*
*  Function Name    : EEProgram.c
*  Description      : Program data into EEPROM
*  Arguments        : FLASH_SSD_CONFIG *, UINT16, UINT32, UINT32,
*                     pFLASHCOMMANDSEQUENCE
*  Return Value     : UINT16
*
*************************************************************************/

UINT16 SSD_SGF18_FAR EEProgram(FLASH_SSD_CONFIG *SSD_SGF18_NEAR PSSDConfig, \
                                    UINT16 dest, UINT16 size, UINT32 source,
                                    pFLASHCOMMANDSEQUENCE FlashCommandSequence)

{
    UINT8  counter;         /* outer loop counter */
    UINT8  index;           /* command index */
    UINT8  loopIndex;       /* inner loop index */
    UINT16 commandArray[6]; /* command sequence array */
    UINT16 returnCode;      /* return code variable */
    UINT32 endAddress;      /* end address for the program operation */

    /* set the default return code as SGF_OK */
    returnCode = SGF_OK;

    /* check the size alignment */
    if (0 != size % HCS12G_WORD_SIZE)
    {
        /* return an error code SGF_ERR_SIZE */
        returnCode = SGF_ERR_SIZE;
    }
    else
    {
        /* calculate end address */
        endAddress = dest + size;

        /* check the range of the address */
        if((dest < EE_START_ADDR) || \
            (dest > EE_END_ADDR) || \
            (endAddress > EE_END_ADDR))
        {
            /* set the return code as SGF_ERR_RANGE */
            returnCode = SGF_ERR_RANGE;
        }
        else
        {
            /* 1st element for the FCCOB register */
            /* set the command value for EEPROM program */
            commandArray[0] = ((((UINT16)(EE_PROGRAM)) << 8)|GLOBAL_ADDRESS_EE);

            while (size>0)
            {
                /* 2nd element fot the FCCOB register */
                commandArray[1] = dest;

                /* setting the loopindex based on the size passed */
                if(size > FLASH_PHRASE_SIZE)
                {
                    loopIndex = COMMAND_INDEX4;
                }
                else
                {
                    loopIndex = (UINT8)(size/HCS12G_WORD_SIZE);
                }

                /* preparing rest of the parameters for FCCOB register */
                for (counter = 0; counter < loopIndex; counter++)
                {
                    /* assigning the command arrat with the word to be
                    programmed */
                    commandArray[counter + 2] = READ16(source);

                    /*updating the destination, source and size */
                    dest += HCS12G_WORD_SIZE;
                    source += HCS12G_WORD_SIZE;
                    size -= HCS12G_WORD_SIZE;
                }

                /* value of FCCOBIX at command launch */
                index = loopIndex + 1; /* value of FCCOBIX at command launch */

                /* calling flash command sequence API to execute the command */
                returnCode = FlashCommandSequence(PSSDConfig, index, \
                                                  commandArray);
                /* checking for the success of command execution */
                if(SGF_OK != returnCode)
                {
                    break;
                }
                else
                {
                    /* do nothing */
                }
            }
            /* end of while loop */
        }

    }



    return(returnCode);
}


/* end of file */
