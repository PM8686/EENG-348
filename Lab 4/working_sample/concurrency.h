/*************************************************************************
 *
 *  Copyright (c) 2013--2018 Rajit Manohar
 *  All Rights Reserved
 *
 **************************************************************************
 */


#ifndef __CONCURRENCY_H__
#define __CONCURRENCY_H__


#ifdef __cplusplus
extern "C" {
#endif

enum Colors {BLUE=6, YELLOW=8, GREEN=10, WHITE=12};

typedef struct process_state process_t;
   /* opaque definition of process type; you must provide this
      implementation.
   */


/*------------------------------------------------------------------------
   THE FOLLOWING FUNCTIONS MUST BE PROVIDED.
------------------------------------------------------------------------*/

/* ====== Part 1 ====== */
int queueTest(int n);

extern process_t *current_process; 
/* the currently running process */

typedef struct process_queue process_queue_t;
struct process_queue {
    // unsigned int len;
    struct process_state *head;
    // struct process_state *tail;
};

__attribute__((used)) unsigned int process_select (unsigned int cursp);
/* Called by the runtime system to select another process.
   "cursp" = the stack pointer for the currently running process
*/

void process_start (void);
/* Starts up the concurrent execution */

int process_create (void (*f)(void), int n);
/* Create a new process */



/* ===== Part 2 ====== */

typedef struct lock_state lock_t;
  /* you have to define the lock_state structure */
// lock structure
struct lock_state {
    process_t *holder;
    process_queue_t *waitingQueue;
};

void lock_init (lock_t *l);
void lock_acquire (lock_t *l);
void lock_release (lock_t *l);


/*-- functions provided in the .c file --*/

unsigned int process_init (void (*f) (void), int n, process_t *p);
void process_begin ();
void yield ();

#ifdef __cplusplus
}
#endif

#endif /* __CONCURRENCY_H__ */