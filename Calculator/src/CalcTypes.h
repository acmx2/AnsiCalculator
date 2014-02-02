#ifndef __FG_CALC_TYPES_H__ 
#define __FG_CALC_TYPES_H__

#include <limits.h>

/**An integer type ('big-int') used for the calculator. The integer sign is stored separately in a char field.*/
#define CL_ULONG unsigned long int
/**Max value for the integer modulus*/
#define CL_ULONG_MAX (ULONG_MAX-1)
/**A min memory size to reserve for printing the integer*/
#define CL_ULONG_MAX_PRINT 500

#endif
