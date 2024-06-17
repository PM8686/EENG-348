#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "concurrency.h"

__attribute__((used)) unsigned char _orig_sp_hi, _orig_sp_lo;
__attribute__((used)) unsigned char _save_sp_hi, _save_sp_lo;

// process data struct
struct process_state {
    unsigned int sp;
    struct process_state* next;
    int is_waiting; // false = 0, true = 1
};

#define false 0
#define true 1
 
// queue struct
struct queue {
    process_t *head;
};

// current running process
process_t *current_process = NULL;

// QUEUE STUFF
queue_t *ready_queue = NULL;

// queue in the process into the queue at the end
void enqueue(process_t *process, queue_t *queue)
{
  // end of queue must remain NULL
  process->next = NULL;
  // insert the new process at the end of the queue
  if (queue->head == NULL)queue->head = process;

  else 
  {
    // traverse the queue until at the last element
    process_t *tmp = queue->head;
    while(tmp->next != NULL) tmp = tmp->next;
    // append process to the end of the queue
    tmp->next = process;
  }

  return;
}


// If there is no process in the queue returns NULL
// else return the next process.
process_t *dequeue(queue_t *queue)
{
  // if the queue is empty, return NULL
  if(queue->head == NULL) return NULL;

  // else remove and return the first element in the queue
  process_t *tmp = queue->head;
  queue->head = tmp->next;
  tmp->next = NULL;
  return tmp;
}

// IMPLEMENTED FUNCTIONS
/*
 The code we provide will call process_select() with interrupts disabled. 
 The argument to process_select() is the value of the stack pointer for the currently running process. 
 If there are no currently running processes, the argument will be 0. If the currently running process 
 has terminated, the argument to process_select() will also be 0. If there is no process ready to 
 execute, process_select() should return 0; if there is a ready process, process_select() must return 
 the value of the stack pointer for the next ready process.
*/
__attribute__((used)) unsigned int process_select (unsigned int cursp) {
	if (cursp == 0) 
  {
    // free the process that just terminated
    if (current_process != NULL) free(current_process);
    // current_process must be set to NULL when a process terminates
    current_process = NULL;   
  }

	//if current process is not finished, put back on queue
	else 
  { 
    // update current process and requeue
		current_process->sp = cursp;
		if (current_process->is_waiting == false) enqueue(current_process, ready_queue);
	}
  
	// dequeue next process
	current_process = dequeue(ready_queue);
	if (current_process == NULL) return 0; // if no next process to run, return 0
	return current_process->sp;
}

/*
 This function starts the execution of concurrent processes. 
 This function must initialize any data structures you need to implement concurrency, 
 and start the execution of the first ready process. You can start the execution of the first process 
 by calling the function process_begin () that we have provided.
*/
void process_start (void) {
  current_process = NULL; 
	process_begin();
}

/*
 This function creates a process that starts at function f(), with an initial stack size that can hold n bytes.
 This function should return -1 on an error, and 0 on success. 
*/
int process_create (void (*f)(void), int n) {

	asm volatile ("cli\n\t"); //disable interrupts

  // initialize the ready queue if necessary
  if (current_process == NULL && ready_queue == NULL)
  {
    ready_queue = malloc(sizeof(queue_t));
    ready_queue->head = NULL;
  }

  // initialize a process
	process_t *process = malloc(sizeof(process_t));
	if (process == NULL) return -1; // if there is an error return -1s
  unsigned int sp = process_init(f, n);
	process->sp = sp;
	process->next = NULL;
  process->is_waiting = false;

  // queue the processes into the ready queue
	enqueue(process, ready_queue);
	asm volatile ("sei\n\t"); //enable interrupts
  return 0;  // return 0 on success
}

// This is called to initialize the lock data structure.
void lock_init (lock_t *l)
{
  // at initilization, the lock is not locked and there are no processes waiting
  l->locked = false;
  l->waiting = malloc(sizeof(queue_t));
  if (l->waiting == NULL) return;
  l->waiting->head = NULL;
}

/* The process should acquire the lock if it is free; otherwise it should block. 
 Remember, you can switch execution to another process by calling the yield() 
 function, which will indirectly call your implementation of process_select() 
 to pick the next ready process. */
void lock_acquire (lock_t *l)
{
  asm volatile ("cli\n\t"); // disable interrupts
  if (l->locked == false) l->locked = true; // acquires lock if it is free
  else 
  {
    // otherwise, block execution, and put process into the waiting queue
    current_process->is_waiting = true;
    enqueue(current_process, l->waiting); 
    yield(); // immediately select a new process
  }
  asm volatile ("sei\n\t"); // enable interrupts
}

// This releases the lock.
void lock_release (lock_t *l)
{
  asm volatile ("cli\n\t"); // disable interrups
  // if someone is waiting on the lock, pass the lock onto them
  if (l->waiting->head != NULL)
  {
    process_t *next_process = dequeue(l->waiting);
    next_process->is_waiting = false;
    enqueue(next_process, ready_queue); // put the now ready process in the ready queue
  }
  // if no one is waiting, release the lock
  else {
    l->locked = false;
  }
  asm volatile ("sei\n\t"); // enable interrupts
}




