/* This file is derived from source code for the Nachos
   instructional operating system.  The Nachos copyright notice
   is reproduced in full below. */

/* Copyright (c) 1992-1996 The Regents of the University of California.
   All rights reserved.

   Permission to use, copy, modify, and distribute this software
   and its documentation for any purpose, without fee, and
   without written agreement is hereby granted, provided that the
   above copyright notice and the following two paragraphs appear
   in all copies of this software.

   IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO
   ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
   CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE
   AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA
   HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
   PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
   BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
   PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
   MODIFICATIONS.
*/

#include "threads/semaphore.h"
#include <stdio.h>
#include <string.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

/* Initializes semaphore SEMA to VALUE.  A semaphore is a
   nonnegative integer along with two atomic operators for
   manipulating it:

   - down or "P": wait for the value to become positive, then
     decrement it.

   - up or "V": increment the value (and wake up one waiting
     thread, if any). */
void
semaphore_init (struct semaphore *sema, unsigned value) 
{
  ASSERT (sema != NULL);

  sema->value = value;
  sema->MaxPriority=-1;//EMPTY
  sema->MinPriority=-1;//EMPTY
  list_init (&sema->waiters);
}

/* Down or "P" operation on a semaphore.  Waits for SEMA's value
   to become positive and then atomically decrements it.

   This function may sleep, so it must not be called within an
   interrupt handler.  This function may be called with
   interrupts disabled, but if it sleeps then the next scheduled
   thread will probably turn interrupts back on. */
void
semaphore_down (struct semaphore *sema) 
{
  enum intr_level old_level;

  ASSERT (sema != NULL);
  ASSERT (!intr_context ());

  old_level = intr_disable ();
  while (sema->value == 0) 
    {
      //MINE
      //Inserts according to priority
      if(thread_get_priority()>sema->MinPriority){//Need to set min pri
        sema->MinPriority=thread_get_priority();
      }
      list_insert_ordered(&sema->waiters,&thread_current ()->sharedelem,&ThreadLessPriority,NULL);
      if(thread_current()->priority>sema->MaxPriority){
        sema->MaxPriority=thread_current()->priority;//Set new max priority
      }
      CalcNewMaxPriority(sema);//Is potentially unnecessary, but still including it just in case, worst case scenario does nothing
      //MINE
      thread_block ();
    }
  sema->value--;
  intr_set_level (old_level);
}

/* Up or "V" operation on a semaphore.  Increments SEMA's value
   and wakes up one thread of those waiting for SEMA, if any.

   This function may be called from an interrupt handler. */
void
semaphore_up (struct semaphore *sema) 
{
  enum intr_level old_level;

  ASSERT (sema != NULL);

  old_level = intr_disable ();
  sema->value++;
  //MINE
  struct thread *NewThread=NULL;
  if (!list_empty (&sema->waiters)){
    list_sort(&sema->waiters,&ThreadLessPriority,NULL);//Sort incase priorities changed
    NewThread=list_entry (list_pop_front (&sema->waiters),struct thread, sharedelem);
    thread_unblock(NewThread);//Does not yield
    sema->MaxPriority=CalcNewMaxPriority(sema);//Recalc max pri here
    if(list_size(&sema->waiters)>1){//Calc new min priority for front of sema
      sema->MinPriority=CalcNewMaxPriority(sema);
    }
    else{
      sema->MinPriority=-1;
    }
    if(NewThread->priority>thread_get_priority()){//Need to switch threads
      thread_yield();
    }
  } 
  //MINE
  intr_set_level (old_level);
}

//NEED TO RESET SEMA TO -1 SO THAT REMOVING DOESN'T BORK EVERYTHING
int CalcNewMaxPriority(struct semaphore *Sema){
  int NewMaxPriority=Sema->MaxPriority=-1;//Default value for empty
  if(list_size(&Sema->waiters)!=0){
    for(struct list_elem* IteratorElem=list_begin(&Sema->waiters);IteratorElem!=list_tail(&Sema->waiters);IteratorElem=list_next(IteratorElem)){
      struct thread *IteratorThread=list_entry(IteratorElem,struct thread,sharedelem);
      if(IteratorThread->priority>NewMaxPriority){
        Sema->MaxPriority=NewMaxPriority=IteratorThread->priority;
      }
    }
  }
  return NewMaxPriority;
}