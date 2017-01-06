#include <hidef.h>      /* common defines and macros */
#include "AppInterface.h"


extern void __near _Startup(void);            /* Forward declaration of external startup function declared in file Start12.c */
extern void __near interrupt TOI_ISR(void);
extern void __near interrupt IC2_ISR(void);


#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void Cpu_Interrupt(void)
{
  /*lint -save -e950 Disable MISRA rule (1.1) checking. */
  asm(BGND);
  /*lint -restore Enable MISRA rule (1.1) checking. */
}
#pragma CODE_SEG DEFAULT




#pragma CONST_SEG APPREG_DATA
volatile const AppRegisterData AppRegisterTable = 
{  
    _Startup,
    (unsigned int)0x0030,                 //BCD, App SW version.
    (unsigned int)0x0010,                 //BCD, ECU HW version.
    (unsigned int)0x2016,                 //BCD, Release year.
    (unsigned int)0x1117,                 //BCD, Release month and day.
  
    'P','E','M','S','0','0', 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, //ECU mode
    '2','0','1','6','1','0','2','6','0','0', 0,  0,  0,  0,  0,  0, //HW serial number
    (unsigned int)0xffff,
    (unsigned int)0xffff,
    (unsigned int)0xffff,
    (unsigned int)0xffff,
    (unsigned int)0xffff
};
#pragma CONST_SEG DEFAULT

#define BOOTLOADER_SUPPORT 1//

