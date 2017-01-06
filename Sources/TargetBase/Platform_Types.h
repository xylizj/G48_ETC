/*******************************************************************************
*
*  FILE
*     Platform_Types.h
*
*  DESCRIPTION
*     data type definition
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

#ifndef _PLATFORM_TYPES_H_ 
#define _PLATFORM_TYPES_H_  

/*******************************************************************************
*                      Global Symbols                                        
*******************************************************************************/
/*  CPU register type width  */
#define CPU_TYPE_8        8                        
#define CPU_TYPE_16       16                        
#define CPU_TYPE_32       32  

/*  Bit order definition */
#define MSB_FIRST         0                 /* Big endian bit ordering        */
#define LSB_FIRST         1                 /* Little endian bit ordering     */

/*  Byte order definition */
#define HIGH_BYTE_FIRST   0                 /* Big endian byte ordering       */
#define LOW_BYTE_FIRST    1                 /* Little endian byte ordering    */

/*  Word order definition */
#define HIGH_WORD_FIRST   0                 /* Big endian word ordering       */
#define LOW_WORD_FIRST    1                 /* Little endian word ordering    */

/* Definitions for TRUE and FALSE. */
/*#ifndef TRUE                                            
 #define TRUE      ((boolean) 1)
#endif

#ifndef FALSE                                          
 #define FALSE     ((boolean) 0)
#endif */

/* Platform type and endianess definitions for MC9S12XEP100 */
#define CPU_TYPE            CPU_TYPE_16
#define CPU_BIT_ORDER       LSB_FIRST
#define CPU_BYTE_ORDER      HIGH_BYTE_FIRST


/*******************************************************************************
*                     Global Data Types                                     
*******************************************************************************/

/*  AUTOSAR integer data types  */
typedef signed char         sint8;          /*        -128 .. +127            */
typedef unsigned char       uint8;          /*           0 .. 255             */
typedef signed int        sint16;         /*      -32768 .. +32767          */
typedef unsigned int      uint16;         /*           0 .. 65535           */
typedef signed long         sint32;         /* -2147483648 .. +2147483647     */
typedef unsigned long       uint32;         /*           0 .. 4294967295      */

typedef volatile signed char         vsint8;          /*        -128 .. +127            */
typedef volatile unsigned char       vuint8_t;          /*           0 .. 255             */
typedef volatile signed int        vsint16_t;         /*      -32768 .. +32767          */
typedef volatile unsigned int      vuint16_t;         /*           0 .. 65535           */
typedef volatile signed long         vsint32_t;         /* -2147483648 .. +2147483647     */
typedef volatile unsigned long       vuint32_t;         /*           0 .. 4294967295      */



typedef signed char         sint8_t;          /*        -128 .. +127            */
typedef unsigned char       uint8_t;          /*           0 .. 255             */
typedef signed int        sint16_t;         /*      -32768 .. +32767          */
typedef unsigned int      uint16_t;         /*           0 .. 65535           */
typedef signed long         sint32_t;         /* -2147483648 .. +2147483647     */
typedef unsigned long       uint32_t;         /*           0 .. 4294967295      */




typedef unsigned char       uint8_least;    /* At least 8 bit                 */
typedef unsigned short      uint16_least;   /* At least 16 bit                */
typedef unsigned long       uint32_least;   /* At least 32 bit                */
typedef signed char         sint8_least;    /* At least 7 bit + 1 bit sign    */
typedef signed short        sint16_least;   /* At least 15 bit + 1 bit sign   */
typedef signed long         sint32_least;   /* At least 31 bit + 1 bit sign   */

typedef unsigned char       boolean;        /* for use with TRUE/FALSE        */

typedef unsigned int        bittype;

/* Float point data type */
typedef float               float32;
typedef double              float64;


/* Extended Definition of Types */

#if (CPU_BIT_ORDER == LSB_FIRST )  
  typedef struct 
  {
    bittype  Bit0:1;  /* LSB-Bits (lower adr!) */
    bittype  Bit1:1;
    bittype  Bit2:1;
    bittype  Bit3:1;
    bittype  Bit4:1;
    bittype  Bit5:1;
    bittype  Bit6:1;
    bittype  Bit7:1;  /* MSB-Bits (higher address)  */
  }Bits8Type;
#else
  typedef struct         
  {
    bittype  Bit7:1;  /* MSB-Bits (higher address)  */
    bittype  Bit6:1;  /* LSB-Bits (lower adr!) */
    bittype  Bit5:1;
    bittype  Bit4:1;
    bittype  Bit3:1;
    bittype  Bit2:1;
    bittype  Bit1:1;
    bittype  Bit0:1;
  }Bits8Type;  
