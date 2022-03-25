/***********************************************************************
 * Harrison Quiring, hquiring
 * 2021 Spring CSE 101 pa2
 * Graph.c
 * Functions for the Graph ADT
 * ***********************************************************************/
#include <stdlib.h>
#include "Graph.h"

typedef struct graphobject
{
    List *neighbors;//ith element contains the neighbors of vertex i
    List queue;
    char *color;//ith element is the color of vertex i, White=W=undiscovered, Gray=G=in the queue, Black=B=discovered
    int *parent;//ith element is the parent of vertex i
    int *distance;//ith element is the distance from the (most recent) source to vertex i
    int order;//number of vertices
    int size;//number of edges
    int recentsource;//label of the vertex that was most recently used as a source for BFS
}graphobject;

/*** Constructors-Destructors ***/

Graph newGraph(int n)
{
    Graph G;
    G=calloc(1,sizeof(graphobject));
    G->order=n;
    G->size=0;
    G->recentsource=NIL;
    G->neighbors=(List*)calloc((n+1),sizeof(List));//adding 1 to n so that the inices match up with the vertex label
    for(int i=0;i<=n;i++)
    {
        G->neighbors[i]=newList();//creating a new list in each index of the array
    }
    G->queue=newList();
    G->color=(char*)calloc((n+1),sizeof(char));
    G->parent=(int*)calloc((n+1),sizeof(int));
    G->distance=(int*)calloc((n+1),sizeof(int));
    return G;
}

/*** Access functions ***/

