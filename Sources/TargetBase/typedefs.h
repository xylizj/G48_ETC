#ifndef __TYPEDES_H__
#define __TYPEDES_H__

#include "stddef.h"
 
#ifdef __cplusplus
extern "C" {
#endif


typedef void (*PROC)(void);
/*!< Parameterless function pointer (Procedure variable) */


/*REQ IDs: TPMS_SW_COM_0002*/
#if defined(__CHAR_IS_8BIT__)
  typedef  unsigned char      uint8_t;
  typedef    signed char      int8_t;
#elif defined(__SHORT_IS_8BIT__)
  typedef  unsigned short     uint8_t;
  typedef    signed short     int8_t;
#elif defined(__INT_IS_8BIT__)
  typedef  unsigned int       uint8_t;
  typedef    signed int       int8_t;
#elif defined(__LONG_IS_8BIT__)
  typedef  unsigned long      uint8_t;
  typedef    signed long      int8_t;
#elif defined(__LONG_LONG_IS_8BIT__)
  typedef  unsigned long long uint8_t;
  typedef    signed long long int8_t;
#else /* default */
  typedef  unsigned char      uint8_t;
  typedef    signed char      int8_t;
#endif
/*! \typedef Byte 
    Byte is a unsigned integral 8 bit type (typically unsigned char) */
/*! \typedef sByte  
    sByte is a signed integral 8 bit type (typically signed char)  */


#if defined(__CHAR_IS_16BIT__)
  typedef  unsigned char      uint16_t;
  typedef    signed char      int16_t;
#elif defined(__SHORT_IS_16BIT__) && !defined(__INT_IS_16BIT__)
  typedef  unsigned short     uint16_t;
  typedef    signed short     int16_t;
#elif defined(__INT_IS_16BIT__)
  typedef  unsigned int       uint16_t;
  typedef    signed int       int16_t;
#elif defined(__LONG_IS_16BIT__)
  typedef  unsigned long      uint16_t;
  typedef    signed long      int16_t;
#elif defined(__LONG_LONG_IS_16BIT__)
  typedef  unsigned long long uint16_t;
  typedef    signed long long int16_t;
#else
  typedef  unsigned short     uint16_t;
  typedef    signed short     int16_t;
#endif
/*! \typedef Word 
    Word is a unsigned integral 16 bit type (typically unsigned short) */
/*! \typedef sWord  
    sWord is a signed integral 16 bit type (typically signed short)  */

#if defined(__CHAR_IS_32BIT__)
  typedef  unsigned char      uint32_t;
  typedef    signed char      int32_t;
#elif defined(__SHORT_IS_32BIT__)
  typedef  unsigned short     uint32_t;
  typedef    signed short     int32_t;
#elif defined(__INT_IS_32BIT__)
  typedef  unsigned int       uint32_t;
  typedef    signed int       int32_t;
#elif defined(__LONG_IS_32BIT__)
  typedef  unsigned long      uint32_t;
  typedef    signed long      int32_t;
#elif defined(__LONG_LONG_IS_32BIT__)
  typedef  unsigned long long uint32_t;
  typedef    signed long long int32_t;
#else /* default */
  typedef  unsigned long uint32_t;
  typedef    signed long int32_t;
#endif
/*! \typedef LWord 
    LWord is a unsigned integral 32 bit type (typically unsigned int or unsigned long) */
/*! \typedef sLWord  
    sLWord is a signed integral 32 bit type (typically signed int or signed long)  */


typedef volatile signed char vint8_t;
typedef volatile unsigned char vuint8_t;

typedef volatile signed int vint16_t;
typedef volatile unsigned int vuint16_t;

typedef volatile signed long vint32_t;
typedef volatile unsigned long vuint32_t;



#ifndef bool
typedef unsigned char bool;
#endif

#ifndef BOOL
#define BOOL bool /*REQ IDs: TPMS_SW_COM_0003*/
#endif

#define true    (1u)
#define false   (0u)

/*REQ IDs: TPMS_SW_COM_0004*/
/*!< Definition for boolean type. */
#ifdef __MISRA__ /* MISRA rule #18 */
  #define TRUE  1u
    /*!< Definitioni for TRUE. */
  #define FALSE 0u
    /*!< Definition for FALSE. */
#else
  #define TRUE  1
    /*!< Definitioni for TRUE. */
  #define FALSE 0
    /*!< Definition for FALSE. */
#endif

#ifndef true
#define true    1
#endif
#ifndef false
#define false   0
#endif

#define STD_ON  1
#define STD_OFF 0

#ifndef NULL
#define NULL 0 /*REQ IDs: TPMS_SW_COM_0005*/
#endif

#ifndef null
#define null 0
#endif

#define UINT8_T_MAX   (0xFF)
#define INT8_T_MAX    (0x7F)
#define UINT16_T_MAX  (0xFFFF)
#define INT16_T_MAX   (0x7FFF)
#define UINT32_T_MAX  (0xFFFFFFFF)
#define INT32_T_MAX   (0x7FFFFFFF)
#define CAL const volatile

typedef enum __IO_ERR_T__
{
  IO_ERR_OK = 0,
  IO_ERR_PARAMETER_ERROR,
  IO_ERR_BUSY,
  IO_ERR_RUN_ERROR,
  IO_ERR_OTHER_FAILURE  
}io_err_t; /*REQ IDs: TPMS_SW_BASE_0011*/

#ifdef __cplusplus
}
#endif

#endif /*end if __TYPEDES_H__*/

