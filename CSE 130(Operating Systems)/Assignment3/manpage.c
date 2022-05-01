/*********************************************************************
 *
* Copyright (C) 2020-2022 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 *
 ***********************************************************************/

#include "manpage.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>//Needed for sleep()

/*
 * Create a thread for each of seven manpage paragraphs and have them synchronize
 * their invocations of showParagraph() so the manual page displays as follows:
 *
 --------- 8< ---------------------------------------------------
 
A semaphore S is an unsigned-integer-valued variable.
Two operations are of primary interest:
 
P(S): If processes have been blocked waiting on this semaphore,
 wake one of them, else S <- S + 1.
 
V(S): If S > 0 then S <- S - 1, else suspend execution of the calling process.
 The calling process is said to be blocked on the semaphore S.
 
A semaphore S has the following properties:

1. P(S) and V(S) are atomic instructions. Specifically, no
 instructions can be interleaved between the test that S > 0 and the
 decrement of S or the suspension of the calling process.
 
2. A semaphore must be given an non-negative initial value.
 
3. The V(S) operation must wake one of the suspended processes. The
 definition does not specify which process will be awakened.

 --------- 8< ---------------------------------------------------
 *
 * As supplied, shows random single messages.
 */
#define NumOfParagraphs 7
struct ThreadStruct{
  pthread_mutex_t Lock;
  pthread_t Thread;
  int ParagraphToPrint;
}ThreadStruct;
int ReturnThreadThatPrintsParagraph(int ParagraphToPrint, struct ThreadStruct *StructArray[]);
void *ThreadedManpage();

void manpage() 
{
  struct ThreadStruct *StructArray[NumOfParagraphs];//Array that holds all created structs

  for(int i=0;i<NumOfParagraphs;i++){//Create locks and structs
    struct ThreadStruct *NewestStruct=malloc(sizeof(ThreadStruct));
    pthread_mutex_init(&NewestStruct->Lock,NULL);
    pthread_mutex_lock(&NewestStruct->Lock);
    NewestStruct->ParagraphToPrint=-1;//Default value
    StructArray[i]=NewestStruct;
  }
  for(int i=0;i<NumOfParagraphs;i++){//Create threads
    pthread_create(&StructArray[i]->Thread,NULL,&ThreadedManpage,(void*)StructArray[i]);
  }
  for(int i=0;i<NumOfParagraphs;i++){//Release locks in order
    int IndexForCorrectThread=ReturnThreadThatPrintsParagraph(i,&StructArray[0]);
    pthread_mutex_unlock(&StructArray[IndexForCorrectThread]->Lock);//First needs to find the thread that holds the right lock
    pthread_join(StructArray[IndexForCorrectThread]->Thread,NULL);//Ensures that corresponding thread finishes before releasing next lock
  }
  for(int i=0;i<NumOfParagraphs;i++){//Free all structs
    pthread_mutex_destroy(&StructArray[i]->Lock);
    free(StructArray[i]);
  }
  return;
}

//Function all threads run 
void *ThreadedManpage(void * StructArg)
{
  struct ThreadStruct *ThisStruct=StructArg;
  ThisStruct->ParagraphToPrint=getParagraphId();//Calculates which paragraph it is printing
  pthread_mutex_lock(&ThisStruct->Lock);//Waits on lock specified by pid
  //Only reaches here after specific lock has been released
  showParagraph();//Shows corresponding paragraph
  pthread_mutex_unlock(&ThisStruct->Lock);
  pthread_exit(NULL);
}

//Calculates the index of which thread should run next to print the next paragraph
int ReturnThreadThatPrintsParagraph(int ParagraphToPrint, struct ThreadStruct *StructArray[]){
  int ThreadIndex=0;
  for(int i=0;i<NumOfParagraphs;i++){
    while(StructArray[i]->ParagraphToPrint==-1){//Thread has not calculated which paragraph it is printing
      //Need to sleep to ensure no race condition
      sleep(.20);//Only sleeps for 1/5th of a second which by itself should be more than long enough, but this is a while loop, so it will continue doing so until the corresponding thread has actually finished
    }
    if(StructArray[i]->ParagraphToPrint==ParagraphToPrint){
      ThreadIndex=i;
      break;
    }
  }
  return ThreadIndex;
}