int getOrder(Graph G)
{
    if(G==NULL)
    {
        printf("Graph Error: calling getOrder() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return (G->order);
}
int getSize(Graph G)
{
   if(G==NULL)
    {
        printf("Graph Error: calling getSize() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return (G->size); 
}
void freeGraph(Graph* pG)
{
    if(pG!=NULL && *pG!=NULL)
    {
        for(int i=0;i<=getOrder(*pG);i++)
        {
            freeList(&(*pG)->neighbors[i]);//frees all of the lists in the neighbors array
        }
        free((*pG)->neighbors);
        freeList(&(*pG)->queue);
        free((*pG)->color);
        free((*pG)->parent);
        free((*pG)->distance);
        
        free(*pG);
        *pG=NULL;
    }
}

int getSource(Graph G)
{
    if(G==NULL)
    {
        printf("Graph Error: calling getSource() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return (G->recentsource); 
}
int getParent(Graph G, int u)
{
    if(G==NULL)
    {
        printf("Graph Error: calling getParent() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if(u<1 || u>getOrder(G))
    {
        printf("Graph Error: calling getParent() on a nonexistant vertex\n");
        exit(EXIT_FAILURE);
    }
    if(getSource(G)==NIL)//CHECKING TO SEE IF BFS HAS BEEN CALLED
    {
        return NIL;
    }
    return (G->parent[u]);
}
int getDist(Graph G, int u)
{
    if(G==NULL)
    {
        printf("Graph Error: calling getDist() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if(u<1 || u>getOrder(G))
    {
        printf("Graph Error: calling getDist() on a nonexistant vertex\n");
        exit(EXIT_FAILURE);
    }
    if(getSource(G)==NIL)//CHECKING TO SEE IF BFS HAS BEEN CALLED
    {
        return INF;
    }
    int totaldistance=0;
    int i=u;
    int triggered=0;
    while(getParent(G,i)!=NIL)
    {
        i=getParent(G,i);
        totaldistance++;
        triggered=1;
    }
    if(triggered==1 || getSource(G)==i)
    {
        G->distance[u]=totaldistance;
    }
    else
    {
        G->distance[u]=INF;
    }
    return G->distance[u];
}
void getPath(List L, Graph G, int u)
{
    if(u<1 || u>getOrder(G))
    {
        printf("Graph Error: calling getPath() on a nonexistant vertex\n");
        exit(EXIT_FAILURE);
    }
    if(getSource(G)==NIL)//CHECKING TO SEE IF BFS HAS BEEN CALLED
    {
        printf("Graph Error: calling getPath() without calling BFS first\n");
        exit(EXIT_FAILURE);
    }

    List LM=newList();//have to create a new list so as to not mess with the original
    int i=u;
    while(i!=0)
    {
        prepend(LM,i);
        i=getParent(G,i);
    }
    moveFront(LM);

    if(get(LM)!=G->recentsource)//checking to see if it is not connected to the recentsource
    {
        clear(LM);
        append(LM,NIL);
    }
    moveFront(LM);
    for(int i=0;i<length(LM);i++)//combine the two lists
    {
        append(L,get(LM));
        moveNext(LM);
    }
    freeList(&LM);
}

/*** Manipulation procedures ***/

void makeNull(Graph G)
{
    for(int i=1;i<=getSize(G);i++)
    {
        clear(G->neighbors[i]);
    }
    G->size=0;
}
void addEdge(Graph G, int u, int v)
{
    if(u<1 || u>getOrder(G))
    {
        printf("Graph Error: calling addEdge() on a nonexistant vertex\n");
        exit(EXIT_FAILURE);
    }
    if(v<1 || v>getOrder(G))
    {
        printf("Graph Error: calling addEdge() on a nonexistant vertex\n");
        exit(EXIT_FAILURE);
    }
    int x=u,y=v;
    int insertedbefore=0;
    G->size++;
    for(int a=0;a<2;a++)//loops twice to add an edge to each vertex
    {
        moveFront(G->neighbors[x]);
        int maxlength=length(G->neighbors[x]);
        insertedbefore=0;
        for(int i=0;i<maxlength;i++)//loop to insert the new edge in the right spot in the list(needs to be in ascending order)
        {
            if(get(G->neighbors[x])>=y)
            {
                insertBefore(G->neighbors[x],y);
                insertedbefore=1;
                break;
            }
            moveNext(G->neighbors[x]);
        }
        if(insertedbefore==0)
        {
            append(G->neighbors[x],y);
        }
        x=v;
        y=u;
    }
}
void addArc(Graph G, int u, int v)
{
    if(u<1 || u>getOrder(G))
    {
        printf("Graph Error: calling addEdge() on a nonexistant vertex\n");
        exit(EXIT_FAILURE);
    }
    if(v<1 || v>getOrder(G))
    {
        printf("Graph Error: calling addEdge() on a nonexistant vertex\n");
        exit(EXIT_FAILURE);
    }
    G->size++;
    moveFront(G->neighbors[u]);
    int maxlength=length(G->neighbors[u]);
    int insertedbefore=0;

    for(int i=0;i<maxlength;i++)
    {
        if(get(G->neighbors[u])>=v)
        {
            insertBefore(G->neighbors[u],v);
            insertedbefore=1;
            break;
        }
        moveNext(G->neighbors[u]);
    }
    if(insertedbefore==0)
    {
        append(G->neighbors[u],v);
    }
}
void BFS(Graph G, int s)//SOURCE: https://www.youtube.com/watch?v=oDqjPvD54Ss&feature=emb_title
{
    clear(G->queue);//emptying the queue
    append(G->queue,s);
    for(int i=1;i<=getOrder(G);i++)
    {
        G->color[i]='W';//setting every vertex to undiscovered(white)
        G->parent[i]=0;//setting the parent array to all zero
    }
    
    while(length(G->queue)!=0)
    {
        moveFront(G->queue);
        int currentvertex=get(G->queue);
        G->color[currentvertex]='B';
        deleteFront(G->queue);

        moveFront(G->neighbors[currentvertex]);
        for(int i=0;i<length(G->neighbors[currentvertex]);i++)//looping over each of the currentvertex's neighbors to see if it needs to queue them
        {
            if(G->color[get(G->neighbors[currentvertex])]=='W')//adding a neighbor if it is white
            {
                append(G->queue,get(G->neighbors[currentvertex]));
                G->color[get(G->neighbors[currentvertex])]='G';//changing color of queued vertex
                G->parent[get(G->neighbors[currentvertex])]=currentvertex;
            }
            moveNext(G->neighbors[currentvertex]);
        }
    }
    G->recentsource=s;
}

/*** Other operations ***/

void printGraph(FILE* out, Graph G)
{
    for(int i=1;i<=G->order;i++)
    {
        moveFront(G->neighbors[i]);//moves all of the list cursors in neighbors to the front
    }

    for(int i=1;i<=G->order;i++)
    {
        fprintf(out,"%d: ",i);//printing the vertex label
        for(int j=0;j<length(G->neighbors[i]);j++)
        {
            fprintf(out,"%d ",get(G->neighbors[i]));//loop that prints all neighbors
            moveNext(G->neighbors[i]);
        }
        fprintf(out,"\n");//moving to a new line
    }
}