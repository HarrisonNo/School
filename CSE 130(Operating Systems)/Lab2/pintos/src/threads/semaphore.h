#ifndef THREADS_SEMAPHORE_H
#define THREADS_SEMAPHORE_H

#include <list.h>
#include <stdbool.h>

/* A Counting Semaphore. */
struct semaphore {
  unsigned value;             /* Current value. */
  struct list waiters;        /* List of waiting threads. */
  int MaxPriority;//MINE
  int MinPriority;
};

int CalcNewMaxPriority(struct semaphore *Sema);//MINE

void semaphore_init (struct semaphore *, unsigned value);
void semaphore_down (struct semaphore *);
void semaphore_up (struct semaphore *);

int CalcSecondMaxPriority(struct semaphore *Sema);//MINE

#endif /* threads/semaphore.h */
