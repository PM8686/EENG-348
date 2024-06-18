#include "testasm.h"

/* the attribute "used" tells the compiler to not inline this function */
__attribute__((used)) unsigned int testasm(unsigned char a, unsigned char b)
{
   unsigned int out;

   asm volatile(R"(

   rjmp start_of_assembly

   .data
   ; any data values/static storage goes here

   .text

start_of_assembly:

   ; Tell the compiler to move the arguments a, b into registers 
   ; r24, r25 respectively
   mov r24,%1
   mov r25,%2

   add r24,r25 ; add b to a and save in r24
   clr r25 ; clear r25
   brcs next  ; if carry flag is set, branch to 'next' section, else continue
   rjmp end_of_assembly
next: 
   ldi r25,1  ; if carry flag is set, carry over by setting r25 to 1. This works because addition of 2 8-bit numbers will at most carry over to the 9th bit.
   rjmp end_of_assembly

   ;      The result is in the register pair r25:r24
end_of_assembly: 
   ; -- move r25:r24 to the 16-bit word in variable out
   movw %0,r24

)" : "=w"(out)                   /* out is %0, and a 16-bit variable that is modified
                          by the assembly language */
                : "r"(a), "r"(b) /* a is %1, b is %2, both are register-sized
                     variables */
                : "r25", "r24"); /* r24, r25 are modified by this assembly
                     language fragment */

   return out;
}
