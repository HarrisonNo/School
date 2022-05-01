/* 
 * This file is derived from source code for the Pintos
 * instructional operating system which is itself derived
 * from the Nachos instructional operating system. The 
 * Nachos copyright notice is reproduced in full below. 
 *
 * Copyright (C) 1992-1996 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * and its documentation for any purpose, without fee, and
 * without written agreement is hereby granted, provided that the
 * above copyright notice and the following two paragraphs appear
 * in all copies of this software.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE
 * AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
 * BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 * MODIFICATIONS.
 *
 * Modifications Copyright (C) 2017-2021 David C. Harrison. 
 * All rights reserved.
 */

#include <stdio.h>
#include <syscall-nr.h>
#include <list.h>

#include "devices/shutdown.h"
#include "devices/input.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "filesys/inode.h"
#include "filesys/directory.h"
#include "threads/palloc.h"
#include "threads/malloc.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/syscall.h"
#include "userprog/process.h"
#include "userprog/umem.h"

/****************** System Call Implementations ********************/
static struct file *FDs[32];
static int NextAvailable=0;

/*
 * BUFFER+0 should be a valid user adresses
 */
void sys_exit(int exitcode) 
{
  printf("%s: exit(%d)\n", thread_current()->name, exitcode);
  thread_exit();
}

static void exit_handler(struct intr_frame *f) 
{
  int exitcode;
  umem_read(f->esp + 4, &exitcode, sizeof(exitcode));
  sys_exit(exitcode);
}

/*
 * BUFFER+0 and BUFFER+size should be valid user adresses
 */
static uint32_t sys_write(int fd, const void *buffer, unsigned size)
{
  umem_check((const uint8_t*) buffer);
  umem_check((const uint8_t*) buffer + size - 1);

  int ret = -1;

  if (fd == 1) { // write to stdout
    putbuf(buffer, size);
    ret = size;
  }
  else if(fd==0){
    sys_exit(-1);
  }
  else{
    ret=file_write(FDs[fd%32],buffer,size);
  }

  return (uint32_t) ret;
}

static void write_handler(struct intr_frame *f)
{
    int fd;
    const void *buffer;
    unsigned size;

    umem_read(f->esp + 4, &fd, sizeof(fd));
    umem_read(f->esp + 8, &buffer, sizeof(buffer));
    umem_read(f->esp + 12, &size, sizeof(size));

    f->eax = sys_write(fd, buffer, size);
}


/****************** System Call Handler ********************/

static void
syscall_handler(struct intr_frame *f)
{
  int syscall;
  ASSERT( sizeof(syscall) == 4 ); // assuming x86

  // The system call number is in the 32-bit word at the caller's stack pointer.
  umem_read(f->esp, &syscall, sizeof(syscall));

  // Store the stack pointer esp, which is needed in the page fault handler.
  // Do NOT remove this line
  thread_current()->current_esp = f->esp;
  
  /*
  printf("\nENTERING THE SWITCH WITH SYSCALL %i\n",syscall);
  printf("\n\nSYS_HALT is %i, SYS_EXIT is %i, SYS_EXEC is %i, SYS_WAIT is %i, SYS_CREATE is %i\n",SYS_HALT,SYS_EXIT,SYS_EXEC,SYS_WAIT,SYS_CREATE);
  printf("SYS_REMOVE is %i, SYS_OPEN is %i, SYS_FILESIZE is %i, SYS_READ is %i, SYS_WRITE is %i\n",SYS_REMOVE,SYS_OPEN,SYS_FILESIZE,SYS_READ,SYS_WRITE);
  printf("SYS_SEEK is %i, SYS_TELL is %i, SYS_CLOSE is %i\n\n",SYS_SEEK,SYS_TELL,SYS_CLOSE);
  */
  switch (syscall) {
  case SYS_HALT: 
    shutdown_power_off();
    break;

  case SYS_EXIT: 
    exit_handler(f);
    break;
      
  case SYS_WRITE: 
    write_handler(f);
    break;

  case SYS_CREATE:;
    const char* CreateName;
    int Size;
    //Insert into vars
    umem_read(f->esp+4,&CreateName,sizeof(CreateName));
    umem_read(f->esp+8,&Size,sizeof(Size));
    if(CreateName==NULL || f==NULL){
      sys_exit(-1);
    }
    if(!filesys_create(CreateName, Size, false)){//If failed
      f->eax=false;
    }
    else{
      f->eax=true;
    }
    break;

  case SYS_OPEN:;
    const char *OpenName;
    struct file *OpenedFile;
    umem_read(f->esp+4,&OpenName,sizeof(OpenName));
    if(OpenName==NULL){//Name is not valid
      sys_exit(-1);
    }
    OpenedFile=filesys_open(OpenName);
    if(OpenedFile==NULL){//If failed
      f->eax=-1;
    }
    else{
      while(NextAvailable%32==0 ||NextAvailable%32==1){
        NextAvailable++;//0 and 1 are reserved
      }
      FDs[NextAvailable%32]=OpenedFile;
      f->eax=NextAvailable++;
    }
    break;

  case SYS_FILESIZE:;
    int FDSize;
    umem_read(f->esp+4,&FDSize,sizeof(FDSize));
    f->eax=file_length(FDs[FDSize]);
    break;
  
  case SYS_READ:;
    int FDRead;
    void *Buffer;
    off_t SizeRead;
    umem_read(f->esp+4,&FDRead,sizeof(FDRead));
    if(FDRead%1!=0 || FDRead==0 || FDRead==1 || FDs[FDRead]==NULL){//Not a valid fd
      sys_exit(-1);
    }
    umem_read(f->esp+8,&Buffer,sizeof(Buffer));
    umem_read(f->esp+12,&SizeRead,sizeof(SizeRead));
    if(SizeRead<0){
      sys_exit(-1);
    }
    f->eax=file_read (FDs[FDRead],Buffer,SizeRead);
    break;

  case SYS_CLOSE:;
    int FDClose;
    umem_read(f->esp+4,&FDClose,sizeof(FDClose));
    if(FDRead%1!=0 || FDs[FDClose]==NULL){//Not a valid fd
      sys_exit(-1);
    }
    file_close(FDs[FDClose]);
    FDs[FDClose]=NULL;
    f->eax=true;
    break;

  case SYS_EXEC:;
    const char *CommandLine;
    tid_t ChildTid;
    umem_read(f->esp+4,&CommandLine,sizeof(CommandLine));
    ChildTid=process_execute(CommandLine);
    if(ChildTid==TID_ERROR){//Failed to create child thread
      sys_exit(-1);
    }
    else{
      process_wait(ChildTid);
      f->eax=ChildTid;
    }
    break;
  
  case SYS_WAIT:;
    int WaitTid;
    umem_read(f->esp+4,&WaitTid,sizeof(WaitTid));
    process_wait(ChildTid);
    break;

  default:
    //printf("[ERROR] system call %d is unimplemented!\n", syscall);
    //thread_exit();
    sys_exit(-1);
    break;
  }
}

void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  for(int i=2;i<32;i++){
    FDs[i]=NULL;
  }
}




