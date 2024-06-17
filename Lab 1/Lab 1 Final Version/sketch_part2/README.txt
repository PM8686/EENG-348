We did not change the stack frame for Parts 1 and 2. 
The manipulations we did to the stack frame for Part 3 were the following:
1. Push the value of n onto the stack
- if n=0 or n=1, then we pop n of the stack while returning 0 or 1 respectively
2. For the (n-1) case, we retrieve n from the stack, save it into another register for use, and push the original n back onto the stack to be used later in the (n-2) case
3. For the (n-2) case, we pop the original n value from the stack
4. We push the result of the (n-1) case onto the stack
5. After the (n-2) result is determined, we pop the result of the (n-1) case from the stack, add them together, and finally return

Thanks for reading!
