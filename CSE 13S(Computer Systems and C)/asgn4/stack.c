#include "stack.h"

#include <stdlib.h>

struct Stack {
    uint32_t top; //Index of next empty slot
    uint32_t capacity;
    uint32_t *items;
};

Stack *stack_create(uint32_t capacity) { //TAKEN FROM LONG'S ASGN4 PDF
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (uint32_t *) calloc(capacity, sizeof(uint32_t));
        if (!s->items) {
            free(s);
            s = NULL;
        }
    }
    return s;
}

void stack_delete(Stack **s) { //TAKEN FROM LONG'S ASGN4 PDF
    if (*s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
    return;
}

uint32_t stack_size(Stack *s) {
    return s->top; //Returns number of items actually in the array
}

bool stack_empty(Stack *s) {
    if (s->top) {
        return false;
    } //Returns false when there is a non-zero number
    else {
        return true;
    }
}

bool stack_full(Stack *s) {
    if (stack_size(s) >= s->capacity) {
        return true;
    } else {
        return false;
    }
}

bool stack_push(Stack *s, uint32_t x) {
    if (stack_full(s)) {
        return false; //Immediately break if the stack is already full
    }
    s->items[s->top] = x;
    s->top++;
    return true;
}

bool stack_pop(Stack *s, uint32_t *x) {
    if (stack_empty(s)) {
        return false; //Immediately break if the stack is already empty
    }
    *x = (s->items[(s->top) - 1]);
    s->top--;
    return true;
}

bool stack_peek(Stack *s, uint32_t *x) {
    if (stack_empty(s)) {
        return false; //If stack is empty just return false
    }
    *x = (s->items[(s->top) - 1]);
    return true;
}

void stack_copy(Stack *dst, Stack *src) {
    for (uint32_t x = 0; stack_pop(dst, &x);) {
    }; //continously empties the dst stack until it is actually empty
    if (src->capacity
        != dst->capacity) //IF THE TWO STACKS ARE NOT THE SAME IN CAPACITY, OVERWRITE THE DESTINATION STACK
    {
        free((dst)->items);
        dst->items = (uint32_t *) calloc(src->capacity, sizeof(uint32_t));
        dst->capacity = src->capacity;
    }
    for (uint32_t i = 0; i < stack_size(src); i++) {
        src->items[i] = dst->items[i];
    }
    dst->top = src->top;
    return;
}

void stack_print(Stack *s, FILE *outfile, char *cities[]) {
    for (uint32_t i = 0; i < s->top; i += 1) //TAKEN FROM LONG'S ASGN4.pdf
    {
        fprintf(outfile, "%s", cities[s->items[i]]);
        if (i + 1 != s->top) {
            fprintf(outfile, " -> ");
        }
    }
    fprintf(outfile, "\n");
}
