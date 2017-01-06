/*******************************************************************************
*
*  FILE
*     Compiler.h
*
*  DESCRIPTION
*     Compiler Keyword 
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

#ifndef _COMPILER_H_ 
#define _COMPILER_H_  

/*******************************************************************************
* include files                                                 
*******************************************************************************/
#include "Compiler_Cfg.h"    /* Module specific memory and pointer classes    */

/*******************************************************************************
*                      Global Symbols                                      
*******************************************************************************/

/* Compiler abstraction symbols */
#define ROM_MEM   const
#define _STATIC_  static

#ifndef NULL_PTR
   #define NULL_PTR  ((void *)0)                       
#endif


/* AUTOMATIC used for the declaration of local pointers */
#define AUTOMATIC

/* used as an empty memory class */
#define TYPEDEF

/* Type definition of pointers to functions
   rettype     return type of the function
   ptrclass    defines the classification of the pointer's distance
   fctname     function name respectivly name of the defined type
 */
#define P2FUNC(rettype, ptrclass, fctname) rettype (* ptrclass fctname)

/* Type definition of const pointers to functions
   rettype     return type of the function
   ptrclass    defines the classification of the pointer's distance
   fctname     function name respectivly name of the defined type
 */
#define CONSTP2FUNC(rettype, ptrclass, fctname) rettype (* const ptrclass fctname)

/* The compiler abstraction shall define the FUNC macro for the declaration and
   definition of functions, that ensures correct syntax of function declarations
   as required by a specific compiler. - used for API functions
   rettype     return type of the function
   memclass    classification of the function itself
 */
#define FUNC(type, memclass) type memclass

/* The compiler abstraction shall define the FUNC macro for the declaration and
   definition of functions, that ensures correct syntax of function declarations
   as required by a specific compiler. - used for API functions
   rettype     return type of the function
   memclass    classification of the function itself
 */
#define IRQFUNC(func) interrupt void __near func(void)


/* The compiler abstraction shall define the FUNC macro for the declaration and
   definition of functions, that ensures correct syntax of function declarations
   as required by a specific compiler. - used for API functions
   rettype     return type of the function
   memclass    classification of the function itself
 */
#define XIRQFUNC(func) interrupt void func

/* Pointer to constant data
   ptrtype     type of the referenced data
   memclass    classification of the pointer's variable itself
   ptrclass    defines the classification of the pointer's distance
 */
#define P2CONST(ptrtype, memclass, ptrclass) const ptrtype memclass * ptrclass

/* Pointer to variable data
   ptrtype     type of the referenced data
   memclass    classification of the pointer's variable itself
   ptrclass    defines the classification of the pointer's distance
 */
#define P2VAR(ptrtype, memclass, ptrclass) ptrtype memclass * ptrclass

/* Const pointer to variable data
   ptrtype     type of the referenced data
   memclass    classification of the pointer's variable itself
   ptrclass    defines the classification of the pointer's distance
 */
#define CONSTP2VAR(ptrtype, memclass, ptrclass) ptrtype memclass * const ptrclass

/* Const pointer to constant data
   ptrtype     type of the referenced data
   memclass    classification of the pointer's variable itself
   ptrclass    defines the classification of the pointer's distance
 */
#define CONSTP2CONST(ptrtype, memclass, ptrclass) const ptrtype memclass * const ptrclass

/* ROM constant
   type        type of the constant
   memclass    classification of the constant
 */
#define CONST(type, memclass) const type memclass 

/* RAM variables
   type        type of the variable
   memclass    classification of the variable
 */
#define VAR(type, memclass) type memclass

/*******************************************************************************
*           Macro substitutions of ansi c memory management                                     
*******************************************************************************/
/* substitute memory copy */
//# define MemCpy2(a, b, c)    memcpy2(a, b, c)
/* substitute memory copy */
//# define MemCpy(a, b, c)    memcpy(a, b, c)
/* substitute memory set */
//# define MemSet(a, b, c)    memset(a, b, c)
/* substitute memory compare */
//# define MemCmp(a, b, c)    memcmp(a, b, c)

#endif /* _COMPILER_H_ */
