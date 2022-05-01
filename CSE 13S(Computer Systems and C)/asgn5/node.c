#include "node.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *N = (Node *) calloc(1, sizeof(Node));
    N->symbol = symbol;
    N->frequency = frequency;
    N->left = NULL;
    N->right = NULL;
    return N;
}

void node_delete(Node **n) {
    free(*n);
    *n = NULL;
    return;
}

Node *node_join(Node *left, Node *right) {
    uint64_t LeftFreq = 0, RightFreq = 0;
    //kept on segfaulting and this is the only bandaid I could come up with after multiple hours of debugging
    if (left != NULL) {
        LeftFreq = left->frequency;
    }
    if (right != NULL) {
        RightFreq = right->frequency;
    }
    Node *ParentNode = node_create('$', (LeftFreq + RightFreq));
    ParentNode->left = left;
    ParentNode->right = right;
    return ParentNode;
}

void node_print(Node *n) {
    printf("Node symbol: %" PRIu8 "\n", n->symbol);
    printf("Frequency: %" PRIu64 "\n", n->frequency);
    printf("Left symbol: %" PRIu8 "\n", n->left->symbol);
    printf("Right symbol: %" PRIu8 "\n", n->right->symbol);
    return;
}
