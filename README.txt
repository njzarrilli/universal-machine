This is a project I worked on when I was taking a Machine Structure and Assembly
Language Programming course during the Fall of 2014. This was a pair programming
assignment. We implemented a simple virtual machine in C, which emulated a Turing-Complete machine.
This VM reads in a .um file of instructions and executes these machine 
code instructions. The instructions are stored in memory segments, each segment
containg one instruction word. The instructions are loaded from the segmented 
memory into registers (this VM has 8). The unpacking and execution of the 
instructions in done in instructions.c. These registers store 32-bit ints.
