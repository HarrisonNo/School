/*********************************************************************
 *
* Copyright (C) 2020-2022 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 *
 ***********************************************************************/

#include "cartman.h"
#include <stdio.h>
#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
//Global global info
int ActualNumberOfTracks;
#define MaxCarts 17

//Struct used by child of child thread
struct ReserveSpot{
    int SpotToReserve;
}ReserveSpot;
//Struct passed along when creating a thread
struct CartStats{
    int Cart;
    enum track Track;
    pthread_t ReserveLeft;
    struct ReserveSpot *LeftStruct;
    pthread_t ReserveRight;
    struct ReserveSpot *RightStruct;
    unsigned int TrueCartNumber;
    int FirstJunction;
    int SecondJunction;
}CartStats;

//Functions
void *ThreadedCart(void* Stats);
void ReserveBothJunctions(struct CartStats *ThisCart);
void *ThreadReserveSpot(void *Stats);

//Global Info
pthread_t CartThreads[MaxCarts];
struct CartStats *StructArray[MaxCarts];
int JunctionReservedByCart[MaxCarts];

//Locks
sem_t ReserveJunctions[MaxCarts];
pthread_mutex_t ReservingBothJunctions;

/*
 * Callback when CART on TRACK arrives at JUNCTION in preparation for
 * crossing a critical section of track.
 */
void arrive(unsigned int cart, enum track track, enum junction junction) 
{
    int LoopedCartVal=cart%ActualNumberOfTracks;
    //printf("STARTING CART %i (%i)\n",cart,LoopedCartVal);//DEBUGGING
    StructArray[LoopedCartVal]=malloc(sizeof(CartStats));
    StructArray[LoopedCartVal]->RightStruct=malloc(sizeof(ReserveSpot));
    StructArray[LoopedCartVal]->LeftStruct=malloc(sizeof(ReserveSpot));
    StructArray[LoopedCartVal]->Cart=LoopedCartVal;
    StructArray[LoopedCartVal]->TrueCartNumber=cart;
    StructArray[LoopedCartVal]->Track=track;
    StructArray[LoopedCartVal]->FirstJunction=junction;
    //printf("Cart %i starting at junction %i\n",cart,junction);//DEBUGGING
    //Create separate thread to handle everything
    if(pthread_create(&CartThreads[LoopedCartVal],NULL,&ThreadedCart,(void*)StructArray[LoopedCartVal])!=0){
        //printf("WARNING HAD TO EMERGENCY EXIT AFTER CREATING THREAD\n");
        free(StructArray[LoopedCartVal]->LeftStruct);
        free(StructArray[LoopedCartVal]->RightStruct);
        free(StructArray[LoopedCartVal]);
        return;//Failed to create thread
    }
    //pthread_join(CartThreads[cart],NULL);//DEBUGGING, forces each thread to go one at a time, results in a timeout but ensures no deadlock
    return;
}

