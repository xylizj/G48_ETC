#ifndef APP_INTERFACE_H
#define APP_INTERFACE_H





/***************************************************************

0xF9C0 - 0xF9FD : User Appl. Vector
0xFA00 - 0xFFFF : BootLoader   

****************************************************************/

#define AppRegisterAddress  0xDF00
#define AppStartupAddress   0xDFFE          //App jump table begins from 0xEE00


typedef void (*near tIsrFunc)(void);

typedef struct
{
   void near (* near const AppStartup)( void );  //2 bytes, _Startup of appl.
   unsigned int  uiAppSwVersion;                 //BCD, should be filled by appl.
   unsigned int  uiEcuHwVersion;                 //BCD, should be filled by appl.
   unsigned int  uiReleaseYear;                  //BCD, should be filled by appl.
   unsigned int  uiReleaseMonthDay;              //BCD, should be filled by appl.
   unsigned char ucEcuModel[16];                 //ASCII, should be filled by appl.
   unsigned char ucHwSerialNum[16];              //ASCII, should be filled by appl.
   unsigned int  uiLastUpdateYear;               //BCD, filled by Efa.
   unsigned int  uiLastUpdateMonthDay;           //BCD, filled by Efa.
   unsigned int  uiConfirmWord1;                 //2 bytes, filled by Efa
   unsigned int  uiConfirmWord2;                 //2 bytes, filled by Efa
   unsigned int  uiChecksum;                     //2 bytes, calculated and filled by EFa
} AppRegisterData;


#pragma CONST_SEG APPREG_DATA
extern volatile const AppRegisterData AppRegisterTable;
#pragma CONST_SEG DEFAULT

#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void Cpu_Interrupt(void);
#pragma CODE_SEG DEFAULT

#pragma CONST_SEG APP_VECTOR
const tIsrFunc AppVectorTable[];
#pragma CONST_SEG DEFAULT

#endif
