/************************************************************************
 * 
 * CSE130 Assignment 1
 * 
 * UNIX Shared Memory Multi-Process Merge Sort
 * 
* Copyright (C) 2020-2022 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 *
 ************************************************************************/

#include "merge.h"
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

/* 
 * Merge Sort in the current process a sub-array of ARR[] defined by the 
 * LEFT and RIGHT indexes.
 */
void singleProcessMergeSort(int arr[], int left, int right) 
{
  if (left < right) {
    int middle = (left+right)/2;
    singleProcessMergeSort(arr, left, middle); 
    singleProcessMergeSort(arr, middle+1, right); 
    merge(arr, left, middle, right); 
  } 
}

/* 
 * Merge Sort in the current process and at least one child process a 
 * sub-array of ARR[] defined by the LEFT and RIGHT indexes.
 */
//https://pubs.opengroup.org/onlinepubs/007904975/functions/ftok.html
//https://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/shmat.html
void multiProcessMergeSort(int arr[], int left, int right) 
{
  int leftend=(left+right)/2;//synonymous to middle
  int rightbegin=leftend+1;
  size_t rightsize=sizeof(int)*(right-rightbegin+1);
  int j=0;

  key_t key=ftok("myprecioussharedmemory",69);
  int id=shmget(key,rightsize,0666 | IPC_CREAT);//got shared mem id
  int *sharedarray= (int*)shmat(id,(void*)0,0);//attach to mem

  for(int i=rightbegin;i<=right;i++)//copy right hand side of array into shared mem
  {
    sharedarray[j]=arr[i];
    j++;
  }

  switch(fork()){
    case -1://ERROR
      exit(-1);
    case 0://CHILD
      singleProcessMergeSort(sharedarray,0,right-rightbegin);
      exit(0);
    default://PARENT
      singleProcessMergeSort(arr,left,leftend);
      wait(NULL);//wait for child
      j=0;
      for(int i=rightbegin;i<=right;i++)//copy shared array back into original
      {
        arr[i]=sharedarray[j];
        j++;
      }
      merge(arr,left,leftend,right);
      shmdt(sharedarray);//detach and destroy
      shmctl(id,IPC_RMID,NULL);
      return;
  }
}
