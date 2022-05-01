/************************************************************************
 *
 * CSE130 Assignment 4
 *
 * Copyright (C) 2021-2022 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 * 
 ************************************************************************/

/**
 * See scheduler.h for function details. All are callbacks; i.e. the simulator 
 * calls you when something interesting happens.
 */
#include <stdlib.h>
#include <stdio.h>//DEBUGGING

#include "queue.h"
#include "simulator.h"
#include "scheduler.h"

void *ReadyQueue=NULL;
void *BlockedQueue=NULL;
void *HeldStatsQueue=NULL;
void *WaitingStatsQueue=NULL;//ONLY FOR STATS
void *TurnAroundStatsQueue=NULL;//ONLY FOR STATS
thread_t *ActiveThread=NULL;

stats_t* ReturnedStats=NULL;

int Algo=0;
unsigned int Quantum=0;
int StartOfFirstThreadTick=-1;//Used only for rr

static bool FindStatInQueue(void *ThisStats, void *WantedTid);
void WaitingQueueIterate(void *Stat);
void TurnaroundQueueIterate(void *Stat);
void InsertQueue(void* Queue,thread_t *Thread);
thread_t *PickActive();
void AutoDispatch();
static int ThreadPriorityComp(void *a, void *b);
static int ShortestJobLengthComp(void *a, void *b);


/**
 * Initalise a scheduler implemeting the requested ALGORITHM. QUANTUM is only
 * meaningful for ROUND_ROBIN.
 */
void scheduler(enum algorithm algorithm, unsigned int quantum) {
  //printf("-Scheduler!\n");
  ReadyQueue=queue_create();
  BlockedQueue=queue_create();
  HeldStatsQueue=queue_create();
  WaitingStatsQueue=queue_create();
  TurnAroundStatsQueue=queue_create();
  Algo=algorithm;
  Quantum=quantum;
  //Create stats obj and default values
  ReturnedStats=(stats_t*)malloc(sizeof(stats_t));
  ReturnedStats->waiting_time=0;
  ReturnedStats->turnaround_time=0;
  ReturnedStats->tid=0;
  ReturnedStats->thread_count=0;
}

/**
 * Thread T is ready to be scheduled for the first time.
 */
void sys_exec(thread_t *t) {
  //printf("-%i thread created\n",t->tid);
  //Create this threads stats and attach to queue
  ReturnedStats->thread_count++;
  stats_t *ThreadStat=(stats_t*)malloc(sizeof(stats_t));
  ThreadStat->waiting_time=0;
  ThreadStat->turnaround_time=0;
  ThreadStat->tid=t->tid;
  queue_enqueue(HeldStatsQueue,ThreadStat);
  queue_enqueue(TurnAroundStatsQueue,ThreadStat);
  queue_enqueue(WaitingStatsQueue,ThreadStat);
  InsertQueue(ReadyQueue,t);
  if(StartOfFirstThreadTick==-1){//RR
    StartOfFirstThreadTick=sim_time();
    //printf("\nCHOSE NEW START OF FIRST THREAD TICK OF %u\n\n\n",sim_time());
  }
}

/**
 * Programmable clock interrupt handler. Call sim_time() to find out
 * what tick this is. Called after all calls to sys_exec() for this
 * tick have been made.
 */
void tick() {
  AutoDispatch();
  queue_iterate(WaitingStatsQueue,WaitingQueueIterate);
  queue_iterate(TurnAroundStatsQueue,TurnaroundQueueIterate);
}

/**
 * Thread T has completed execution and should never again be scheduled.
 */
void sys_exit(thread_t *t) {
  //printf("-%i thread exited\n",t->tid);
  queue_remove(ReadyQueue,t);
  queue_remove(TurnAroundStatsQueue,queue_find(TurnAroundStatsQueue,FindStatInQueue,&(t->tid)));
  queue_remove(WaitingStatsQueue,queue_find(WaitingStatsQueue,FindStatInQueue,&(t->tid)));
  if(t==ActiveThread){
    ActiveThread=NULL;
  }
}

/**
 * Thread T has requested a read operation and is now in an I/O wait queue.
 * When the read operation starts, io_starting(T) will be called, when the
 * read operation completes, io_complete(T) will be called.
 */
