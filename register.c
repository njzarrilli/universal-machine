/*
 *      regsiter.c
 *      Alexander Ravan (aravan01) and Naomi Zarrilli (nzarri01)
 *      HW6: UM
 *      November 12, 2014
 *      
 *      Summary: this is the implementation of the register module
 */

#include <stdio.h>
#include <stdlib.h>
#include "register.h"

/* initializes all eight register */
registers init_regs() 
{
        UArray_T regs = UArray_new(8, sizeof(word));
        return regs;
}

/* frees all memory associated with registers */
void free_regs(registers regs)
{
        UArray_free(&regs);
}

/* loads a value into register at reg_num */
void load_val(registers regs, unsigned reg_num, word val)
{
        word *temp = (word *)UArray_at(regs, reg_num);
        *temp = val;
}

/* returns val at registers[reg_num] */
word at_reg(registers regs, unsigned reg_num)
{      
        return *((word *)UArray_at(regs, reg_num));
}
