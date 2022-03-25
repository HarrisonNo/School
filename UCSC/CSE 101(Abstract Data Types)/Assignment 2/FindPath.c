/***********************************************************************
 * Harrison Quiring, hquiring
 * 2021 Spring CSE 101 pa2
 * FindPath.c
 * Client program for Graph ADT
 * ***********************************************************************/
#include "Graph.h"
#include <stdlib.h>
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

    int InputOrder=0;
    fscanf(inputfile,"%d",&InputOrder);//getting the order of the graph
    Graph G=newGraph(InputOrder);
    int VertexA,VertexB;
    do//Loop that scans through the first part of the input file after the order
    {
        fscanf(inputfile,"%d %d",&VertexA,&VertexB);//gets the edges in the graph, SOURCE: https://stackoverflow.com/questions/4600797/read-int-values-from-a-text-file-in-c
        if(VertexA!=0 && VertexB!=0)
        {
            addEdge(G,VertexA,VertexB);
        }
    } while (VertexA!=0 && VertexB!=0);
    printGraph(outputfile,G);//printing the adjacency list of the graph to the output file
    fprintf(outputfile,"\n");//goes to new line to separate adjacency list from shortest path


    List L=newList();
    do//Loop that scans through the second part of the input file 
    {
        fscanf(inputfile,"%d %d",&VertexA,&VertexB);//gets the pair of vertices to find the path of in the graph
        if(VertexA!=0 && VertexB!=0)
        {
            BFS(G, VertexA);//calling BFS with source at VertexA
            fprintf(outputfile,"The distance from %d to %d is ",VertexA,VertexB);
            clear(L);
            getPath(L,G,VertexB);
            if(front(L)==NIL)//changes what it says if the list returns INF(-1)
            {
                fprintf(outputfile,"infinity\n");
                fprintf(outputfile,"No %d-%d path exists\n",VertexA,VertexB);
            }
            else
            {
                fprintf(outputfile,"%d\n",getDist(G,VertexB));
                fprintf(outputfile,"A shortest %d-%d path is: ",VertexA,VertexB);\
                printList(outputfile,L);
            }  
            fprintf(outputfile,"\n");//mandatory to go to new line
            fprintf(outputfile,"\n");//skips a line
        } 
    } while (VertexA!=0 && VertexB!=0);

    freeList(&L);
    freeGraph(&G);
    fclose(inputfile);//closing the input file
    fclose(outputfile);//closing the output
    return 0;
}