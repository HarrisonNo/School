/***********************************************************************
 * Harrison Quiring, hquiring
 * 2021 Spring CSE 101 pa3
 * Graph.h
 * Header file for functions found in Graph ADT
 * ***********************************************************************/
#ifndef GRAPH_H
#define GRAPH_H
#include "List.h"
typedef struct graphobject* Graph;

#define INF -1
#define NIL 0
#define UNDEF -2

/*** Constructors-Destructors ***/

//Returns a graph pointint to a newly created GraphObj representing a graph having n vertices and no edges
Graph newGraph(int n);
//Frees all dynamic memory associated with the Graph *pG, then sets the handle *pG to NULL
void freeGraph(Graph* pG);

/*** Access functions ***/

//Returns the corresponding Order of Graph G
int getOrder(Graph G);
//Returns the corresponding Size of Graph G
int getSize(Graph G);
//Returns the source vertex most recently used in function BFS()
int getSource(Graph G);
//Returns the parent of vertex u in the Breadth-First tree created by BFS(), or NIL if BFS() has not yet been called
//Precondition of 1<=u<=getOrder(G)
int getParent(Graph G, int u);
//Returns the distance from the most recent BFS source to vertex u, or INF if BFS() has not yet been called
//Precondition of 1<=u<=getOrder(G)
int getDist(Graph G, int u);
//Appends to the List L the vertices of a shortest path in G from source to u, or appends to L the value NIL if no such path exists
//Precondition of getSource(G)!=NIL, so BFS() must be called before getPath()
//Precondition of 1<=u<=getOrder(G)
void getPath(List L, Graph G, int u);

/* Access functions specific to DFS*/
int getDiscover(Graph G, int u); /*Pre: 1<=u<=n=getOrder(G)*/
int getFinish(Graph G, int u); /*Pre: 1<=u<=n=getOrder(G)*/


/*** Manipulation procedures ***/

//Deletes all edges of G, restoring it to its original(no edge) state
void makeNull(Graph G);
//Inserts a new edge joining u to v, i.e. u is added to the adjacency List of v, and v to the adjacency List of u
//Precondition that its two int arguments must lie in the range 1 to getOrder(G)
void addEdge(Graph G, int u, int v);
//Inserts a new directed edge from u to v, i.e. v is added to the adjacency List of u(but not u to the adjacency List of v)
//Precondition that its two int arguments must lie in the range 1 to getOrder(G)
void addArc(Graph G, int u, int v);
//Runs the BFS algorithm on the Graph G with source s, setting the color, distance, parent, and source fields of G accordingly
void BFS(Graph G, int s);

/*Manipulation procedures specific to DFS*/
void DFS(Graph G, List S); /*Pre: length(S)==getOrder(G)*/
void Visit(Graph G, int *time);

/*** Other operations ***/

//Prints the adjacency list representation of G to the file pointed to by out
//The format of this representation should match the above examples, so all that is required by the client is a single call to printGraph()
void printGraph(FILE* out, Graph G);//WILL NEED TO CHANGE TO MATCH DFS

/* Other operations specific to DFS*/
Graph transpose(Graph G);
Graph copyGraph(Graph G);

#endif