/***********************************************************************
 * Harrison Quiring, hquiring
 * 2021 Spring CSE 101 pa1
 * Lex.c
 * Program to indirectly sort a file
 * ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

int main(int NumOfArgs, char* ArgNames[])
{
    FILE *inputfile,*outputfile;
    if(NumOfArgs!=3)//Checking for right number of arguments
    {
        fprintf(stderr,"Usage: %s <input file> <output file>\n", ArgNames[0]);
        exit(EXIT_FAILURE);
    }

    inputfile=fopen(ArgNames[1],"r");//Opening input file
    if(inputfile==NULL)
    {
        fprintf(stderr,"Unable to open file %s for reading\n", ArgNames[1]);
        exit(EXIT_FAILURE);
    }
    outputfile=fopen(ArgNames[2],"w");//opening output file
    if(outputfile==NULL)
    {
        printf("Unable to open file %s for writing\n", ArgNames[2]);
        exit(1);
    }

    //Reading the number of lines in the program
    int NumberOfLines=0;
    char inputline[300];
    while(fgets(inputline,300,inputfile)!=NULL)//300 stands for the max distance it will read in a single line
    {
        NumberOfLines+=1;
    }

    //Closing the file to reopen it again to input the strings into the array
    fclose(inputfile);
    inputfile=fopen(ArgNames[1],"r");//Opening input file

    char** InputStringsArray=malloc(NumberOfLines *sizeof(char*));//SOURCE https://stackoverflow.com/questions/5935933/dynamically-create-an-array-of-strings-with-malloc
    //Creating a string array of length(number of lines)
    //char* InputStringsArray=(char*) malloc(NumberOfLines *sizeof(char)); //SOURCE:https://www.quora.com/How-do-I-increase-the-size-of-an-existing-array-statically-and-dynamically-in-C
    for(int i=0;i<NumberOfLines;i++)
    {
        InputStringsArray[i]=malloc(300*sizeof(char));
        fgets(InputStringsArray[i],300,inputfile);
        strtok(InputStringsArray[i],"\n");//removes the newline character from the string because for some reason if a string is moved using insertbefore it removes the newline character, resulting in a mix of newlines and not newlines
        //SOURCE FOR REMOVAL OF \n https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
    }
    
    List OutputList=newList();
    for(int IndexInsert=0;IndexInsert<NumberOfLines;IndexInsert++)//Start of indirectly sorting the array,indexinsert keeps track of which word it is on for the inputfile
    {
        //printf("It looped %d \n",IndexInsert);DEBBUING
       moveFront(OutputList);
       int InsertedBefore=0;//insertedbefore=false
       for(int OutputLine=0;OutputLine<length(OutputList);OutputLine++)//looping through the output list
       {
            //printf("internal loop %d \n",OutputLine);DEBUGGING
           if(strcmp(InputStringsArray[IndexInsert],InputStringsArray[get(OutputList)])<=0)
           {
               if(length(OutputList)>0)
               {
                   insertBefore(OutputList,IndexInsert);
                   //printf("INSERTED BEFORE\n");
               }
               else
               {
                   prepend(OutputList,IndexInsert);
                   //printf("PREPENDED\n");
               }
               InsertedBefore=1;
               break;
           }
           moveNext(OutputList);
       }
       if(InsertedBefore==0)
       {
           append(OutputList,IndexInsert);
           //printf("APPENDED\n");
       }

    }

    moveFront(OutputList);
    for(int i=0;i<length(OutputList);i++)//PRINTING TO THE OUTPUT FILE
    {
        //printf("%s\n",InputStringsArray[get(OutputList)]);//DEBUG
        fprintf(outputfile,"%s\n",InputStringsArray[get(OutputList)]);
        moveNext(OutputList);
    }

    fclose(inputfile);//closing the input file
    fclose(outputfile);
    freeList(&OutputList);//End of program
    for(int i=0;i<NumberOfLines;i++)
    {
        free(InputStringsArray[i]);
    }
    //freeing the dynamically created array of arrays
    for(int j=0;j<=NumberOfLines;j++)
    {
        free(InputStringsArray[j]);
    }
    free(InputStringsArray);
    return 0;
}