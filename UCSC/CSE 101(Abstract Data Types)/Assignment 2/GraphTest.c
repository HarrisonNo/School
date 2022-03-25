/***********************************************************************
 * Harrison Quiring, hquiring
 * 2021 Spring CSE 101 pa2
 * GraphTest.c
 * Test file for functions in Graph ADT
 * ***********************************************************************/
#include "Graph.h"
#include <stdio.h>

int main()
{
    printf("RUNNING TESTS\n");
    //Test for addEdge
    printf("Test for addEdge:");
    Graph G=newGraph(100);
    addEdge(G,1,2);
    addEdge(G,2,3);
    addEdge(G,2,4);
    addEdge(G,2,5);
    addEdge(G,3,6);
    printf("SUCCESS\n");
    freeGraph(&G);
    //Test for addArc
    printf("Test for addArc:");
    G=newGraph(100);
    addArc(G,1,2);
    addArc(G,2,3);
    addArc(G,2,4);
    addArc(G,2,5);
    addArc(G,3,6);
    printf("SUCCESS\n");
    freeGraph(&G);
    //Test for BFS
    printf("Test for BFS:");
    G=newGraph(100);
    addEdge(G,1,2);
    addEdge(G,2,3);
    addEdge(G,2,4);
    addArc(G,2,5);
    addArc(G,3,6);
    BFS(G,3);
    printf("SUCCESS\n");
    freeGraph(&G);
    //Test for getOrder
    printf("Test for getOrder:");
    G=newGraph(100);
    int OrderG=getOrder(G);
    if(OrderG==100)
    {
        printf("SUCCESS\n");
    }
    else
    {
        printf("FAILED\n");
    }
    freeGraph(&G);
    //Test for getSize
    printf("Test for getSize:");
    G=newGraph(100);
    addEdge(G,1,2);
    addEdge(G,2,3);
    addEdge(G,15,32);
    addEdge(G,76,12);
    addArc(G,41,23);
    addArc(G,21,22);
    int SizeG=getSize(G);
    if(SizeG==6)
    {
        printf("SUCCESS\n");
    }
    else
    {
        printf("FAILED\n");
    }
    freeGraph(&G);
    //Test for getSource
    printf("Test for getSource:");
    G=newGraph(100);
    addEdge(G,1,2);
    addEdge(G,2,3);
    addEdge(G,2,4);
    addEdge(G,2,5);
    addEdge(G,3,6);
    BFS(G,3);
    int SourceG=getSource(G);
    if(SourceG==3)
    {
        printf("SUCCESS\n");
    }
    else
    {
        printf("FAILED\n");
    }
    freeGraph(&G);
    //Test for getParent
    printf("Test for getParent:");
    G=newGraph(100);
    addEdge(G,1,2);
    addEdge(G,2,3);
    addEdge(G,2,4);
    addEdge(G,2,5);
    addEdge(G,3,6);
    BFS(G,3);
    int ParentG=getParent(G,4);
    if(ParentG==2)
    {
        printf("SUCCESS\n");
    }
    else
    {
        printf("FAILED\n");
    }
    freeGraph(&G);
    //Test for getDist
    printf("Test for getDist:");
    G=newGraph(100);
    addEdge(G,1,2);
    addEdge(G,2,3);
    addEdge(G,2,4);
    addEdge(G,2,5);
    addEdge(G,3,6);
    BFS(G,3);
    int DistanceG=getDist(G,1);
    if(DistanceG==2)
    {
        printf("SUCCESS\n");
    }
    else
    {
        printf("FAILED\n");
    }
    freeGraph(&G);
    //Test for getPath
    printf("Test for getPath:");
    G=newGraph(100);
    addEdge(G,1,2);
    addEdge(G,2,3);
    addEdge(G,2,4);
    addEdge(G,2,5);
    addEdge(G,3,6);
    BFS(G,3);
    List L=newList();
    getPath(L,G,1);
    List OG=newList();
    append(OG,3);
    append(OG,2);
    append(OG,1);
    if(equals(L,OG))
    {
        printf("SUCCESS\n");
    }
    else
    {
        printf("FAILED\n");
    }
    freeList(&L);
    freeList(&OG);
    freeGraph(&G);
    //Test for makeNull
    printf("Test for makeNull:");
    G=newGraph(100);
    addEdge(G,1,2);
    addEdge(G,2,3);
    addEdge(G,2,4);
    addEdge(G,2,5);
    addEdge(G,3,6);
    makeNull(G);
    SizeG=getSize(G);
    if(SizeG==0)
    {
        printf("SUCCESS\n");
    }
    else
    {
        printf("FAILED\n");
    }
    freeGraph(&G);
    printf("END OF TESTS\n");
    return 0;
}