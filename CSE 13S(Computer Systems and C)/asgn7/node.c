#include "node.h"
#include <stdlib.h> //needed for NULL
#include <string.h> //needed for strdup
#include <stdio.h>

Node *node_create(char *oldspeak, char *newspeak) {
    Node *N = (Node *) malloc(1 * sizeof(Node));
    N->left = NULL;
    N->right = NULL;
    if (oldspeak == NULL) {
        N->oldspeak = NULL;
    } else {
        N->oldspeak = strdup(oldspeak);
    }
    if (newspeak == NULL) {
        N->newspeak = NULL;
    } else {
        N->newspeak = strdup(newspeak);
    }
    return N;
}

void node_delete(Node **n) //Does not free left or right nodes
{
    if (n == NULL) {
        return;
    }
    if (*n == NULL) {
        n = NULL;
        return;
    }
    free((*n)->newspeak);
    free((*n)->oldspeak);
    free(*n);
    *n = NULL;
    n = NULL;
    return;
}

void node_print(Node *n) {
    if (n == NULL) {
        printf("NULL\n");
        return;
    }
    if (n->newspeak != NULL && n->oldspeak != NULL) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    } else if (n->oldspeak != NULL) {
        printf("%s\n", n->oldspeak);
    }
    return;
}
