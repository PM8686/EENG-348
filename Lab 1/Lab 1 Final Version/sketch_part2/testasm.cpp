#include "testasm.h"

/* the attribute "used" tells the compiler to not inline this function */
__attribute__((used))
unsigned int sumval(unsigned char a, unsigned char b) {
  unsigned int out;

  asm volatile (R"(

   rjmp start_sumval

   .data
   ; any data values/static storage can go here

   .text

start_sumval:
   ; Your assembly language program goes here
   ;
   ; For this example, your program must not modify any registers other than 
   ; r24 and r25. In other words, save and restore all the registers that
   ; are modified by your code.

   ; Tell the compiler to move the arguments a, b into registers 
   ; r24, r25 respectively
   ;
   mov r24,%1
   mov r25,%2
   ;
   ;  --- YOUR CODE GOES HERE ---
   ;       r24 = a, r25 = b
   ;

   add r24,r25 ; add a and b and save in r24
   clr r25 ; clear r25
   brcs cont  ; if carry flag is set, branch to 'cont' section, else continue
   rjmp end_sumval
cont: 
   ldi r25,1 ; if carry flag is set, carry over by setting r25 to 1. This works because addition of 2 8-bit numbers will at most carry over to the 9th bit.
   rjmp end_sumval
   ;
   ;  --- YOUR CODE ENDS ---
   ;      The result must be in the register pair r25:r24
   ;      You can also rjmp to the label end_of_assembly if you need to.
   ;
end_sumval: 
   ; -- move r25:r24 to the 16-bit word in variable out
   movw %0,r24

)" : "=w" (out)  /* out is %0, and a 16-bit variable that is modified
		    by the assembly language */
   : "r" (a), "r" (b)  /* a is %1, b is %2, both are register-sized
			  variables */
   : "r25", "r24");   /* r24, r25 are modified by this assembly
			 language fragment */

  return out;
}

__attribute__((used))
unsigned int diffval(unsigned char a, unsigned char b) {
  unsigned int out;

  asm volatile (R"(

   rjmp start_of_diffval

   .data
   ; any data values/static storage can go here

   .text

start_of_diffval:
   ; Your assembly language program goes here
   ;
   ; For this example, your program must not modify any registers other than 
   ; r24 and r25. In other words, save and restore all the registers that
   ; are modified by your code.

   ; Tell the compiler to move the arguments a, b into registers 
   ; r24, r25 respectively
   ;
   mov r24,%1
   mov r25,%2
   ;
   ;  --- YOUR CODE GOES HERE ---
   ;       r24 = a, r25 = b

   cp r25,r24 ; compare a and b
   brsh else  ; if b is same or higher than a, then we want to branch to return b-a
   sub r24,r25 ; return a-b
   clr r25
   rjmp end_of_diffval
else: 
   sub r25,r24  ; return b-a
   mov r24,r25  ; return value is currently in r25, move to r24
   clr r25
  rjmp end_of_diffval
end_of_diffval: 
   ; -- move r25:r24 to the 16-bit word in variable out
   movw %0,r24

)" : "=w" (out)  /* out is %0, and a 16-bit variable that is modified
		    by the assembly language */
   : "r" (a), "r" (b)  /* a is %1, b is %2, both are register-sized
			  variables */
   : "r25", "r24");   /* r24, r25 are modified by this assembly
			 language fragment */

  return out;
}

__attribute__((used))
unsigned int ourMain(unsigned char a, unsigned char b) {
  unsigned int out;

  asm volatile (R"(

   rjmp start_of_ourMain

   .data
   ; any data values/static storage can go here

   .text

start_of_ourMain:
   ; Your assembly language program goes here
  
   ; Tell the compiler to move the arguments a, b into registers 
   ; r24, r25 respectively
   ;
   mov r24,%1
   mov r25,%2
   
   mov r16,r24 ; store the value of a
   mov r17,r25 ; store the value of b
  
   call start_sumval  ; output of sumval is in registers r24, r25
   mov r9,r25  ; save output into our own regs so they aren't overwritten (upper 8 bits)
   mov r8,r24  ; (lower 8 bits)

   mov %1, r16;  have to reset %1 and %2 as they are changed to sumval result, but they are used later in the diffval, so must restore
   mov %2, r17;  restore inputs

   call start_of_diffval ; output of diffval is in reg r24
   mov %2,r8  ; move the lower 8 bits of return of sumval into input b
   mov %1,r24 ; move the lower 8 bits of return of diffval into input a so that sumval can be called on them
   call start_sumval ; sum lower 8 bits of diffval with lower 8 bits of sumval using sumval
   add r25,r9 ; add the previous carry from first sumval call to current carry of second sumval call
   jmp end_of_ourMain


end_of_ourMain: 
   ; -- move r25:r24 to the 16-bit word in variable out
   movw %0,r24

)" : "=w" (out)  /* out is %0, and a 16-bit variable that is modified
		    by the assembly language */
   : "r" (a), "r" (b)  /* a is %1, b is %2, both are register-sized
			  variables */
   : "r25", "r24");   /* r24, r25 are modified by this assembly
			 language fragment */

  return out;
}
