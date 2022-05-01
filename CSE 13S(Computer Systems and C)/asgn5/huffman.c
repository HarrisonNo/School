#include "stdio.h" //Necessary for NULL
#include "huffman.h"
#include "pq.h"
#include "stack.h"
#include "io.h"
#include <stdlib.h>

bool RightChildIsMostRecentVisited(Node *CurrentNode, Node *MostRecentVisited);
void DumpTreePostOrder(int outfile, Node *CurrentNode, uint8_t *LChar, uint8_t *IChar);
void BuildCodesPostOrder(Node *CurrentNode, Code *table, Code *C, uint8_t *DummyInt);

Node *build_tree(uint64_t hist[static ALPHABET]) {
    bool NeedToRecreateNode = false;
    PriorityQueue *PQ = pq_create(ALPHABET);
    for (int i = 0; i < ALPHABET; i++) {
        Node *N = node_create(i, hist[i]);
        while (i < ALPHABET && hist[i] == 0) //If frequency is zero then move on to the next symbol
        {
            NeedToRecreateNode = true;
            i++;
        } //Don't have to worry about last symbol having freqency of zero as Long mandates we increment it by 1 so it can never be zero
        if (NeedToRecreateNode) {
            node_delete(&N); //free the previous useless node to create a new one
            N = node_create(i,
                hist[i]); //Recreate node outside of while loop for efficiency, while loop could potentially loop 100 times
            NeedToRecreateNode = false;
        }
        enqueue(PQ, N);
    } //end of creating priority queue from histogram
    Node *Left, *Right, *Parent;
    while (pq_size(PQ) >= 2) {
        dequeue(PQ, &Left);
        dequeue(PQ, &Right);
        Parent = node_join(Left, Right);
        enqueue(PQ, Parent);
    } //will be left with 1 node left in the priority queue
    Node *Root;
    dequeue(PQ, &Root);
    pq_delete(&PQ);
    return Root;
}

void build_codes(Node *root, Code table[static ALPHABET]) {
    Code C = code_init(); //keeps track of the code, 1 when going right, 0 when going left
    uint8_t DummyInt;
    //Post order traversal of tree
    BuildCodesPostOrder(root, &table[0], &C, &DummyInt);
    return;
}

void dump_tree(int outfile, Node *root) {
    uint8_t LChar = 'L', IChar = 'I';
    //Post order traversal of tree
    DumpTreePostOrder(outfile, root, &LChar, &IChar);
    return;
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    Stack *StackOfNodes = stack_create(nbytes);
    Node *LeftChild = NULL, *RightChild = NULL, *Parent = NULL, *NewLeaf = NULL;
    for (int i = 0; i < nbytes; i++) {
        if (tree[i] == 'L') {
            i++; //increment i to access the actual symbol of the leaf
            NewLeaf = node_create(tree[i], 0); //frequency does not matter so just set =0
            stack_push(StackOfNodes, NewLeaf);
        } else //found interior node
        {
            stack_pop(StackOfNodes, &RightChild);
            stack_pop(StackOfNodes, &LeftChild);
            Parent = node_join(LeftChild, RightChild);
            stack_push(StackOfNodes, Parent);
        }
    }
    stack_pop(StackOfNodes, &Parent);
    stack_delete(&StackOfNodes);
    return Parent; //contains the root node
}

void delete_tree(Node **root) {
    if ((*root)->left != NULL) {
        delete_tree(&((*root)->left));
    }
    if ((*root)->right != NULL) {
        delete_tree(&((*root)->right));
    }
    //If has reached this spot then there are no other children for this node
    free(*root);
    *root = NULL;
    return;
}

bool RightChildIsMostRecentVisited(Node *CurrentNode, Node *MostRecentVisited) //helper function
{
    if (CurrentNode->right != NULL) {
        if (CurrentNode->right == MostRecentVisited) {
            return true;
        }
    }
    return false;
}

//Separate functions for dump tree and build codes as they both do separate things once
//they finished a node
void DumpTreePostOrder(int outfile, Node *CurrentNode, uint8_t *LChar, uint8_t *IChar) {
    if (CurrentNode->left != NULL) {
        DumpTreePostOrder(outfile, CurrentNode->left, LChar, IChar);
    }
    if (CurrentNode->right != NULL) {
        DumpTreePostOrder(outfile, CurrentNode->right, LChar, IChar);
    }
    if (CurrentNode->left == NULL && CurrentNode->right == NULL) //found a leaf
    {
        write_bytes(outfile, LChar, 1);
        write_bytes(outfile, &(CurrentNode->symbol), 1);
    } else //found an interior node
    {
        write_bytes(outfile, IChar, 1);
    }
    return;
}

void BuildCodesPostOrder(Node *CurrentNode, Code *table, Code *C, uint8_t *DummyInt) {
    if (CurrentNode->left != NULL) {
        code_push_bit(C, 0);
        BuildCodesPostOrder(CurrentNode->left, table, C, DummyInt);
    }
    if (CurrentNode->right != NULL) {
        code_push_bit(C, 1);
        BuildCodesPostOrder(CurrentNode->right, table, C, DummyInt);
    }
    if (CurrentNode->left == NULL && CurrentNode->right == NULL) //found a leaf
    {
        table[CurrentNode->symbol] = *C; //assign code
    }
    code_pop_bit(C, DummyInt);
    return;
}
