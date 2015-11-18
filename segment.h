/*
 *      segment.h
 *      Alexander Ravan (aravan01) and Naomi Zarrilli (nzarri01)
 *      HW6: UM
 *      November 12, 2014
 *
 *      Summary: this is the header file for the segment module which deals
 *               with memory managment of the segemented memory of the UM
 */

#ifndef _SEGMENT_
#define _SEGMENT_

#include <stdint.h>
#include "seq.h"
#include "uarray.h"
#include "types.h"

/**************************************************************************
 *                              Getters                                   *
 **************************************************************************/

UArray_T        get_seg(Memory mem, uint32_t segid);
uint32_t        get_prog_counter (Memory mem);
Seq_T           get_segs(Memory mem);
Um_instruction  get_inst(Memory mem, uint32_t segid);
uint32_t        get_num_words(Memory mem);
word            *get_word(Memory mem, uint32_t segid, uint32_t offset);
uint32_t        get_seg_length(Memory mem, uint32_t segid);

/************************************************************************
 *                              Setters                                 *
 ************************************************************************/
void            set_prog_counter(Memory mem, uint32_t val);
void            set_num_words(Memory mem, uint32_t val);
void            increment_prog_counter(Memory mem);


/*************************************************************************
 *                              Initializers                             *
 *************************************************************************/
Memory          init_memory(int num_words);
void            init_load(Memory mem, unsigned i, word val);


/**************************************************************************
 *                        Uncategorized Functions                         *
 **************************************************************************/
UArray_T        new_seg(registers regs, unsigned rc);
uint32_t        map(Memory mem, registers regs, unsigned rc);
void            unmap(Memory mem, uint32_t segid);
void            move_seg_copy(Memory mem, uint32_t segid_from, 
                              uint32_t segid_to);
void            load(void *val);

/*************************************************************************
 *                             Freeing Memory                            *
 *************************************************************************/
void            free_segs(Memory mem);
void            free_stack(Memory mem);


#endif
