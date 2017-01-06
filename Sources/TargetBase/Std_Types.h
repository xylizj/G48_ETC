/*******************************************************************************
*
*  FILE
*     Std_Types.h
*
*  DESCRIPTION
*     Standard type definition
*      
*       
*  COPYRIGHT
*     
*     All rights reserved.
*
*  AUTHOR
*    
*
*  VERSION
*    1.00
*
*******************************************************************************/

#ifndef _STD_TYPES_H_ 
#define _STD_TYPES_H_ 

/*******************************************************************************
* include files                                                 
*******************************************************************************/

#include "Platform_Types.h"             /* platform specific type definitions */
#include "Compiler.h"                   /* mapping compiler specific keywords */
#include <stdtypes.h>

/*******************************************************************************
* Defines                                                                
*******************************************************************************/

/* for OSEK compliance this typedef has been added */
#ifndef STATUSTYPEDEFINED 
  #define STATUSTYPEDEFINED 

  typedef unsigned char StatusType; 
  #define E_OK    0

#endif

/*
  The Std_ReturnType may be used with the following values:
  E_OK:     0
  E_NOT_OK: 1
*/
#define E_NOT_OK  1


#define STD_HIGH    1  /* Physical state 5V or 3.3V */
#define STD_LOW     0  /* Physical state 0V         */

#define STD_ACTIVE  1  /* Logical state active      */
#define STD_IDLE    0  /* Logical state idle        */

#define STD_ON      1  
#define STD_OFF     0 

/*******************************************************************************
* Defines                                                                
*******************************************************************************/
typedef struct
{
   uint16 vendorID;
   uint8 moduleID;
   uint8 sw_major_version;
   uint8 sw_minor_version;
   uint8 sw_patch_version;
}Std_VersionInfoType;

typedef uint8 Std_ReturnType;
#endif  /* _STD_TYPES_H_ */
