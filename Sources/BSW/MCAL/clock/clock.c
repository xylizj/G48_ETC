#include "clock.h"
#include "derivative.h"      /* derivative-specific definitions */

void PLL_PEE_Init(void) 
{     
    /* CPMUPROT: PROT=0 Writing 0x26 to the CPMUPROT register clears the PROT bit, 
      other write accesses set the PROT bit.*/
    CPMUPROT = 0x26U;         /* Disable protection of clock configuration registers */
    /* CPMUCLKS: PSTP=0 */
    CPMUCLKS_PSTP = 0;
    /* CPMUCLKS: PLLSEL=1 */
    CPMUCLKS_PLLSEL = 1;/* Enable the PLL to allow write to divider registers */
    /* CPMUCLKS: RTIOSCSEL=1 RTI clock source is OSCCLK
                 RTIOSCSEL=0 RTI clock source is IRCCLK*/
    CPMUCLKS_RTIOSCSEL = 1;
    /* Set Freq = 8 MHz */
    CPMUREFDIV_REFDIV = 0;//OSC_FRQ_MHZ - 1; /* Set the divider register(Write only when PROT is 0,and PLLSEL is 1) */
    CPMUREFDIV_REFFRQ = 0b10;    
    /* Set Fvco is 2 * BUS_FRQ_MHZ */
    CPMUSYNR_SYNDIV = 2;//BUS_FRQ_MHZ - 1;   /* Set the multiplier register */
    CPMUSYNR_VCOFRQ = 0; /*0 For 32 MHz ~ 48 MHz*/
    
    /* Fpll = Fvco / 2 when PLL locked */
    CPMUPOSTDIV = 0x00U;
    
    CPMUOSC_OSCE = 1;                     /* Enable the oscillator */
    /* CPMUPLL: FM1=0,FM0=0 */
    CPMUPLL = 0x00U;                     /* Set the PLL frequency modulation(0 for FM off)*/
    while(0u == CPMUFLG_LOCK && 0u == CPMUFLG_UPOSC)
    {
        /* Wait until the oscillator is qualified by the PLL */
    }
    CPMUFLG = 0xFF;//Clear all flags in the CPMUFLG register to be able to detect any future status bit change
    /* CPMUPROT: PROT=0 */
    CPMUPROT = 0x00U;                    /* Enable protection of clock configuration registers */
    /* CPMUCOP: RSBCK=0,WRTMASK=0 */
    CPMUINT_OSCIE = 0;
    CPMUINT_LOCKIE = 0;
    CPMUINT_RTIE = 0;
}

//PLL_PEE_Init /* Set Freq = 8 MHz */
void RTI_Init(void)
{
  CPMURTI_RTDEC = 1;
  CPMURTI_RTR = 0b0000000;//1M/(1*1000)=8K
  CPMUINT_RTIE = 1;
}

/*#pragma CODE_SEG NON_BANKED
#pragma TRAP_PROC
volatile unsigned long int RTI_Cnt;

void near interrupt  
//VectorNumber_Vrti  
RTI1ISR(void)
{
   RTI_Cnt++;         
   CPMUFLG_RTIF = 1;
   if(0xFFFFFFFF <= RTI_Cnt) {
    RTI_Cnt = 0; 
   }
}
#pragma CODE_SEG DEFAULT
*/

void delay_us(unsigned int n) {
  while(n--){
    NOP_DELAY_1US();  
  }
}