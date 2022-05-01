/************************************************************************
 * 
 * CSE130 Assignment 1
 *  
 * POSIX Shared Memory Multi-Process Merge Sort
 * 
 * Copyright (C) 2020-2022 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 *
 ************************************************************************/

#include "merge.h"
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>

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
//https://pubs.opengroup.org/onlinepubs/007904875/functions/shm_open.html
void multiProcessMergeSort(int arr[], int left, int right) 
{
  int leftend=(left+right)/2;
  int rightbegin=leftend+1;
  int j=0;
  size_t rightsize=sizeof(int)*(right-rightbegin+1);
  
  int fd=shm_open("myprecioussharedmemory", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);//create shared mem
  ftruncate(fd,rightsize);//allocate size in mem
  int *sharedarray=mmap(NULL, rightsize,PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);//pointer to shared mem
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
      wait(NULL);//wait for child to exit
      j=0;
      for(int i=rightbegin;i<=right;i++)//copy shared array back into original
      {
        arr[i]=sharedarray[j];
        j++;
      }
      merge(arr,left,leftend,right);
      munmap(sharedarray,rightsize);//unlink and free all shared mem
      shm_unlink("myprecioussharedmemory");
      close(fd);
      return;
  }
}
