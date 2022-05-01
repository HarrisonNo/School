/*********************************************************************
 *
 * Copyright (C) 2020-2022 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 *
 ***********************************************************************/

#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>//Needed for scandir()
//#include <string.h>
//#include <stdio.h>//DEBUGGING

//void RecursiveFilePrint(char *Path,int Depth, char* FileName);

/*
 * Extended ASCII box drawing characters:
 * 
 * The following code:
 * 
 * printf("CSE130\n");
 * printf("%s%s Assignments\n", TEE, HOR);
 * printf("%s  %s%s Assignment 1\n", VER, TEE, HOR);
 * printf("%s  %s%s Assignment 2\n", VER, TEE, HOR);
 * printf("%s  %s%s Assignment 3\n", VER, TEE, HOR);
 * printf("%s  %s%s Assignment 4\n", VER, TEE, HOR);
 * printf("%s  %s%s Assignment 5\n", VER, TEE, HOR);
 * printf("%s  %s%s Assignment 6\n", VER, ELB, HOR);
 * printf("%s%s Labs\n", ELB, HOR);
 * printf("   %s%s Lab 1\n", TEE, HOR);
 * printf("   %s%s Lab 2\n", TEE, HOR);
 * printf("   %s%s Lab 3\n", ELB, HOR);
 * printf();
 * 
 * Shows this tree:
 * 
 * CSE130
 * ├─ Assignments
 * │  ├─ Assignment 1
 * │  ├─ Assignment 2
 * │  ├─ Assignment 3
 * │  ├─ Assignment 4
 * │  ├─ Assignment 5
 * |  └─ Assignment 6
 * └─ Labs
 *    ├─ Lab 1
 *    ├─ Lab 2
 *    └─ Lab 3
 */
#define TEE "\u251C"  // ├ 
#define HOR "\u2500"  // ─ 
#define VER "\u2502"  // │
#define ELB "\u2514"  // └

/*
 * Read at most SIZE bytes from FNAME starting at FOFFSET into BUF starting 
 * at BOFFSET.
 *
 * RETURN number of bytes read from FNAME into BUF, -1 on error.
 */
size_t fileman_read(
  const char *const fname, 
  const size_t foffset, 
  char *const buf, 
  const size_t boffset, 
  const size_t size) 
{
  size_t BytesRead=-1;
  if(size<=0){
    return 0;
  }
  //Open the file
  int FileDesc=open(fname,O_RDONLY);
  if(FileDesc==-1){//Error, likely can't find file
    return BytesRead;
  }
  //Repostion file position
  lseek(FileDesc,foffset,SEEK_SET);
  //Read the file
  BytesRead=read(FileDesc,buf+boffset,size);
  //Close file
  close(FileDesc);
  return BytesRead;
}

/*
 * Create FNAME and write SIZE bytes from BUF starting at BOFFSET into FNAME
 * starting at FOFFSET.
 * 
 * RETURN number of bytes from BUF written to FNAME, -1 on error or if FNAME
 * already exists
 */
size_t fileman_write(
  const char *const fname, 
  const size_t foffset, 
  const char *const buf, 
  const size_t boffset, 
  const size_t size) 
{
  size_t BytesWritten=-1;
  if(size<=0){
    return 0;
  }
  //Open the file, exiting with -1 if already exists
  int FileDesc=open(fname,O_RDWR);
  if(FileDesc!=-1){//Found a preexisting file
    close(FileDesc);
    return BytesWritten;
  }
  //Actually create the file here
  FileDesc=open(fname,O_RDWR | O_CREAT | 0666);
  if(FileDesc==-1){//ERROR
    return BytesWritten;
  }
  //Repostion file position
  lseek(FileDesc,foffset,SEEK_SET);
  //Write the file
  BytesWritten=write(FileDesc,buf+boffset,size);
  //Close file
  close(FileDesc);
  return BytesWritten;
}

/*
 * Append SIZE bytes from BUF to existing FNAME.
 * 
 * RETURN number of bytes from BUF appended to FNAME, -1 on error or if FNAME
 * does not exist
 */
size_t fileman_append(const char *const fname, const char *const buf, const size_t size) 
{
  size_t BytesWritten=-1;
  //Open the file, exiting with -1 if already exists
  int FileDesc=open(fname,O_RDWR | O_APPEND);
  if(FileDesc==-1){//No file found
    return BytesWritten;
  }
  //Write the file
  BytesWritten=write(FileDesc,buf,size);
  //Close file
  close(FileDesc);
  return BytesWritten;
}

/*
 * Copy existing file FSRC to new file FDEST.
 *
 * Do not assume anything about the size of FSRC. 
 * 
 * RETURN number of bytes from FSRC written to FDEST, -1 on error, or if FSRC 
 * does not exists, or if SDEST already exists.
 */
