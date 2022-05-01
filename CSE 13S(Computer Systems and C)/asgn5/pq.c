#include "pq.h"
#include <stdlib.h>

/*
Since the pdf stated that pq was up to our interpretation as to how to implement it, I opted to make it via a sort of linked list.
The key thing to note is that the objects that make up the pseudo linked list are not called 'nodes' but instead 'layers' since nodes was already taken.
Apart from that this functions almost identically to a linked list except that enque is done to specifically order the pq from lowest->highest freq.
*/

typedef struct Layer Layer; //Necessary in order to reference a Layer* within the Layer object
struct Layer {
    Node *HeldNode;
    Layer *Left;
    Layer *Right;
};

struct PriorityQueue {
    uint32_t capacity;
    uint32_t Size;
    Layer *Front;
    Layer *Back;
};

Layer *CreateLayer(Node *N) {
    Layer *L = (Layer *) calloc(1, sizeof(Layer));
    L->HeldNode = N;
    L->Left = NULL;
    L->Right = NULL;
    return L;
}

void DeleteLayer(Layer **L) //Does not free the attached node
{
    free(*L);
    *L = NULL;
    return;
}

PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *PQ = (PriorityQueue *) calloc(1, sizeof(PriorityQueue));
    PQ->capacity = capacity;
    PQ->Front = NULL;
    PQ->Back = NULL;
    return PQ;
}

void pq_delete(PriorityQueue **q) {
    Node *TempNode;
    while (!pq_empty(*q)) {
        dequeue(*q, &TempNode);
        node_delete(&TempNode);
    }
    free(*q);
    *q = NULL;
    return;
}

bool pq_empty(PriorityQueue *q) {
    if (q->Size == 0) {
        return true;
    } else {
        return false;
    }
}

bool pq_full(PriorityQueue *q) {
    if (q->Size == q->capacity) {
        return true;
    } else {
        return false;
    }
}

uint32_t pq_size(PriorityQueue *q) {
    return q->Size;
}

bool enqueue(PriorityQueue *q, Node *n) //Inserts new node in proper position to other nodes
{
    Layer *AddedLayer = CreateLayer(n);
    if (pq_full(q)) {
        return false;
    }
    if (pq_empty(q)) {
        q->Front = q->Back = AddedLayer;
        q->Size++;
        return true;
    }
    for (
        Layer *TempLayer = q->Back; TempLayer != q->Front;
        TempLayer
        = TempLayer
              ->Left) //Starting at the back, loop through every layer to find the proper place to store the new node
    {
        if (n->frequency
            >= TempLayer->HeldNode
                   ->frequency) //queue goes from low freq->high freq, so if current added node freq is greater than node's freq to the left, insert node to the right
        {
            AddedLayer->Left = TempLayer;
            AddedLayer->Right = TempLayer->Right;
            TempLayer->Right = AddedLayer;
            if (TempLayer == q->Back) {
                q->Back = AddedLayer;
            }
            q->Size++;
            return true;
        }
    }
    //Finished loop with TempLayer being set to the front and didn't insert the added node, so the new node should get added to the front
    q->Front->Left = AddedLayer;
    AddedLayer->Right = q->Front;
    q->Front = AddedLayer;
    q->Size++;
    return true;
}

bool dequeue(PriorityQueue *q, Node **n) //Removes front node
{
    if (pq_empty(q)) {
        return false; //Instantly exit if empty
    }
    *n = q->Front->HeldNode;
    if (q->Size == 1) {
        DeleteLayer(&(q->Front));
        q->Back = NULL;
    } else {
        q->Front = q->Front->Right;
        DeleteLayer(&(q->Front->Left));
    }
    q->Size--;
    return true;
}

void pq_print(PriorityQueue *q);