void sys_read(thread_t *t) {
  //printf("-%i thread CALLED FOR READ\n",t->tid);
  if(t==ActiveThread){
    //printf("\nREMOVING THREAD %i",ActiveThread->tid);
    queue_remove(ReadyQueue,t);
    //printf(" AND PUSHING NEW THREAD %i ON\n\n",((thread_t*)queue_head(ReadyQueue))->tid);
    AutoDispatch();
    ActiveThread=NULL;
  }
  else{
    //printf("\nOPTING TO NOT PUSH NEW THREAD\n\n");
    queue_remove(ReadyQueue,t);
  }
  queue_enqueue(BlockedQueue,t);
  //queue_enqueue(WaitingStatsQueue,queue_find(HeldStatsQueue,FindStatInQueue,&(t->tid)));
}

/**
 * Thread T has requested a write operation and is now in an I/O wait queue.
 * When the write operation starts, io_starting(T) will be called, when the
 * write operation completes, io_complete(T) will be called.
 */
void sys_write(thread_t *t) {
  //printf("-%i thread CALLED FOR WRITE\n",t->tid);
  if(t==ActiveThread){
    //printf("\nREMOVING THREAD %i",ActiveThread->tid);
    queue_remove(ReadyQueue,t);
    //printf(" AND PUSHING NEW THREAD %i ON\n\n",((thread_t*)queue_head(ReadyQueue))->tid);
    AutoDispatch();
    ActiveThread=NULL;
  }
  else{
    //printf("\nOPTING TO NOT PUSH NEW THREAD\n\n");
    queue_remove(ReadyQueue,t);
  }
  queue_enqueue(BlockedQueue,t);
  //queue_enqueue(WaitingStatsQueue,queue_find(HeldStatsQueue,FindStatInQueue,&(t->tid)));
}

/**
 * An I/O operation requested by T has completed; T is now ready to be 
 * scheduled again.
 */
void io_complete(thread_t *t) {
  //printf("-%i thread completed io\n",t->tid);
  queue_enqueue(ReadyQueue,t);
  stats_t *Temp=NULL;
  Temp=queue_find(HeldStatsQueue,FindStatInQueue,&(t->tid));
  queue_enqueue(WaitingStatsQueue,Temp);
  Temp->waiting_time--;
}

/**
 * An I/O operation requested by T is starting; T will not be ready for
 * scheduling until the operation completes.
 */
void io_starting(thread_t *t) {
  //printf("-%i thread started io\n",t->tid);
  queue_remove(BlockedQueue,t);
  queue_remove(WaitingStatsQueue,queue_find(WaitingStatsQueue,FindStatInQueue,&(t->tid)));
}

/**
 * Return dynamically allocated stats for the scheduler simulation, see 
 * scheduler.h for details. Memory allocated by your code will be free'd
 * by the similulator. Do NOT return a pointer to a stack variable.
 */
stats_t *stats() {
  //printf("STATS!\n");
  queue_destroy(ReadyQueue);
  queue_destroy(BlockedQueue);
  queue_destroy(WaitingStatsQueue);
  queue_destroy(TurnAroundStatsQueue);
  ReturnedStats->tstats=malloc((ReturnedStats->thread_count)*sizeof(stats_t));
  for(int i=0;i<(ReturnedStats->thread_count);i++){
    ReturnedStats->tstats[i]=*(stats_t*)queue_dequeue(HeldStatsQueue);
    ReturnedStats->waiting_time+=ReturnedStats->tstats[i].waiting_time;
    ReturnedStats->turnaround_time+=ReturnedStats->tstats[i].turnaround_time;
  }
  //Calculate averages
  ReturnedStats->waiting_time=(ReturnedStats->waiting_time)/(ReturnedStats->thread_count);
  ReturnedStats->turnaround_time=(ReturnedStats->turnaround_time)/(ReturnedStats->thread_count);
  queue_destroy(HeldStatsQueue);
  return ReturnedStats; 
}

//Presumes thread is present NOWHERE ELSE
void InsertQueue(void* Queue,thread_t *Thread){
  switch (Algo){
    case 0://First Come First Serve
      queue_enqueue(Queue,Thread);
      return;
    case 1://Round Robin
      queue_enqueue(Queue,Thread);
      return;
    case 2:
      queue_enqueue(Queue,Thread);
      queue_sort(ReadyQueue,ThreadPriorityComp);
      return;
    case 3:
      queue_enqueue(Queue,Thread);
      queue_sort(ReadyQueue,ThreadPriorityComp);
      return;
    case 4:
      queue_enqueue(Queue,Thread);
      queue_sort(ReadyQueue,ShortestJobLengthComp);
      return;
    case 5:
      queue_enqueue(Queue,Thread);
      queue_sort(ReadyQueue,ShortestJobLengthComp);
      return;
    case 6:
      queue_enqueue(Queue,Thread);//Not correct
      return;
    case 7:
      queue_enqueue(Queue,Thread);//Not correct
      return;
    default://ERROR
      return;
  }
}

