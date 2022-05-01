#include "stack.h"
#include <stdlib.h>
struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

Stack *stack_create(uint32_t capacity) {
    Stack *S = (Stack *) calloc(1, sizeof(Stack));
    S->top = 0;
    S->capacity = capacity;
    S->items = (Node **) calloc(capacity, sizeof(Node *));
    return S;
}

void stack_delete(Stack **s) {
    Node *tempnode;
    while (!stack_empty(*s)) {
        stack_pop(*s, &tempnode); //empty the stack to ensure all of the nodes are freed
        node_delete(&tempnode);
    }
    free((*s)->items);
    free(*s);
    *s = NULL;
    return;
}

bool stack_empty(Stack *s) {
    if (s->top) //is only empty when top=0
    {
        return false;
    } else {
        return true;
    }
}

bool stack_full(Stack *s) {
    if (s->top >= s->capacity) {
        return true;
    } else {
        return false;
    }
}

uint32_t stack_size(Stack *s) {
    return s->top;
}

bool stack_push(Stack *s, Node *n) {
    if (stack_full(s)) {
        return false; //exit failed if stack is full
    }
    s->items[s->top] = n;
    s->top++;
    return true;
}

bool stack_pop(Stack *s, Node **n) {
    if (stack_empty(s)) {
        return false;
    }
    s->top--;
    *n = s->items[s->top];
    return true;
}

void stack_print(Stack *s) {
    for (uint32_t i = 0; i < s->capacity; i++) {
        node_print(s->items[i]);
    }
    return;
}