#endif /* #if (CPU_BIT_ORDER == LSB_FIRST )*/  

#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
  #if (CPU_BIT_ORDER == LSB_FIRST)
    typedef struct 
    {
      bittype  Bit8:1; /* higher address */
      bittype  Bit9:1;
      bittype  Bit10:1;
      bittype  Bit11:1;
      bittype  Bit12:1;
      bittype  Bit13:1;
      bittype  Bit14:1;
      bittype  Bit15:1;   

      bittype  Bit0:1;  /* lower adr */
      bittype  Bit1:1;
      bittype  Bit2:1;
      bittype  Bit3:1;
      bittype  Bit4:1;
      bittype  Bit5:1;
      bittype  Bit6:1;
      bittype  Bit7:1;  
    }Bits16Type;
  #else
    typedef struct 
    {
      bittype  Bit15:1;  /* higher adr */
      bittype  Bit14:1;
      bittype  Bit13:1;
      bittype  Bit12:1;
      bittype  Bit11:1;
      bittype  Bit10:1;
      bittype  Bit9:1;
      bittype  Bit8:1;  
      
      bittype  Bit7:1; /* lower address */
      bittype  Bit6:1;
      bittype  Bit5:1;
      bittype  Bit4:1;
      bittype  Bit3:1;
      bittype  Bit2:1;
      bittype  Bit1:1;
      bittype  Bit0:1;   
    }Bits16Type;  
  #endif /* #if (CPU_BIT_ORDER == LSB_FIRST)*/
#else
  #if (CPU_BIT_ORDER == LSB_FIRST) 
    typedef struct 
    {
      bittype  Bit0:1;  /* lower adr */
      bittype  Bit1:1;
      bittype  Bit2:1;
      bittype  Bit3:1;
      bittype  Bit4:1;
      bittype  Bit5:1;
      bittype  Bit6:1;
      bittype  Bit7:1;

      bittype  Bit8:1; /* higher adr */
      bittype  Bit9:1;
      bittype  Bit10:1;
      bittype  Bit11:1;
      bittype  Bit12:1;
      bittype  Bit13:1;
      bittype  Bit14:1;
      bittype  Bit15:1;
    }Bits16Type; 
  #else
    typedef struct 
    {
      bittype  Bit7:1; /* lower address */
      bittype  Bit6:1;
      bittype  Bit5:1;
      bittype  Bit4:1;
      bittype  Bit3:1;
      bittype  Bit2:1;
      bittype  Bit1:1;
      bittype  Bit0:1; 
      
      bittype  Bit15:1;  /* higher adr */
      bittype  Bit14:1;
      bittype  Bit13:1;
      bittype  Bit12:1;
      bittype  Bit11:1;
      bittype  Bit10:1;
      bittype  Bit9:1;
      bittype  Bit8:1;  
    }Bits16Type;    
  #endif      
#endif /* #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST) */  

#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
  #if (CPU_BIT_ORDER == LSB_FIRST)
   typedef struct 
    {
      bittype  Bit24:1; /* higher adr */
      bittype  Bit25:1;
      bittype  Bit26:1;
      bittype  Bit27:1;
      bittype  Bit28:1;
      bittype  Bit29:1;
      bittype  Bit30:1;
      bittype  Bit31:1;
      
      bittype  Bit16:1; /* mid high adr */
      bittype  Bit17:1;
      bittype  Bit18:1;
      bittype  Bit19:1;
      bittype  Bit20:1;
      bittype  Bit21:1;
      bittype  Bit22:1;
      bittype  Bit23:1;

      bittype  Bit8:1; /* mid lower adr */
      bittype  Bit9:1;
      bittype  Bit10:1;
      bittype  Bit11:1;
      bittype  Bit12:1;
      bittype  Bit13:1;
      bittype  Bit14:1;
      bittype  Bit15:1;

      bittype  Bit0:1;  /* lower adr */
      bittype  Bit1:1;
      bittype  Bit2:1;
      bittype  Bit3:1;
      bittype  Bit4:1;
      bittype  Bit5:1;
      bittype  Bit6:1;
      bittype  Bit7:1;
    }Bits32Type;
  #else
   typedef struct 
    {
      bittype  Bit31:1; /* higher adr */
      bittype  Bit30:1;
      bittype  Bit29:1;
      bittype  Bit28:1;
      bittype  Bit27:1;
      bittype  Bit26:1;
      bittype  Bit25:1;
      bittype  Bit24:1;
      
      bittype  Bit23:1; /* mid high adr */
      bittype  Bit22:1;
      bittype  Bit21:1;
      bittype  Bit20:1;
      bittype  Bit19:1;
      bittype  Bit18:1;
      bittype  Bit17:1;
      bittype  Bit16:1;

      bittype  Bit15:1; /* mid lower adr */
      bittype  Bit14:1;
      bittype  Bit13:1;
      bittype  Bit12:1;
      bittype  Bit11:1;
      bittype  Bit10:1;
      bittype  Bit9:1;
      bittype  Bit8:1;

      bittype  Bit7:1;  /* lower adr */
      bittype  Bit6:1;
      bittype  Bit5:1;
      bittype  Bit4:1;
      bittype  Bit3:1;
      bittype  Bit2:1;
      bittype  Bit1:1;
      bittype  Bit0:1;
    }Bits32Type;  
  #endif /* #if (CPU_BIT_ORDER == LSB_FIRST)*/
