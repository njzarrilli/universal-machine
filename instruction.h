/*      
 *      instruction.h
 *
 *
 *
 */
#ifndef _INSTRUCTION_   
#define _INSTRUCTION_
#include <seq.h>
#include <stdint.h>
#include "um-opcode.h"
#include "segment.h"
#include "types.h"

void unpack_inst(Memory mem, UArray_T regs, Um_instruction inst);
void instruction_switch(Memory mem, UArray_T regs, Um_Opcode opcode, 
                        unsigned ra, unsigned rb, unsigned rc);

void input(UArray_T regs, unsigned rc);

#endif
