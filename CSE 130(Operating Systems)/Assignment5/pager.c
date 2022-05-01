/************************************************************************
 *
 * CSE130 Assignment 5
 *
 * Copyright (C) 2021-2022 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 * 
 ************************************************************************/

#include "simulator.h"
#include "queue.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>//DEBUGGING

void *FrameQueue; 

int Algo;
//This struct is disgustingly inefficient in terms of memory use
//But on the flip side it is also slightly faster in terms of run speed
//But this is still really bad and I would fix it if I were not so lazy
//This could 100% be just 2 variables as oppposed to 5
struct FrameStruct{
    unsigned int Page;
    int TimesUsed;
    int LastUsed;
    int ChancesLeft;
    int Circle;
};

static bool FrameHasPage(void *Frame, void *Page);

/**
 * Initialise your ALGORITHM pager with FRAMES frames (physical pages).
 */
void pager_init(enum algorithm algorithm, unsigned int frames) {
    Algo=algorithm;
    FrameQueue=queue_create();//Create frame
    for(int i=0;i<frames;i++){
        struct FrameStruct *ThisFrame=(struct FrameStruct*)malloc(sizeof(struct FrameStruct));
        ThisFrame->Page=-1;
        ThisFrame->LastUsed=-1;
        ThisFrame->TimesUsed=0;
        ThisFrame->ChancesLeft=0;
        ThisFrame->Circle=1;
        queue_enqueue(FrameQueue,ThisFrame);//Insert into frame here
    }
    return;
}

/**
 * Free any dynamically allocated resources.
 */
void pager_destroy() {
    while(queue_size(FrameQueue)>0){
        struct FrameStruct *Frame=queue_dequeue(FrameQueue);
        free(Frame);
    }
    queue_destroy(FrameQueue);
    return;
}

/**
 * A request has been made for virtual page PAGE. If your pager does
 * not currently have PAGE in physical memory, request it from the 
 * simulator.
 */
//This is also really bad, PAY NO ATTENTION TO THE MAN BEHIND THE CURTAIN
void pager_request(unsigned int page) {
    struct FrameStruct *Frame=NULL;
    //printf("--TICK %i\n",sim_time());
    if((Frame=queue_find(FrameQueue,FrameHasPage,&page))!=NULL){//Already have page, can just exit
        Frame->LastUsed=sim_time();
        Frame->TimesUsed++;
        Frame->ChancesLeft=1;
        Frame->Circle=1;
        //printf("Increased chance of page %i\n",Frame->Page);
        return;
    }
    sim_get_page(page);//Call for page, results in page fault
    Frame=queue_head(FrameQueue);
    switch(Algo){
        case 0://FIFO
            Frame->Page=page;
            queue_enqueue(FrameQueue,queue_dequeue(FrameQueue));//Move frame to back of queue
            return;
        case 1://Least Recently Used
            if(Frame->LastUsed==-1){//Empty frame is available
                Frame->LastUsed=sim_time();
                Frame->Page=page;
                queue_enqueue(FrameQueue,queue_dequeue(FrameQueue));//Move frame to back of queue
                return;
            }
            else{//No empty frame available
                struct FrameStruct* LowestFrame=queue_head(FrameQueue);
                for(int i=0;i<queue_size(FrameQueue);i++){
                    Frame=queue_head(FrameQueue);
                    if(LowestFrame->LastUsed>Frame->LastUsed){
                        LowestFrame=Frame;//Set new lowest frame
                    }
                    queue_enqueue(FrameQueue,queue_dequeue(FrameQueue));
                }
                //Found lowest lastused value
                LowestFrame->Page=page;
                LowestFrame->LastUsed=sim_time();
                queue_remove(FrameQueue,LowestFrame);
                queue_enqueue(FrameQueue,LowestFrame);//Push to back so that other frames of equal value will be prioritized
                return;
            }
        case 2://Least Frequently Used
            if(Frame->Page==-1){//Empty frame is available
                Frame->Page=page;
                queue_enqueue(FrameQueue,queue_dequeue(FrameQueue));//Move frame to back of queue
                return;
            }
            else{//No empty frame available
                struct FrameStruct* LowestFrame=queue_head(FrameQueue);
                for(int i=0;i<queue_size(FrameQueue);i++){
                    Frame=queue_head(FrameQueue);
                    if(LowestFrame->TimesUsed>Frame->TimesUsed){
                        LowestFrame=Frame;//Set new lowest frame
                    }
                    queue_enqueue(FrameQueue,queue_dequeue(FrameQueue));
                }
                //Found lowest lastused value
                LowestFrame->Page=page;
                LowestFrame->TimesUsed=0;
                queue_remove(FrameQueue,LowestFrame);
                queue_enqueue(FrameQueue,LowestFrame);//Push to back so that other frames of equal value will be prioritized
                return;
            }
        case 3://Second Chance
            if(Frame->Page==-1){//Empty frame is available,MAY NEED TO COUNT AS A MISS FOR THE SAKE OF EVERY OTHER FRAME
                Frame->Page=page;
                queue_enqueue(FrameQueue,queue_dequeue(FrameQueue));//Move frame to back of queue
                //printf("Put page %i into an empty frame\n",Frame->Page);
                return;
            }
            else{//Miss and need to kick a page off
                struct FrameStruct* LowestFrame=queue_head(FrameQueue);
                for(int i=0;i<queue_size(FrameQueue);i++){
                    Frame=queue_head(FrameQueue);
                    if(Frame->ChancesLeft==0){
                        LowestFrame=Frame;//Set new lowest frame
                        break;
                    }
                    else if(Frame->ChancesLeft==1){
                        Frame->ChancesLeft=0;
                    }
                    queue_enqueue(FrameQueue,queue_dequeue(FrameQueue));
                }
                //Found lowest chances left value
                //printf("Kicked page %i off, resetting all page's chances to 0\n",LowestFrame->Page);
                LowestFrame->Page=page;
                queue_remove(FrameQueue,LowestFrame);
                queue_enqueue(FrameQueue,LowestFrame);
                return;
            }
        case 4://Circular Queue
            if(Frame->Page==-1){//Empty frame is available,MAY NEED TO COUNT AS A MISS FOR THE SAKE OF EVERY OTHER FRAME
                Frame->Page=page;
                queue_enqueue(FrameQueue,queue_dequeue(FrameQueue));//Move frame to back of queue
                //printf("Put page %i into an empty frame\n",Frame->Page);
                return;
            }
            else{//Miss and need to kick a page off
                struct FrameStruct* LowestFrame=queue_head(FrameQueue);
                for(int i=0;i<queue_size(FrameQueue);i++){
                    Frame=queue_head(FrameQueue);
                    if(Frame->Circle==0){
                        LowestFrame=Frame;//Set new lowest frame
                        break;
                    }
                    else if(Frame->Circle==1){
                        Frame->Circle=0;
                    }
                    queue_enqueue(FrameQueue,queue_dequeue(FrameQueue));
                }
                //Found lowest chances left value
                //printf("Kicked page %i off, resetting all page's chances to 0\n",LowestFrame->Page);
                LowestFrame->Page=page;
                LowestFrame->Circle=1;
                queue_remove(FrameQueue,LowestFrame);
                queue_enqueue(FrameQueue,LowestFrame);
                return;
            }
        default://ERROR
            return;
    }
}

static bool FrameHasPage(void *Frame, void *Page) {
    return ((struct FrameStruct*)Frame)->Page == *(unsigned int*)Page;
}