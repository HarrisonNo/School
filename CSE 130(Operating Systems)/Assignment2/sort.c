/************************************************************************
 *
 * CSE130 Assignment 2
 *
 * Copyright (C) 2020-2022 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 *
 ************************************************************************/

#include "merge.h"
#include <stdlib.h>
#include <pthread.h>

//MiddleLeft calculates the middle index, favoring the left hand side if decimal
#define MiddleLeft(Left, Right) ((Left+Right)/2)
//MiddleRight calculates the middle index, favoring the right hand side if decimal
#define MiddleRight(Left, Right) ((Left+Right)/2+1)

void NonStructSplitAndSort(int arr[], int Left, int Right);
void * SplitAndSort(void *Args);
void * Sort(void *Args);
struct Arguments{
  int left;
  int right;
  int *arr;
}Arguments;

/* LEFT index and RIGHT index of the sub-array of ARR[] to be sorted */
void singleThreadedMergeSort(int arr[], int left, int right) 
{
  if (left < right) {
    int middle = (left+right)/2;
    singleThreadedMergeSort(arr, left, middle); 
    singleThreadedMergeSort(arr, middle+1, right); 
    merge(arr, left, middle, right); 
  } 
}

/* 
 * This function stub needs to be completed
 */
//Process creates a total of three threads
//Master process creates one thread
//Then master process and child thread both create one thread each
//Not the cleanest code, but it is the most memory efficient as it uses the bare minimum number of threads to get the required speedup
void multiThreadedMergeSort(int arr[], int left, int right) 
{
  pthread_t ThreadA;
  //ChildA takes care of right hand side of og array
  struct Arguments *ChildA=malloc(1*sizeof(Arguments));
  ChildA->arr=arr;
  ChildA->left=MiddleRight(left,right);
  ChildA->right=right;
  pthread_create(&ThreadA,NULL,&SplitAndSort,(void*)ChildA);
  //Uses main process to create an additional thread to work together to sort left hand of og array
  NonStructSplitAndSort(arr,left,MiddleLeft(left,right));
  //Merge with A
  pthread_join(ThreadA,NULL);
  merge(arr,left,MiddleLeft(left,right),right);
  free(ChildA);
  return;
}
//Uses a single thread to sort array from struct's indices
void * Sort(void *Args){
  struct Arguments *Parameters=Args;//Catch the passed struct
  singleThreadedMergeSort(Parameters->arr,Parameters->left,Parameters->right);
  pthread_exit(NULL);
}
//Functionally identical to NonStructSplitAndSort except it can be called with a thread that isn't the parent
void * SplitAndSort(void *Args){
  struct Arguments *Parameters=Args;//Catch the passed struct
  NonStructSplitAndSort(Parameters->arr,Parameters->left,Parameters->right);
  pthread_exit(NULL);
}
//Uses parent thread and creates a child thread to sort array between given indices
void NonStructSplitAndSort(int arr[], int Left, int Right){
  pthread_t NewThreadID;
  struct Arguments *ChildParams=malloc(1*sizeof(Arguments));
  //Child takes care of right hand side of parents array
  ChildParams->left=MiddleRight(Left,Right);
  ChildParams->right=Right;
  ChildParams->arr=arr;
  pthread_create(&NewThreadID,NULL,Sort,(void*)ChildParams);
  //Parent takes care of left hand side of parents array
  singleThreadedMergeSort(arr,Left,MiddleLeft(Left,Right));
  pthread_join(NewThreadID,NULL);
  merge(arr,Left,MiddleLeft(Left,Right),Right);
  free(ChildParams);
  return;
}