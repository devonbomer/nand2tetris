// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Grab the value 1 in R0
@R0
D=M

// initialize R2 to 0
@R2
M=0
// notice, was able to manipulate constant without A ( probably assembler picking up zx in ALU)

// Set counter in RAM[10] to value in R0
@COUNT
M=D

// check if counter value was zero, if other value was zero, will just add zero to R2 (WASTE OF R0 operations)
@END
D;JLE


// addition loop (add R1 to R2, R0 times)
(JUMP)
@R1
D=M

@R2
M=M+D

@COUNT
M=M-1
D=M

@JUMP
D;JGT

// reset timer after loop (not really necessary)
@COUNT
M=0

(END)
@END
0;JMP

// just need to distinguish R1 and R0 roles in multiplication
// R0 is for counter
// R1 is added to R2