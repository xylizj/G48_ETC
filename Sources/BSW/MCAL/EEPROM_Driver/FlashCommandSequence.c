/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2010, All Rights Reserved  *
*************************************************************************

************************************************************************
*                                                                      *
*        Standard Software Flash Driver For S12G                       *
*                                                                      *
* FILE NAME     :  FlashCommandSequence.c                              *
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


/***********************************************************************
*
*  Function Name    : FlashCommandSequence
*  Description      : Perform command write sequence for flash operation
*  Arguments        : FLASH_SSD_CONFIG *, UINT, UINT16 *
*  Return Value     : UINT16
*
************************************************************************/

UINT16 SSD_SGF18_NEAR FlashCommandSequence(FLASH_SSD_CONFIG *SSD_SGF18_NEAR PSSDConfig, \
                                           UINT8 index, \
                                           UINT16 *SSD_SGF18_NEAR commandArray)
{
    UINT8  counter;          /* for loop counter variable */
    UINT8  registerValue;    /* store data read from flash register */
    UINT16 returnCode;       /* return code variable */

    /* set the default return as SGF_OK */
    returnCode = SGF_OK;

    /* check CCIF bit of the flash configuration register */
    while(FALSE == (REG_BIT_TEST((PSSDConfig->registerBase + \
                   FLASH_FSTAT_OFFSET), FLASH_FSTAT_CCIF)))
    {
        /* wait till CCIF bit is set */
    }

    /* clear ACCERR and FPVIOL bits */
    REG_WRITE((PSSDConfig->registerBase + FLASH_FSTAT_OFFSET), \
              (FLASH_FSTAT_ACCERR|FLASH_FSTAT_FPVIOL));

    /* load FCCOBIX & FCCOB register */
    for(counter=0; counter<=index; counter++)
    {
        REG_WRITE(PSSDConfig->registerBase + FLASH_FCCOBIX_OFFSET, counter);
        REG_WRITE16(PSSDConfig->registerBase + FLASH_FCCOB_OFFSET, \
                    commandArray[counter]);
    }

    /* clear CCIF bit */
    REG_WRITE(PSSDConfig->registerBase + FLASH_FSTAT_OFFSET, FLASH_FSTAT_CCIF);

    /* check CCIF bit */
    while(FALSE == (REG_BIT_TEST((PSSDConfig->registerBase + \
                   FLASH_FSTAT_OFFSET), FLASH_FSTAT_CCIF)))
    {
        /* wait till CCIF bit is set */
        /* serve callback function if counter reaches limitation */
        //if(NULL_CALLBACK != PSSDConfig->CallBack)
        {
            //(PSSDConfig->CallBack)();
        }
        //else
        {
            /* do nothing */
        }
    }

    /* Check error bits */
    /*Get flash status register value */
    registerValue = REG_READ(PSSDConfig->registerBase + FLASH_FSTAT_OFFSET);

    /* checking access error */
    if(0 != (registerValue & FLASH_FSTAT_ACCERR))
    {
        /* return an error code SGF_ERR_ACCERR */
        returnCode = SGF_ERR_ACCERR;
    }
    /* checking protection error */
    else if(0 != (registerValue & FLASH_FSTAT_FPVIOL))
    {
        /* return an error code SGF_ERR_PVIOL */
        returnCode = SGF_ERR_PVIOL;
    }
    /* checking MGSTAT1 error */
    else if(0 != (registerValue & FLASH_FSTAT_MGSTAT1))
    {
        /* return an error code SGF_ERR_MGSTAT1 */
        returnCode = SGF_ERR_MGSTAT1;
    }
    /* checking MGSTAT0 non-correctable error */
    else if(0 != (registerValue & FLASH_FSTAT_MGSTAT0))
    {
        /* return an error code SGF_ERR_MGSTAT0 */
        returnCode = SGF_ERR_MGSTAT0;
    }
    else
    {
        /* do nothing*/
    }

    return(returnCode);
}


/* end of file */



