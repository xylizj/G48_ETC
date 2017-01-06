/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2010, All Rights Reserved  *
*************************************************************************

************************************************************************
*                                                                      *
*        Standard Software Flash Driver For S12G                       *
*                                                                      *
* FILE NAME     :  FlashInit.c                                         *
* DATE          :  April 10,2010                                       *
*                                                                      *
* AUTHOR        :  FPT Team			                                   *
* E-mail        :  b28216@freescale.com                                *
*                                                                      *
************************************************************************/
/************************** CHANGES ***********************************
 0.1.0    04.10.2010	 FPT Team			Port to S12G from S12P SSD
 1.1.1	  04.15.2010	 FPT Team			No change
 1.1.2    09.13.2012     FPT Team           Use MAX_CLOCK_VALUE macro 
                                            for checking valid clock range.
***********************************************************************/

/* include the header files */
#include "SSD_Types.h"
#include "SSD_SGF18.h"
#include "SSD_SGF18_Internal.h"



/*********************************************************************
*
*  Function Name    : FlashInit
*  Description      : Initialize the Flash memory.
*  Arguments        : FLASH_SSD_CONFIG *
*  Return Value     : UNIT16
*
**********************************************************************/

UINT16 SSD_SGF18_FAR FlashInit (FLASH_SSD_CONFIG *SSD_SGF18_NEAR PSSDConfig)
{
    UINT16  flashDivider;    /* flash clock divider */
    UINT8  registerValue;   /* store data read from flash register */
    UINT16 clockValue;      /* clock in MHz */
    UINT16 returnCode;      /* return code variable */

    /* set the default return code as SGF_OK */
    returnCode = SGF_OK;

    /* clear ACCERR & FPVIOL flag in flash status register */
    REG_WRITE((PSSDConfig->registerBase + FLASH_FSTAT_OFFSET), \
              (FLASH_FSTAT_ACCERR | FLASH_FSTAT_FPVIOL));

    /* clear DFDIF & SFDIF flag in flash error status register */
    REG_WRITE((PSSDConfig->registerBase + FLASH_FERSTAT_OFFSET), \
              (FLASH_FERSTAT_DFDIF | FLASH_FERSTAT_SFDIF));

    /* set the ignore single bit value */
    if(FALSE == PSSDConfig->ignoreSingleFault)
    {
        /* clear IGNSF bit in the FCNFG register to report a
        single bit faults during array reads */
        REG_BIT_CLEAR((PSSDConfig->registerBase + FLASH_FCNFG_OFFSET), \
                       FLASH_FCNFG_IGNSF);
    }
    else
    {
        /* set IGNSF bit in the FCNFG register to ignore single bit faults */
        REG_BIT_SET((PSSDConfig->registerBase + FLASH_FCNFG_OFFSET), \
                     FLASH_FCNFG_IGNSF);
    }

    /*Get flash clock divider register value */
    registerValue = REG_READ(PSSDConfig->registerBase + FLASH_FCLKDIV_OFFSET);

    /* check the FDIVLD bit in the FCLKDIV register is set */
    if(FALSE == (registerValue & FLASH_FCLKDIV_FDIVLD))
    {
                            /* bus clock in MHz */
        clockValue = PSSDConfig->busClock/100;
            /* calculating flash divider value */
        if((clockValue >= 1) && (clockValue <= MAX_CLOCK_VALUE))
            {
                flashDivider = clockValue - 1;
            }
        else
            {
                /* if bus clock is less than 1MHz or greater than 32MHz,
                   return an invalid clk error code SGF_ERR_INVALIDCLK */
                returnCode = SGF_ERR_INVALIDCLK;
            }
      

        if(SGF_OK == returnCode )
        {
            /* write the flashDivider value into the FCLKDIV register */
            REG_WRITE((PSSDConfig->registerBase + FLASH_FCLKDIV_OFFSET), \
                      (UINT8)flashDivider);
                            /* If S12P family */           
                /* write the flashDivider value into the FCLKDIV register */
                REG_BIT_SET((PSSDConfig->registerBase + FLASH_FCLKDIV_OFFSET), \
                             FLASH_FCLKDIV_FDIVLCK);          
        }
    }
    else
    {
        /* clock set : do nothing */
    }


    return(returnCode);
}


/* end of file */