#else /* #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST) */
  #if (CPU_BIT_ORDER == LSB_FIRST) 
   typedef struct 
    {
      bittype  Bit0:1;  /* lower adr */
      bittype  Bit1:1;
      bittype  Bit2:1;
      bittype  Bit3:1;
      bittype  Bit4:1;
      bittype  Bit5:1;
      bittype  Bit6:1;
      bittype  Bit7:1;

      bittype  Bit8:1; /* mid lower adr */
      bittype  Bit9:1;
      bittype  Bit10:1;
      bittype  Bit11:1;
      bittype  Bit12:1;
      bittype  Bit13:1;
      bittype  Bit14:1;
      bittype  Bit15:1;

      bittype  Bit16:1; /* mid high adr */
      bittype  Bit17:1;
      bittype  Bit18:1;
      bittype  Bit19:1;
      bittype  Bit20:1;
      bittype  Bit21:1;
      bittype  Bit22:1;
      bittype  Bit23:1;

      bittype  Bit24:1; /* higher adr */
      bittype  Bit25:1;
      bittype  Bit26:1;
      bittype  Bit27:1;
      bittype  Bit28:1;
      bittype  Bit29:1;
      bittype  Bit30:1;
      bittype  Bit31:1;
    }Bits32Type; 
  #else
   typedef struct 
    {
      bittype  Bit7:1;  /* lower adr */
      bittype  Bit6:1;
      bittype  Bit5:1;
      bittype  Bit4:1;
      bittype  Bit3:1;
      bittype  Bit2:1;
      bittype  Bit1:1;
      bittype  Bit0:1;     

      bittype  Bit15:1; /* mid lower adr */
      bittype  Bit14:1;
      bittype  Bit13:1;
      bittype  Bit12:1;
      bittype  Bit11:1;
      bittype  Bit10:1;
      bittype  Bit9:1;
      bittype  Bit8:1;

      bittype  Bit23:1; /* mid high adr */
      bittype  Bit22:1;
      bittype  Bit21:1;
      bittype  Bit20:1;
      bittype  Bit19:1;
      bittype  Bit18:1;
      bittype  Bit17:1;
      bittype  Bit16:1;

      bittype  Bit31:1; /* higher adr */
      bittype  Bit30:1;
      bittype  Bit29:1;
      bittype  Bit28:1;
      bittype  Bit27:1;
      bittype  Bit26:1;
      bittype  Bit25:1;
      bittype  Bit24:1;      
    }Bits32Type;  
  #endif      
#endif /* #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST) */  




typedef struct 
{
# if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
  uint8 LowByte; /*!< least significant uint8 */
  uint8 HighByte; /*!< most significant uint8  */
# else
  uint8 HighByte; /*!< most significant uint8  */
  uint8 LowByte; /*!< least significant uint8 */
# endif /* _BIG_ENDIAN */
}TwoBytesType;

typedef struct 
{
# if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
  uint8 Byte0; 
  uint8 Byte1; 
  uint8 Byte2; 
  uint8 Byte3;
# else
  uint8 Byte3; 
  uint8 Byte2;
  uint8 Byte1; 
  uint8 Byte0;
# endif 
}FourBytesType;

typedef struct 
{
# if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
  uint16 LowWord; /*!< low word / least significant uint8  */
  uint16 HighWord; /*!< low word / most significant uint8   */
# else
  uint16 HighWord; /*!< low word / most significant uint8   */
  uint16 LowWord; /*!< low word / least significant uint8  */
# endif /* _BIG_ENDIAN == 1 */
}TwoWordsType;

typedef union 
{
  uint16  WORD; 
  TwoBytesType Bytes; 
}OneWordUnionType;

typedef union 
{
  uint32 Long;
  TwoWordsType Words;     /*!< long value          */
  FourBytesType Bytes;  /*!< splitted long value */
  Bits32Type Bits;
}OneLongUnionType;



#endif /* _PLATFORM_TYPES_H_ */

