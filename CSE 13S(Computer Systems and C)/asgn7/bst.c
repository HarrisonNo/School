#include "bst.h"
#include <string.h> //needed for strcmp

void RecursiveHeightCalculator(Node *CurrentNode, uint32_t *CurrentHeight, uint32_t *RecordHeight);
void RecursiveSizeCalculator(Node *CurrentNode, uint32_t *Size);

uint64_t branches = 0;

Node *bst_create(void) {
    return NULL; //all that is necessary?
}

uint32_t bst_height(Node *root) {
    if (root == NULL) {
        return 0;
    }
    uint32_t CurrentHeight = 1, RecordHeight = 0;
    RecursiveHeightCalculator(root, &CurrentHeight, &RecordHeight);
    return RecordHeight;
}

uint32_t bst_size(Node *root) {
    if (root == NULL) {
        return 0;
    }
    uint32_t Size = 1;
    RecursiveSizeCalculator(root, &Size);
    return Size;
}

Node *bst_find(Node *root, char *oldspeak) {
    branches++;
    if (root == NULL) {
        return NULL;
    }
    if (strcmp(root->oldspeak, oldspeak) == 0) {
        return root; //found the desired node
    } else if (strcmp(root->oldspeak, oldspeak) > 0) {
        return bst_find(root->left, oldspeak); //look to the left of the current node
    } else if (strcmp(root->oldspeak, oldspeak) < 0) {
        return bst_find(root->right, oldspeak); //look to the right of the current node
    }
    return NULL; //should be completely unnecessary
}

Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    if (root == NULL) {
        Node *N = node_create(oldspeak, newspeak);
        return N;
    }
    Node *CurrentNode = root;
    while (1) //continuously dive to find where to insert the node
    {
        branches++;
        if (strcmp(CurrentNode->oldspeak, oldspeak) == 0) //node already exists
        {
            return NULL;
        }
        if (strcmp(CurrentNode->oldspeak, oldspeak) < 0) //look to right
        {
            if (CurrentNode->right != NULL) //right node exists
            {
                CurrentNode = CurrentNode->right;
            } else //right node does not exists, insert here
            {
                Node *N = node_create(oldspeak, newspeak);
                CurrentNode->right = N;
                return root;
            }
        } else if (strcmp(CurrentNode->oldspeak, oldspeak) > 0) //look to left
        {
            if (CurrentNode->left != NULL) //left node exists
            {
                CurrentNode = CurrentNode->left;
            } else //right node does not exists, insert here
            {
                Node *N = node_create(oldspeak, newspeak);
                CurrentNode->left = N;
                return root;
            }
        }
    }
    return root;
}

void bst_print(Node *root) {
    if (root == NULL) {
        return;
    }
    if (root->left != NULL) {
        bst_print(root->left);
    }
    node_print(root);
    if (root->right != NULL) {
        bst_print(root->right);
    }
    return;
}

void bst_delete(Node **root) {
    if (root == NULL) {
        return;
    }
    if (*root == NULL) {
        return;
    }
    if ((*root)->left != NULL) {
        bst_delete(&((*root)->left));
    }
    if ((*root)->right != NULL) {
        bst_delete(&((*root)->right));
    }
    node_delete(root);
    return;
}

//Recursive function that calculates the height of the tree via recursion
void RecursiveHeightCalculator(Node *CurrentNode, uint32_t *CurrentHeight, uint32_t *RecordHeight) {
    if (CurrentNode->left != NULL) {
        (*CurrentHeight)++;
        RecursiveHeightCalculator(CurrentNode->left, CurrentHeight, RecordHeight);
    }
    if (CurrentNode->right != NULL) {
        (*CurrentHeight)++;
        RecursiveHeightCalculator(CurrentNode->right, CurrentHeight, RecordHeight);
    }
    if (*CurrentHeight > *RecordHeight) //set new record height
    {
        *RecordHeight = *CurrentHeight;
    }
    (*CurrentHeight)--;
    return;
}

void RecursiveSizeCalculator(Node *CurrentNode, uint32_t *Size) {
    if (CurrentNode->left != NULL) {
        (*Size)++;
        RecursiveSizeCalculator(CurrentNode->left, Size);
    }
    if (CurrentNode->right != NULL) {
        (*Size)++;
        RecursiveSizeCalculator(CurrentNode->right, Size);
    }
    return;
}
