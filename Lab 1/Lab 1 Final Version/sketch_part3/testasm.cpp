#include "testasm.h"

__attribute__((used)) unsigned int fib(unsigned char a, unsigned char b)
{
   unsigned int out;

   asm volatile(R"(

   rjmp start_of_fib

   .data
   ; any data values/static storage go here

   .text

start_of_fib:
   ; Tell the compiler to move the arguments n into a register
    mov r24,%1
    push r24; push n onto the stack

   ; recurse n times:
   cpi r24,0  ; n = 0
   breq case0
   cpi r24,1  ; n = 1
   breq case1

   pop r21; retireve the original n value from the stack
   mov r24,r21; move n into r24
   push r21 ; put the original n back on stack
   subi r24,1 ; decrement n (the one in r24)
   mov %1, r24; put decremented n into the input a
   call start_of_fib; find fib(n-1)
   
   mov r8, r24; store the return value of fib(n-1)
   pop r24; retrieve the original n value from the stack
   push r8; push the result of fib(n-1)
   subi r24,2 ; decrement n by 2 for fib(n-2)
   mov %1,r24 ; put decremented n into the input a
   call start_of_fib ; find fib(n-2)
   
   pop r8; retrieve the return value of fib(n-1) from the stack
   add r24,r8; add fib(n-2) with fib(n-1)   
   rjmp end_of_fib

   
case0:  ; return 0 if n = 0
   clr r24  ; return 0 
   clr r25
   pop r28 ; pop off n=0 from the stack
   rjmp end_of_fib
case1:  ; return 1 if n = 1
   ldi r24,1
   clr r25
   pop r28 ; pop off n=1 from the stack
   rjmp end_of_fib

end_of_fib: 
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