#if BOOTLOADER_SUPPORT > 0
#pragma CONST_SEG APP_VECTOR
const tIsrFunc AppVectorTable[] = { /* Interrupt vector table */
#else
const tIsrFunc AppVectorTable[] @0xFF80U = { /* Interrupt vector table */
#endif
/*lint -restore Enable MISRA rule (1.1) checking. */
  /* ISR name                               No.  Address  Name          Description */
  &Cpu_Interrupt,                       /* 0x40  0xFF80   ivVsi         unused by PE */
  &Cpu_Interrupt,                       /* 0x41  0xFF82   ivVportad     unused by PE */
  &Cpu_Interrupt,                       /* 0x42  0xFF84   ivVatdcompare unused by PE */
  &Cpu_Interrupt,                       /* 0x43  0xFF86   ivVReserved60 unused by PE */
  &Cpu_Interrupt,                       /* 0x44  0xFF88   ivVapi        unused by PE */
  &Cpu_Interrupt,                       /* 0x45  0xFF8A   ivVlvi        unused by PE */
  &Cpu_Interrupt,                       /* 0x46  0xFF8C   ivVReserved57 unused by PE */
  &Cpu_Interrupt,                       /* 0x47  0xFF8E   ivVportp      unused by PE */
  &Cpu_Interrupt,                       /* 0x48  0xFF90   ivVReserved55 unused by PE */
  &Cpu_Interrupt,                       /* 0x49  0xFF92   ivVReserved54 unused by PE */
  &Cpu_Interrupt,                       /* 0x4A  0xFF94   ivVReserved53 unused by PE */
  &Cpu_Interrupt,                       /* 0x4B  0xFF96   ivVReserved52 unused by PE */
  &Cpu_Interrupt,                       /* 0x4C  0xFF98   ivVReserved51 unused by PE */
  &Cpu_Interrupt,                       /* 0x4D  0xFF9A   ivVReserved50 unused by PE */
  &Cpu_Interrupt,                       /* 0x4E  0xFF9C   ivVReserved49 unused by PE */
  &Cpu_Interrupt,                       /* 0x4F  0xFF9E   ivVReserved48 unused by PE */
  &Cpu_Interrupt,                       /* 0x50  0xFFA0   ivVReserved47 unused by PE */
  &Cpu_Interrupt,                       /* 0x51  0xFFA2   ivVReserved46 unused by PE */
  &Cpu_Interrupt,                       /* 0x52  0xFFA4   ivVReserved45 unused by PE */
  &Cpu_Interrupt,                       /* 0x53  0xFFA6   ivVReserved44 unused by PE */
  &Cpu_Interrupt,                       /* 0x54  0xFFA8   ivVReserved43 unused by PE */
  &Cpu_Interrupt,                       /* 0x55  0xFFAA   ivVReserved42 unused by PE */
  &Cpu_Interrupt,                       /* 0x56  0xFFAC   ivVReserved41 unused by PE */
  &Cpu_Interrupt,                       /* 0x57  0xFFAE   ivVReserved40 unused by PE */
  &Cpu_Interrupt,                       /* 0x58  0xFFB0   ivVcantx      unused by PE */
  &Cpu_Interrupt,  /* 0x59  0xFFB2   ivVcanrx      unused by PE */
  &Cpu_Interrupt,                       /* 0x5A  0xFFB4   ivVcanerr     unused by PE */
  &Cpu_Interrupt,                       /* 0x5B  0xFFB6   ivVcanwkup    unused by PE */
  &Cpu_Interrupt,                       /* 0x5C  0xFFB8   ivVflash      unused by PE */
  &Cpu_Interrupt,                       /* 0x5D  0xFFBA   ivVflashfd    unused by PE */
  &Cpu_Interrupt,                       /* 0x5E  0xFFBC   ivVspi2       unused by PE */
  &Cpu_Interrupt,                       /* 0x5F  0xFFBE   ivVspi1       unused by PE */
  &Cpu_Interrupt,                       /* 0x60  0xFFC0   ivVReserved31 unused by PE */
  &Cpu_Interrupt,                       /* 0x61  0xFFC2   ivVsci2       unused by PE */
  &Cpu_Interrupt,                       /* 0x62  0xFFC4   ivVReserved29 unused by PE */
  &Cpu_Interrupt,                       /* 0x63  0xFFC6   ivVcpmuplllck unused by PE */
  &Cpu_Interrupt,                       /* 0x64  0xFFC8   ivVcpmuocsns  unused by PE */
  &Cpu_Interrupt,                       /* 0x65  0xFFCA   ivVReserved26 unused by PE */
  &Cpu_Interrupt,                       /* 0x66  0xFFCC   ivVReserved25 unused by PE */
  &Cpu_Interrupt,                       /* 0x67  0xFFCE   ivVportj      unused by PE */
  &Cpu_Interrupt,                       /* 0x68  0xFFD0   ivVReserved23 unused by PE */
  &Cpu_Interrupt,                       /* 0x69  0xFFD2   ivVatd        unused by PE */
  &Cpu_Interrupt,          /* 0x6A  0xFFD4   ivVsci1       unused by PE */
  &Cpu_Interrupt,                       /* 0x6B  0xFFD6   ivVsci0       unused by PE */
  &Cpu_Interrupt,                       /* 0x6C  0xFFD8   ivVspi0       unused by PE */
  &Cpu_Interrupt,                       /* 0x6D  0xFFDA   ivVtimpaie    unused by PE */
  &Cpu_Interrupt,                       /* 0x6E  0xFFDC   ivVtimpaaovf  unused by PE */
  &TOI_ISR,                       /* 0x6F  0xFFDE   ivVtimovf     unused by PE */
  &Cpu_Interrupt,       /* 0x70  0xFFE0   ivVtimch7     unused by PE */
  &Cpu_Interrupt,                       /* 0x71  0xFFE2   ivVtimch6     unused by PE */
  &Cpu_Interrupt,                       /* 0x72  0xFFE4   ivVtimch5     unused by PE */
  &Cpu_Interrupt,                       /* 0x73  0xFFE6   ivVtimch4     unused by PE */
  &Cpu_Interrupt,                       /* 0x74  0xFFE8   ivVtimch3     unused by PE */
  &IC2_ISR,                       /* 0x75  0xFFEA   ivVtimch2     unused by PE */
  &Cpu_Interrupt,                       /* 0x76  0xFFEC   ivVtimch1     unused by PE */
  &Cpu_Interrupt,                       /* 0x77  0xFFEE   ivVtimch0     unused by PE */
  &Cpu_Interrupt,                       //Cpu_Interrupt,///* 0x78  0xFFF0   ivVrti        unused by PE */
  &Cpu_Interrupt,                       /* 0x79  0xFFF2   ivVirq        unused by PE */
  &Cpu_Interrupt,                       /* 0x7A  0xFFF4   ivVxirq       unused by PE */
  &Cpu_Interrupt,                       /* 0x7B  0xFFF6   ivVswi        unused by PE */
  &Cpu_Interrupt,                       /* 0x7C  0xFFF8   ivVtrap       unused by PE */
  &_Startup,                         /* 0xFFFA  ivVcop         unused by PE */
  &_Startup,                         /* 0xFFFC  ivVclkmon      unused by PE */
  &_Startup                          /* 0xFFFE  ivVreset       used by PE */
};
#pragma CONST_SEG DEFAULT



