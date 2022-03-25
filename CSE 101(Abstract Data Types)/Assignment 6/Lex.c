/***********************************************************************
 * Harrison Quiring, hquiring
 * 2021 Spring CSE 101 pa6
 * Lex.c
 * Client file for the Dictionary ADT
 * ***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Dictionary.h"
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

    //Creating the dictionary, ALL PREVIOUS CODE CAME FROM MY PA1 LEX.C
    Dictionary D=newDictionary(0);
    for(int i=0;i<NumberOfLines;i++)
    {
        insert(D,InputStringsArray[i],1);
    }
    //Printing an indirectly sorted dictionary by starting at the far left of the tree and moving right
    beginForward(D);
    for(int i=0;i<NumberOfLines;i++)
    {
        fprintf(outputfile,"%s\n",currentKey(D));
        next(D);
    }

    fclose(inputfile);//closing the input file
    fclose(outputfile);
    freeDictionary(&D);
    for(int i=0;i<NumberOfLines;i++)
    {
        free(InputStringsArray[i]);
    }
    free(InputStringsArray);
    return 0;
}