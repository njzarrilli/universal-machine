/*      
 *      regsiter.h
 *      Alexander Ravan (aravan01) and Naomi Zarrilli (nzarri01)
 *      HW6: UM
 *      November 12, 2014
 *      
 *      Summary: thi is the header file for the register module
 */

#ifndef _REGISTER_
#define _REGISTER_

#include "uarray.h"
#include "stdint.h"
#include "types.h"

/* Note: registers has been typedef'd as a UArray_T */

registers       init_regs();
void            free_regs(registers regs);
void            load_val(registers regs, unsigned reg_num, word val);
word            at_reg(registers regs, unsigned reg_num);

#endif