size_t fileman_copy(const char *const fsrc, const char *const fdest) 
{
  size_t BufferSize=4096, Offset=0, BytesRead=0, TotalBytesWritten=0;
  char Buffer[4096];
  if(Buffer==NULL){//Failed to allocate mem
    return -1;
  }
  BytesRead=fileman_read(fsrc,Offset,&Buffer[0],0,BufferSize);
  if(BytesRead<=0){//No bytes to read or source file doesn't exist
    return -1;
  }
  if(fileman_write(fdest,Offset,&Buffer[0], 0, BytesRead)==-1){//Destination file already existed
    return -1;
  }
  while(1){
    Offset+=BytesRead;
    TotalBytesWritten+=BytesRead;
    if(BytesRead<BufferSize){//No bytes left already
      return TotalBytesWritten;
    }
    BytesRead=fileman_read(fsrc,Offset,&Buffer[0],0,BufferSize);
    fileman_append(fdest,&Buffer[0],BytesRead);//Append instead of write
  }
  return TotalBytesWritten;//SHOULD NEVER GET HERE
}

/*
 * Print a hierachival directory view starting at DNAME to file descriptor FD 
 * as shown in an example below where DNAME == 'data.dir'
 *
 *   data.dir
 *       blbcbuvjjko
 *           lgvoz
 *               jfwbv
 *                   jqlbbb
 *                   yfgwpvax
 *           tcx
 *               jbjfwbv
 *                   demvlgq
 *                   us
 *               zss
 *                   jfwbv
 *                       ahfamnz
 *       vkhqmgwsgd
 *           agmugje
 *               surxeb
 *                   dyjxfseur
 *                   wy
 *           tcx
 */
//https://stackoverflow.com/questions/18402428/how-to-properly-use-scandir-in-c
void fileman_dir(const int fd, const char *const dname) 
{
  /*
  struct dirent **ChildFileNames;
  int NumOfDirs=scandir("./data.dir",&ChildFileNames,NULL,alphasort);

  printf("%s\n",dname);
  for(int i=2;i<NumOfDirs;i++){
    printf("\t%s\n",ChildFileNames[i]->d_name);
  }
  for(int i=0;i<NumOfDirs;i++){
    free(ChildFileNames[i]);
  }
  free(ChildFileNames);
  */
 /*
  char *Starting="./";
  char *NewFilePath=malloc((strlen(Starting)+strlen(dname))*sizeof(char)+1);
  memcpy(NewFilePath,Starting,strlen(Starting));
  memcpy(NewFilePath+strlen(Starting),dname,strlen(dname)+1);//Calc new file path for child dirs
  //printf("CALCED THE NEW FILE PATH TO BE %s\n",NewFilePath);
  RecursiveFilePrint(NULL,0,(char*)NewFilePath);
  */
 return;//Old solution that never worked
}

/*
void RecursiveFilePrint(char *Path,int Depth, char* FileName){
  struct dirent **ChildFileNames;
  char *Slash="/";
  for(int i=0;i<Depth;i++){
    printf("\t");//Print tabs repeatedly
  }
  printf("%s\n",FileName);

  int PathLength=0;
  if(Path!=NULL){
    PathLength=strlen(Path);
  }
  char *NewFilePath=malloc((PathLength+strlen(FileName)+strlen(Slash))*sizeof(char)+1);
  memcpy(NewFilePath,Path,PathLength);
  memcpy(NewFilePath+PathLength,Slash,strlen(Slash));
  memcpy(NewFilePath+PathLength+strlen(Slash),FileName,strlen(FileName)+1);//Calc new file path for child dirs

  int NumOfDirs=scandir(".",&ChildFileNames,NULL,alphasort);
  for(int i=2;i<NumOfDirs;i++){//START AT 2
    RecursiveFilePrint(NewFilePath,Depth+1,ChildFileNames[i]->d_name);
  }
  //Free mem
  for(int i=0;i<NumOfDirs;i++){
    free(ChildFileNames[i]);
  }
  free(ChildFileNames);
  free(NewFilePath);
  return;
}
*/
/*
 * Print a hierachival directory tree view starting at DNAME to file descriptor 
 * FD as shown in an example below where DNAME == 'world'.
 * 
 * Use the extended ASCII box drawing characters TEE, HOR, VER, and ELB.
 *
 *   world
 *   ├── europe
 *   │   ├── france
 *   │   │   └── paris
 *   │   │       ├─- entente
 *   │   │       └── saint-germain
 *   │   └── uk
 *   │       ├── london
 *   │       │   ├── arsenal
 *   │       │   └── chelsea
 *   │       └── manchester
 *   │           └── city
 *   │           └── united
 *   └── usa
 *       ├── ma
 *       │   └── boston
 *       │       ├── bruins
 *       │       └── sox
 *       └── tx
 */
void fileman_tree(const int fd, const char *const dname) 
{
}