//Recalculates what active thread should be and returns it
//Used for optionally resorting ready queue
thread_t *PickActive(){
  switch (Algo){
    case 0://First Come First Serve
      ActiveThread=queue_head(ReadyQueue);
      return ActiveThread;
    case 1://Round Robin
      if((sim_time()-StartOfFirstThreadTick)%Quantum==0 && queue_size(ReadyQueue)>1){//Replace with new thread
        //printf("-Pushing thread %i to back of queue\n",((thread_t*)queue_head(ReadyQueue))->tid);
        queue_enqueue(ReadyQueue,queue_dequeue(ReadyQueue));
      }
      //printf("-Making thread %i the new head of queue",((thread_t*)queue_head(ReadyQueue))->tid);
      ActiveThread=queue_head(ReadyQueue);
      return ActiveThread;
    case 2://NonPreemptive Priority
      queue_sort(ReadyQueue,ThreadPriorityComp);
      if(ActiveThread!=NULL){
        return ActiveThread;
      }
      else{
        ActiveThread=queue_head(ReadyQueue);
        return ActiveThread;
      }
    case 3://Preemptive Priority
      queue_sort(ReadyQueue,ThreadPriorityComp);
      ActiveThread=queue_head(ReadyQueue);
      return ActiveThread;
    case 4://NonPreemptive Shortest Job First
      queue_sort(ReadyQueue,ShortestJobLengthComp);
      if(ActiveThread!=NULL){
        return ActiveThread;
      }
      else{
        ActiveThread=queue_head(ReadyQueue);
        return ActiveThread;
      }
    case 5://Preemptive Shortest Job First
      queue_sort(ReadyQueue,ShortestJobLengthComp);
      ActiveThread=queue_head(ReadyQueue);
      return ActiveThread;
    case 6://NonPreemptive Shortest Time Left First
      ActiveThread=queue_head(ReadyQueue);//Not correct
      return ActiveThread;//Not correct
    case 7://Preemptive Shortest Time Left First
      ActiveThread=queue_head(ReadyQueue);//Not correct
      return ActiveThread;//Not correct
    default://ERROR
      return NULL;
  }
}

void WaitingQueueIterate(void *Stat){
  ((stats_t*)Stat)->waiting_time++;
  //printf("-Waiting incremented %i\n",((stats_t*)Stat)->tid);
  return;
}

void TurnaroundQueueIterate(void *Stat){
  ((stats_t*)Stat)->turnaround_time++;
  //printf("-Turnaround incremented %i\n",((stats_t*)Stat)->tid);
  return;
}

static bool FindStatInQueue(void *ThisStats, void *WantedTid){
  return ((stats_t*)ThisStats)->tid == *(int*)WantedTid;
}

void AutoDispatch(){
  thread_t *Pre,*Post, *Mid;
  Pre=Post=NULL;
  Pre=ActiveThread;
  Mid=PickActive();
  if(ActiveThread!=NULL)
    sim_dispatch(Mid);//Dispatches a potentially new thread
  Post=ActiveThread;
  if(Pre!=NULL){//Nothing to remove
    if(Pre!=Post){//No change on the cpu
      stats_t *Temp=queue_find(TurnAroundStatsQueue,FindStatInQueue,&(Pre->tid));
      if(Temp!=NULL){
        //printf("-Added thread %i onto waiting queue as it is off the cpu\n",Temp->tid);
        queue_enqueue(WaitingStatsQueue,Temp);
      }
      //printf("-Removed thread %i from waiting queue as it is put on the cpu\n",Post->tid);
      queue_remove(WaitingStatsQueue,queue_find(WaitingStatsQueue,FindStatInQueue,&(Post->tid)));
    }
  }
  else{
    //printf("-Removed thread %i from waiting queue as it is on the cpu now\n",ActiveThread->tid);
    queue_remove(WaitingStatsQueue,queue_find(WaitingStatsQueue,FindStatInQueue,&(ActiveThread->tid)));
  }
  return;
}

static int ThreadPriorityComp(void *a, void *b){
  return ((thread_t*)a)->priority > ((thread_t*)b)->priority;
}

static int ShortestJobLengthComp(void *a, void *b){
  return ((thread_t*)a)->length > ((thread_t*)b)->length;
}