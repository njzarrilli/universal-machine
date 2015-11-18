/*
 *      instruction.c
 *      Alexander Ravan (aravan01) and Naomi Zarrilli (nzarri01)
 *      HW6: UM
 *      November 12, 2014
 *      
 *      Summary: this is the implementation of the instruction module
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "bitpack.h"
#include "instruction.h"
#include "register.h"
#include "uarray.h"
#include "um-opcode.h"
#include "mem.h"

const uint32_t EOF_VAL = -1;
const int MAX_ASCII = 255;
const uint64_t MAX_NUM = 4294967296; /* this is 2^32 */

/* 
 * unpacks an instruction into opcode, ra, rb, and rc. If opcode LV is unpacked,
 * only the opcode, val, and ra will be unpacked. Then the instruction switch is
 * called for all opcode, except LV, using the unpacked values
 */  
void unpack_inst(Memory mem, registers regs, Um_instruction inst)
{       
        Um_Opcode opcode = Bitpack_getu(inst, 4, 28);
        if ((opcode == LV) || (opcode == 13)) {
                unsigned ra = Bitpack_getu(inst, 3, 25);
                uint32_t val = Bitpack_getu(inst, 25, 0);
                load_val(regs, ra, val);
        } else {
                unsigned rc = Bitpack_getu(inst, 3, 0);
                unsigned rb = Bitpack_getu(inst, 3, 3);
                unsigned ra = Bitpack_getu(inst, 3, 6);

                instruction_switch(mem, regs, opcode, ra, rb, rc);
        }
                if (opcode != LOADP){
                        increment_prog_counter(mem);
                }
}

/* switch table for instructions, opcode determines which case to execute */
void instruction_switch(Memory mem, registers regs, Um_Opcode opcode, 
                        unsigned ra, unsigned rb, unsigned rc)
{
        word *val;
        word temp_a;
        word temp_b;
        word temp_c;
        word temp_and;
        uint32_t segid;
        uint32_t len;

        switch(opcode) {
                case 0:
                        if (at_reg(regs, rc) != 0) {
                                load_val(regs, ra, at_reg(regs, rb));
                        }
                        break;
                case SLOAD:
                        temp_b = at_reg(regs, rb);
                        temp_c = at_reg(regs, rc);
                        val = get_word(mem, temp_b, temp_c); 
                        load_val(regs, ra, *val);
                        break;

                case SSTORE: 
                        temp_a = at_reg(regs, ra);
                        temp_b = at_reg(regs, rb);
                        val = get_word(mem, temp_a, temp_b);
                        *val = at_reg(regs, rc);
                        break;
                case ADD:
                        temp_b = at_reg(regs, rb);
                        temp_c = at_reg(regs, rc);
                        load_val(regs, ra, ((temp_b + temp_c) % MAX_NUM));
                        break;
                case MUL: 
                        temp_b = at_reg(regs, rb);
                        temp_c = at_reg(regs, rc);
                        load_val(regs, ra, ((temp_b * temp_c) % MAX_NUM));
                        break;
                case DIV:
                        temp_b = at_reg(regs, rb);
                        temp_c = at_reg(regs, rc);
                        assert(temp_c != 0);
                        load_val(regs, ra, ((temp_b / temp_c) % MAX_NUM));
                        break;
                case NAND:
                        temp_b = at_reg(regs, rb);
                        temp_c = at_reg(regs, rc);
                        temp_and = ~(temp_b & temp_c);
                        load_val(regs, ra, temp_and);
                        break;
                case HALT:
                        free_regs(regs);
                        free_segs(mem);
                        free_stack(mem);
                        FREE(mem);
                        exit(0);
                case ACTIVATE:
                        segid = map(mem, regs, rc);
                        load_val(regs, rb, segid);
                        break;
                case INACTIVATE:
                        temp_c = at_reg(regs, rc);
                        assert(temp_c != 0);
                        unmap(mem, temp_c);
                        break;
                case OUT:
                        temp_c = at_reg(regs, rc);
                        assert(temp_c <= (uint32_t)MAX_ASCII);
                        putchar(temp_c);
                        break;
                case IN:
                        input(regs, rc);
                        temp_c = at_reg(regs, rc);
                        break;
                case LOADP:
                        temp_b = at_reg(regs, rb);
                        temp_c = at_reg(regs, rc);
                        if (temp_b == 0){
                                set_prog_counter(mem, temp_c);
                        } else {
                                len = get_seg_length(mem, temp_b);
                                move_seg_copy(mem, temp_b, 0);
                                set_num_words(mem, len);
                                set_prog_counter(mem, temp_c);
                        }
                        break;
                case LV:
                        break;
        }
}

/* reads in input and saves in rc */        
void input(registers regs, unsigned rc)
{
        int32_t c = fgetc(stdin);
        
        if (c == -1) {
                load_val(regs, rc, EOF_VAL);
        } else if (c <= MAX_ASCII && c >= 0) {
                load_val(regs, rc, (word)c);
        }
}
