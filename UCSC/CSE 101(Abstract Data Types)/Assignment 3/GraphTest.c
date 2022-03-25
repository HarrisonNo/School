/***********************************************************************
 * Harrison Quiring, hquiring
 * 2021 Spring CSE 101 pa3
 * GraphTest.c
 * Test file for functions in Graph ADT
 * ***********************************************************************/
#include "Graph.c"
#include "List.c"
#include <stdio.h>

int main()
{
    printf("RUNNING TESTS\n");
    //Test for addArc
    printf("Test for addArc:");
    Graph G=newGraph(100);
    addArc(G,1,2);
    addArc(G,2,3);
    addArc(G,2,4);
    addArc(G,2,5);
    addArc(G,3,6);
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
    //Test for DFS
    printf("Test for DFS:");
    List L=newList();
    G=newGraph(100);
    for(int i=1;i<=getOrder(G);i++)
    {
        append(L,i);
    }
    addArc(G,1,2);
    addArc(G,2,3);
    addArc(G,2,4);
    addArc(G,2,5);
    addArc(G,3,6);
    DFS(G,L);
    printf("SUCCESS\n");
    freeList(&L);
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
    addArc(G,1,2);
    addArc(G,2,3);
    addArc(G,15,32);
    addArc(G,76,12);
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
    //Test for getParent
    printf("Test for getParent:");
    List LA=newList();
    G=newGraph(100);
    for(int i=1;i<=getOrder(G);i++)
    {
        append(LA,i);
    }
    addArc(G,1,2);
    addArc(G,2,3);
    addArc(G,2,4);
    addArc(G,2,5);
    addArc(G,3,6);
    DFS(G,LA);
    int ParentG=getParent(G,4);
    if(ParentG==2)
    {
        printf("SUCCESS\n");
    }
    else
    {
        printf("FAILED\n");
    }
    freeList(&LA);
    freeGraph(&G);
    //Test for getDiscover
    printf("Test for getDiscover:");
    List LB=newList();
    G=newGraph(100);
    for(int i=1;i<=getOrder(G);i++)
    {
        append(LB,i);
    }
    addArc(G,1,2);
    addArc(G,2,3);
    addArc(G,2,4);
    addArc(G,2,5);
    addArc(G,3,6);
    DFS(G,LB);
    int DiscoverG=getDiscover(G,4);
    if(DiscoverG==7)
    {
        printf("SUCCESS\n");
    }
    else
    {
        printf("FAILED\n");
    }
    freeList(&LB);
    freeGraph(&G);
    //Test for getFinish
    printf("Test for getFinish:");
    List LC=newList();
    G=newGraph(100);
    for(int i=1;i<=getOrder(G);i++)
    {
        append(LC,i);
    }
    addArc(G,1,2);
    addArc(G,2,3);
    addArc(G,2,4);
    addArc(G,2,5);
    addArc(G,3,6);
    DFS(G,LC);
    int FinishG=getFinish(G,3);
    if(FinishG==6)
    {
        printf("SUCCESS\n");
    }
    else
    {
        printf("FAILED\n");
    }
    freeList(&LC);
    freeGraph(&G);
    //Test for makeNull
    printf("Test for makeNull:");
    G=newGraph(100);
    addArc(G,1,2);
    addArc(G,2,3);
    addArc(G,2,4);
    addArc(G,2,5);
    addArc(G,3,6);
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