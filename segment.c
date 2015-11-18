/*
 *      segment.c
 *      Alexander Ravan (aravan01) and Naomi Zarrilli (nzarri01)
 *      HW6: UM
 *      November 12, 2014
 *      
 *      Summary: this is the implementation of the segment module which manages
 *               the segmented memory of the UM
 */


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "register.h"
#include "segment.h"
#include "bitpack.h"
#include "stack.h"
#include "mem.h"

struct Memory {
        Seq_T segments;
        Stack_T stack;
        int seg_counter;
        uint32_t prog_counter;
        uint32_t num_words;
};

/* Adds a newly created segment to sequence of segments */
static inline void add_seg(Memory mem, registers regs, unsigned rc)
{    
        UArray_T seg = new_seg(regs, rc);
        Seq_addhi(mem->segments, (void *) (uintptr_t) seg);
}

/* returns a segid from the top of the stack of unmapped segids */
static inline uint32_t pop_segid(Memory mem)
{
        uint32_t segid = (unsigned)(uintptr_t)(Stack_pop(mem->stack));
        return segid;
}

/* returns number of words in segment 0 */
uint32_t get_num_words(Memory mem)
{
        return mem->num_words;
}

/* sets value of num_words struct member */
void set_num_words(Memory mem, uint32_t val)
{
        mem->num_words = val;
}

/* returns sequence of segments */
Seq_T get_segs(Memory mem)
{
        return mem->segments;       
}

/* returns length of a segment at seq[segid] */
uint32_t get_seg_length(Memory mem, uint32_t segid)
{
        UArray_T seg = get_seg(mem, segid);
        return UArray_length(seg);
}

/* loads a program word by word into 0th segment */
void init_load(Memory mem, unsigned i, word val)
{
        UArray_T seg = get_seg(mem, 0);
        word *inst = (word *) UArray_at(seg, i);
        *inst = val;
}

/* returns an instruction from a segment[program_counter] */
Um_instruction get_inst(Memory mem, uint32_t segid)
{
        UArray_T seg = get_seg(mem, segid);
        Um_instruction inst = *((Um_instruction *)UArray_at(seg, 
                                                            mem->prog_counter));
        return inst;
}

/* sets program counter to a value */
void set_prog_counter(Memory mem, uint32_t val)
{
        mem->prog_counter = val;
}

/* returns program counter */
uint32_t get_prog_counter(Memory mem)
{
        return mem->prog_counter;
}

/* increments the program counter by 1 */
void increment_prog_counter(Memory mem)
{
        (mem->prog_counter)++;
}

/* creates a copy of a segment and moves it into another slot in the sequence*/
void move_seg_copy(Memory mem, uint32_t segid_from, uint32_t segid_to)
{
        UArray_T temp = Seq_get(mem->segments, segid_from);
        UArray_T temp2 = Seq_get(mem->segments, segid_to);
        UArray_free(&temp2);
        UArray_T copy = UArray_copy(temp, UArray_length(temp));
        Seq_put(mem->segments, segid_to, copy);
}

/* returns a word located at the offset of a segment */
word *get_word(Memory mem, uint32_t segid, uint32_t offset)
{
        UArray_T temp = get_seg(mem, segid);
        word *val = (word *)UArray_at(temp, offset);
        return val;
}

/* initializes memory struct and accompanying data structure members */
Memory init_memory(int num_words)
{
        Memory mem = malloc(sizeof(struct Memory));
        mem->segments = Seq_new(1);
        UArray_T seg = UArray_new(num_words, sizeof(word));
        
        Seq_addhi(mem->segments, (void *) (uintptr_t) seg); 
        mem->stack = Stack_new();
        set_prog_counter(mem, 0);
        mem->seg_counter = 0;
        
        return mem;
}

/* creates new segment */
UArray_T new_seg(registers regs, unsigned rc)
{
        uint32_t num_words = at_reg(regs, rc);
        UArray_T seg;
        if(num_words != 0) {
                seg = UArray_new(num_words, sizeof(word));
                //zero_seg(seg);
        } else {
                seg = NULL;
        }
        return seg;
}

/* maps a segment to memory */
uint32_t map(Memory mem, registers regs, unsigned rc)
{
        uint32_t segid;
        if (Stack_empty(mem->stack)){
                mem->seg_counter++;
                add_seg(mem, regs, rc);
                segid = mem->seg_counter;    
        } else {
                segid = pop_segid(mem);
                UArray_T array = new_seg(regs, rc);
                Seq_put(mem->segments, segid, array);
        }
        return segid;
}

/* 
 * unmaps a segment and puts its segid onto stack of unmapped segids, then 
 * frees segment and sets the sequence to point to the now NULL segment 
 */
void unmap(Memory mem, uint32_t segid)
{
        assert(Seq_get(mem->segments, segid) != NULL);
        Stack_push(mem->stack, (void *)(uint64_t)segid);
        UArray_T seg = get_seg(mem, segid);
        UArray_free(&seg);
        Seq_put(mem->segments, segid, seg);
}

/* returns a segment from seq[segid] */
UArray_T get_seg(Memory mem, uint32_t segid)
{
        UArray_T seg = (UArray_T)(uintptr_t)(Seq_get(mem->segments, segid));
        return seg;
}

/* frees segments and the sequence */
void free_segs(Memory mem)
{
        uint32_t len = (unsigned)Seq_length(mem->segments);

        UArray_T temp;
        for (uint32_t i = 0; i < len; i++) {
                temp = (UArray_T)Seq_get(mem->segments, i);
                if(temp != NULL) {
                          UArray_free(&temp);
                }
        }
        Seq_free(&(mem->segments));
}

/* frees stack of unmapped segids */
void free_stack(Memory mem)
{
        Stack_free(&(mem->stack));
}