// NOT OURS
__attribute__((used)) void *process_malloc(unsigned int sz)
{
 if (!current_process) return (void *)malloc (sz);
 asm volatile ( "in r23,__SP_L__ \n\t"
                "sts _save_sp_lo, r23\n\t"
                "in r23,__SP_H__ \n\t"
                "sts _save_sp_hi, r23\n\t"
                "lds r23, _orig_sp_hi\n\t"
                "out __SP_H__, r23\n\t"
                "lds r23, _orig_sp_lo\n\t"
                "out __SP_L__, r23\n\t"
                "call malloc\n\t"
                "lds r23, _save_sp_hi\n\t"
                "out __SP_H__, r23\n\t"
                "lds r23, _save_sp_lo\n\t"
                "out __SP_L__, r23\n\t"
                "ret\n\t");
}

__attribute__((used)) void process_begin ()
{
  asm volatile (
		"cli \n\t"
		"in r24,__SP_L__ \n\t"
		"sts _orig_sp_lo, r24\n\t"
		"in r25,__SP_H__ \n\t"
		"sts _orig_sp_hi, r25\n\t"
		"ldi r24, 0\n\t"
		"ldi r25, 0\n\t"
		"jmp .dead_proc_entry\n\t"
		);
}

__attribute__((used)) void process_terminated ()
{
  asm volatile (
		"cli\n\t"
		"lds r25, _orig_sp_hi\n\t"
		"out __SP_H__, r25\n\t"
		"lds r24, _orig_sp_lo\n\t"
		"out __SP_L__, r24\n\t"
		"ldi r24, lo8(0)\n\t"
		"ldi r25, hi8(0)\n\t"
		"jmp .dead_proc_entry"
		);
}

void process_timer_interrupt ();

__attribute__((used)) void yield ()
{
  if (!current_process) return;
  asm volatile ("cli\n\t");
  asm volatile ("rjmp process_timer_interrupt\n\t");
}

__attribute__((used)) void process_timer_interrupt()
{
  asm volatile (
		"push r31\n\t"
		"push r30\n\t"
		"push r29\n\t"
		"push r28\n\t"
		"push r27\n\t"
		"push r26\n\t"
		"push r25\n\t"
		"push r24\n\t"
		"push r23\n\t"
		"push r22\n\t"
		"push r21\n\t"
		"push r20\n\t"
		"push r19\n\t"
		"push r18\n\t"
		"push r17\n\t"
		"push r16\n\t"
		"push r15\n\t"
		"push r14\n\t"
		"push r13\n\t"
		"push r12\n\t"
		"push r11\n\t"
		"push r10\n\t"
		"push r9\n\t"
		"push r8\n\t"
		"push r7\n\t"
		"push r6\n\t"
		"push r5\n\t"
		"push r4\n\t"
		"push r3\n\t"
		"push r2\n\t"
		"push r1\n\t"
		"push r0\n\t"
		"in r24, __SREG__\n\t"
		"push r24\n\t"
		"in r24, __SP_L__\n\t"
		"in r25, __SP_H__\n\t"
		".dead_proc_entry:\n\t"
		"rcall process_select\n\t"
		"eor r18,r18\n\t"
		"or r18,r24\n\t"
		"or r18,r25\n\t"
		"brne .label_resume\n\t"
		"lds r25, _orig_sp_hi\n\t"
		"out __SP_H__, r25\n\t"
		"lds r24, _orig_sp_lo\n\t"
		"out __SP_L__, r24\n\t"
		"ret\n\t"
		".label_resume:\n\t"
		"out __SP_L__, r24\n\t"
		"out __SP_H__, r25\n\t"
		"pop r0\n\t"
		"out  __SREG__, r0\n\t"
		"pop r0\n\t"
		"pop r1\n\t"
		"pop r2\n\t"
		"pop r3\n\t"
		"pop r4\n\t"
		"pop r5\n\t"
		"pop r6\n\t"
		"pop r7\n\t"
		"pop r8\n\t"
		"pop r9\n\t"
		"pop r10\n\t"
		"pop r11\n\t"
		"pop r12\n\t"
		"pop r13\n\t"
		"pop r14\n\t"
		"pop r15\n\t"
		"pop r16\n\t"
		"pop r17\n\t"
		"pop r18\n\t"
		"pop r19\n\t"
		"pop r20\n\t"
		"pop r21\n\t"
		"pop r22\n\t"
		"pop r23\n\t"
		"pop r24\n\t"
		"pop r25\n\t"
		"pop r26\n\t"
		"pop r27\n\t"
		"pop r28\n\t"
		"pop r29\n\t"
		"pop r30\n\t"
		"pop r31\n\t"
		"reti\n\t");
}


/*
 * Stack: save 32 regs, +2 for entry point +2 for ret address
 */
#define EXTRA_SPACE 37
#define EXTRA_PAD 4

unsigned int process_init (void (*f) (void), int n)
{
  unsigned long stk;
  int i;
  unsigned char *stkspace;

  /* Create a new process */
  n += EXTRA_SPACE + EXTRA_PAD;
  stkspace = (unsigned char *) process_malloc (n);

  if (stkspace == NULL) {
    /* failed! */
    return 0;
  }

  /* Create the "standard" stack, including entry point */
  for (i=0; i < n; i++) {
      stkspace[i] = 0;
  }

  n -= EXTRA_PAD;

  stkspace[n-1] = ( (unsigned int) process_terminated ) & 0xff;
  stkspace[n-2] = ( (unsigned int) process_terminated ) >> 8;
  stkspace[n-3] = ( (unsigned int) f ) & 0xff;
  stkspace[n-4] = ( (unsigned int) f ) >> 8;

  /* SREG */
  stkspace[n-EXTRA_SPACE] = SREG;

  stk = (unsigned int)stkspace + n - EXTRA_SPACE - 1;

  return stk;
}
