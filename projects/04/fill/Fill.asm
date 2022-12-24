// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, the
// program clears the screen, i.e. writes "white" in every pixel.

// select first screen pixels
// set data to first screen pixels
// set M to -1 (first screen pixels black)
// set data to data + 1
// set A to data + 1
// set M to -1 and repeat

// have a register that tracks which pixels we have selected and increment it
// copy the current increment value into another register that counts in the opposite direction
// figure out when to stop changing the screen
// can't even begin to figure out where to start the keyboard input haha

// Greater than logic: Copy first value to accumulator (data), go to second value location, make D equal the difference between D and second value (M), D=D-M
// Now check if D is greater than zero

// Screen size: 256 rows of 512 pixels

////////////////////////////// KEYBOARD PRESS LISTENER /////////////////////////////////////
(KBDLP)
@KBD 
D=M
@KBDLP
D;JEQ

@BLACKFLAG
M=0

////////////////////////////// BLACK & WHITE PIXEL LOOP ////////////////////////////////////
(LOOP) // primitive keyboard 'listener'

// INITIALIZE TRACKERS
@SCREEN //initialize screen start for tracker
D=A
@COORD  // set coordinate tracker
M=D

// INITIALIZE COUNTER
@8192   // length of number of screens for loop comparison
D=A     // +1 was used earlier to easily accommodate subtraction in the last loop (when JMP set to not equal to zero) Don't need for JGT
@COUNT  // free D for use in keyboard (and reset count after first loop)
M=D

(SCREENLOOP)

@COORD  // loop to coord
A=M     // Use current coord to move A to current screen coords
M=!M    // set the value in the current screen coords to 111111... (black) or 00000... (white)

@COORD
M=M+1   // increment coord

// DECREASE SCREEN COUNTER AND CHECK IF COMPLETE
@COUNT
M=M-1 
D=M

@SCREENLOOP
D;JGT
////////////////////////////////////////////////////////////////////////////////////////////

@BLACKFLAG  // check the flag to see if we're resetting to white or going through black (from white)
D=M
@KBDLP
D;JNE       // zero: jump to wait for input. Not zero: continue to keep black until key is not pressed anymore


////////////////////////////// KEEP BLACK WHILE PRESSED ////////////////////////////////////
(KBDLP2)
@KBDLP2

@KBD
D=M

@KBDLP2
D;JNE

// set flag to go back to first listener after key is no longer pressed
@BLACKFLAG
M=1

// return to screen loop when key not pressed to reset to white
@LOOP
0;JMP

// end program loop in unlikely case of main loop break
(END)
@END
0;JMP

