/***********************************************************************
 * Harrison Quiring, hquiring
 * 2021 Spring CSE 101 pa3
 * FindComponents.c
 * Client for the Graph ADT, finds the strongly connected components of a graph
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
            addArc(G,VertexA,VertexB);
        }
    } while (VertexA!=0 && VertexB!=0);
    fprintf(outputfile,"Adjacency list representation of G:\n");
    printGraph(outputfile,G);//printing the adjacency list of the graph to the output file
    fprintf(outputfile,"\n");//goes to new line to separate adjacency list from strongly connected components

    //Finding and printing the strongly connected components
    List L=newList();
    for(int i=1;i<=getOrder(G);i++)
    {
        append(L,i);
    }
    DFS(G,L);//returns the list L in opposite order of finish time
    Graph GT=transpose(G);
    DFS(GT,L);//runs dfs on transpose with list from the return of dfs on original graph
    //Finding the number of strongly connected components
    int NumberOfStronglyConnectedComponents=0;
    moveFront(L);
    for(int i=0;i<length(L);i++)
    {
        if(getParent(GT,get(L))==NIL)
        {
            NumberOfStronglyConnectedComponents++;
        }
        moveNext(L);
    }
    //Printing 
    fprintf(outputfile,"G contains %d strongly connected components:\n",NumberOfStronglyConnectedComponents);
    //Printing the components here
    moveBack(L);//start at back and move towards front
    int NumberSkipped=0;
    for(int i=1;i<=NumberOfStronglyConnectedComponents;i++)
    {
        NumberSkipped=0;
        fprintf(outputfile, "Component %d: ",i);
        while(getParent(GT,get(L))!=NIL)
        {
            movePrev(L);
            NumberSkipped++;
        }
        fprintf(outputfile,"%d ",get(L));
        for(int j=0;j<NumberSkipped;j++)
        {
            moveNext(L);
            fprintf(outputfile,"%d ",get(L));
        }
        for(int j=0;j<NumberSkipped;j++)
        {
            movePrev(L);
        }
        fprintf(outputfile,"\n");
        movePrev(L);
    }
    fprintf(outputfile,"\n");
    freeList(&L);
    freeGraph(&G);
    freeGraph(&GT);
    return 0;
}
