/*
 *      um.c
 *      COMP 40 HW 6
 *      11/12/14
 *
 *      Naomi Zarrilli (nzarri01) & Alexander Ravan (aravan01)
 *      Summary: this is the um implementation file, which utilizes the segment
 *      and register modules to emulate a um
 */

#include "segment.h"
#include "instruction.h"
#include "register.h"
#include <stdio.h>
#include "types.h"
#include <sys/stat.h>
#include <mem.h>
#include <assert.h>

const unsigned SIZE_CODEWORD = 32;

int32_t read_inst(FILE *input);
int32_t concat(int32_t inst, int byte);
void free_all(Memory mem, registers regs);

int main(int argc, char *argv[])
{
        (void) argc;
        FILE *input = fopen(argv[1], "r");
        assert(input != NULL); 
        struct stat st;
        stat(argv[1], &st);
        int size = st.st_size;
        int num_words = size / 4;
        int32_t inst;
         
        registers regs = init_regs();
        Memory mem = init_memory(num_words);
        
        uint32_t counter = 0;
        int byte = fgetc(input);
        ungetc(byte, input);
        set_num_words(mem, num_words); 

        /*reads the program from file and stores it into segment 0 */ 
        while (byte != EOF) {
                inst = read_inst(input);
                if(inst == EOF){
                        break;
                }
                init_load(mem, counter, (word)inst);

                /* counter is the offset in segment 0 */
                counter++;
        }
        fclose(input);
        /* executes instructions stroed in segment 0 */
        while (get_prog_counter(mem) < get_num_words(mem)) {
                inst = get_inst(mem, 0);
                unpack_inst(mem, regs, inst);
        }  
        free_all(mem, regs);
        return 0;
}

/* concatonates a byte to an instruction */
int32_t concat(int32_t inst, int byte)
{
        inst = inst << 8;
        inst = inst | byte;
        return inst;
}

/* reads an instruction, concatonating input to an instruction */
int32_t read_inst(FILE *input)
{
        int32_t inst = 0;
        int byte;

        for (int lsb = SIZE_CODEWORD - 8; lsb >= 0; lsb -=8) {
                byte = fgetc(input);
                if (byte == EOF) {
                        return EOF;
                }
                inst = concat(inst, byte);
        }
        return inst;
}

/* frees all memory associated with the UM */
void free_all(Memory mem, registers regs)
{
        free_segs(mem);
        free_stack(mem);
        free_regs(regs);
        FREE(mem);
}