void *ThreadedCart(void* Stats){
    struct CartStats *ThisThreadStats=Stats;
    //Calculate indices of both junctions to reserve
    if(ThisThreadStats->FirstJunction==ThisThreadStats->Track){//Going clockwise
        ThisThreadStats->SecondJunction=ThisThreadStats->FirstJunction+1;
            if(ThisThreadStats->SecondJunction>=ActualNumberOfTracks){
                ThisThreadStats->SecondJunction=0;
            }
    }
    else{//Going counter clockwise
        ThisThreadStats->SecondJunction=ThisThreadStats->FirstJunction-1;
        if(ThisThreadStats->SecondJunction<0){
            ThisThreadStats->SecondJunction=ActualNumberOfTracks-1;
        }
    }
    //printf("Thread for cart %i has begun\n",ThisThreadStats->TrueCartNumber);//DEBUGGING
    ReserveBothJunctions(ThisThreadStats);//Will continue past this even if not both junctions were successfully reserved
    //printf("Cart %i put in semaphore reservations for junctions %i and %i\n",ThisThreadStats->TrueCartNumber,ThisThreadStats->FirstJunction,ThisThreadStats->SecondJunction);//DEBUGGING
    pthread_join(ThisThreadStats->ReserveLeft,NULL);
    pthread_join(ThisThreadStats->ReserveRight,NULL);
    //printf("Cart %i finally received reservations for junctions %i and %i\n",ThisThreadStats->TrueCartNumber,ThisThreadStats->FirstJunction,ThisThreadStats->SecondJunction);//DEBUGGING
    //Only gets past here once both junctions have been successfully reserved
    //Reserve and cross both junctions
    //printf("Cart %i (%i) is attempting to reserve() junction %i\n",ThisThreadStats->TrueCartNumber,ThisThreadStats->Cart,ThisThreadStats->FirstJunction);//DEBUGGING
    reserve(ThisThreadStats->TrueCartNumber,ThisThreadStats->FirstJunction);
    //printf("Cart %i (%i) is attempting to reserve() junction %i\n",ThisThreadStats->TrueCartNumber,ThisThreadStats->Cart,ThisThreadStats->SecondJunction);//DEBUGGING
    reserve(ThisThreadStats->TrueCartNumber,ThisThreadStats->SecondJunction);
    //printf("Cart %i (%i) successfully reserved() both junctions\n",ThisThreadStats->TrueCartNumber,ThisThreadStats->Cart);//DEBUGGING
    //printf("CART %i (%i) ATTEMPTING TO CROSS JUNCTION %i\n",ThisThreadStats->TrueCartNumber,ThisThreadStats->Cart,ThisThreadStats->FirstJunction);//DEBUGGING
    cross(ThisThreadStats->TrueCartNumber,ThisThreadStats->Track,ThisThreadStats->FirstJunction);
    //Unlock both junctions and exit
    //printf("Cart %i is attempting to release() junction %i\n",ThisThreadStats->TrueCartNumber,ThisThreadStats->FirstJunction);//DEBUGGING
    release(ThisThreadStats->TrueCartNumber,ThisThreadStats->FirstJunction);
    //printf("Cart %i is attempting to release() junction %i\n",ThisThreadStats->TrueCartNumber,ThisThreadStats->SecondJunction);//DEBUGGING
    release(ThisThreadStats->TrueCartNumber,ThisThreadStats->SecondJunction);
    //printf("Cart %i successfully released() both junctions\n",ThisThreadStats->TrueCartNumber);//DEBUGGING
    sem_post(&ReserveJunctions[ThisThreadStats->FirstJunction]);
    //printf("Cart %i discarded semaphore control of junction %i\n",ThisThreadStats->TrueCartNumber,ThisThreadStats->FirstJunction);//DEBUGGING
    sem_post(&ReserveJunctions[ThisThreadStats->SecondJunction]);
    //printf("Cart %i discarded semaphore control of junction %i\n",ThisThreadStats->TrueCartNumber,ThisThreadStats->SecondJunction);//DEBUGGING
    free(ThisThreadStats->RightStruct);
    free(ThisThreadStats->LeftStruct);
    free(ThisThreadStats);
    //printf("FINISHED A CART (value unkown)\n");//DEBUGGING
    pthread_exit(NULL);
}

void ReserveBothJunctions(struct CartStats *ThisCart){
    //Lock so that nothing else can reserve one junction before this thread
    ThisCart->LeftStruct->SpotToReserve=ThisCart->FirstJunction;
    ThisCart->RightStruct->SpotToReserve=ThisCart->SecondJunction;
    pthread_mutex_lock(&ReservingBothJunctions);
    //Reserve first spot
    pthread_create(&ThisCart->ReserveLeft,NULL,&ThreadReserveSpot,(void*)ThisCart->LeftStruct);
    //Reserve right spot
    pthread_create(&ThisCart->ReserveRight,NULL,&ThreadReserveSpot,(void*)ThisCart->RightStruct);
    pthread_mutex_unlock(&ReservingBothJunctions);
    return;
}

void *ThreadReserveSpot(void *Stats){
    struct ReserveSpot *Spot=Stats;
    //Thread waits here until spot is lifted
    //printf("A cart is attempting to reserve the semaphore for junction %i\n",Spot->SpotToReserve);//DEBUGGING
    sem_wait(&ReserveJunctions[Spot->SpotToReserve]);
    //printf("Junction %i's semaphore was just repicked up\n",Spot->SpotToReserve);//DEBUGGING
    pthread_exit(NULL);//Only exits once spot has been successfully reserved by parent thread
}

/*
 * Initialise the CART Manager for TRACKS tracks.
 *
 * Some implementations will do nothing, most will initialise necessary
 * concurrency primitives.
 */
void cartman(unsigned int tracks) 
{
    ActualNumberOfTracks=tracks;
    for(int i=0;i<MaxCarts;i++){//Initializes all necessary locks
        sem_init(&ReserveJunctions[i],0,1);
        JunctionReservedByCart[i]=-1;//Default
    }
    pthread_mutex_init(&ReservingBothJunctions,NULL);
}