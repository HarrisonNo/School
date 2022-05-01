#include "code.h"
#include <stdio.h>

#define BITS_PER_UNIT 8

Code code_init(void) {
    Code C;
    C.top = 0;
    for (int i = 0; i < MAX_CODE_SIZE; i++) {
        C.bits[i] = 0;
    }
    return C;
}

uint32_t code_size(Code *c) {
    return c->top;
}

bool code_empty(Code *c) {
    if (c->top) //if number is not zero
    {
        return false;
    } else {
        return true;
    }
}

bool code_full(Code *c) {
    if (c->top >= MAX_CODE_SIZE) {
        return true;
    } else {
        return false;
    }
}

bool code_set_bit(Code *c, uint32_t i) {
    if (i > MAX_CODE_SIZE || i < 0) {
        return false;
    }
    c->bits[i / BITS_PER_UNIT] |= (0x1 << i % BITS_PER_UNIT); //TAKEN FROM THE CODE COMMENTS GITLAB
    return true;
}

bool code_clr_bit(Code *c, uint32_t i) {
    if (i > MAX_CODE_SIZE || i < 0) {
        return false;
    }
    c->bits[i / BITS_PER_UNIT] &= ~(0x1 << i % BITS_PER_UNIT); //TAKEN FROM THE CODE COMMENTS GITLAB
    return true;
}

bool code_get_bit(Code *c, uint32_t i) {
    if (i > MAX_CODE_SIZE || i < 0) {
        return false;
    }
    return (c->bits[i / BITS_PER_UNIT] >> i % BITS_PER_UNIT)
           & 0x1; //TAKEN FROM THE CODE COMMENTS GITLAB
}

bool code_push_bit(Code *c, uint8_t bit) {
    if (code_full(c)) {
        return false;
    }
    if (bit == 1) {
        code_set_bit(c, c->top);
    } else {
        code_clr_bit(c, c->top);
    }
    c->top++;
    return true;
}

bool code_pop_bit(Code *c, uint8_t *bit) {
    if (code_empty(c)) {
        return false;
    }
    c->top--;
    *bit = code_get_bit(c, c->top);
    return true;
}

void code_print(Code *c) {
    for (int i = MAX_CODE_SIZE; i > 0; i--) {
        if (i % BITS_PER_UNIT == 0) {
            printf(" ");
        }
        printf("%d", code_get_bit(c, i));
    }
    printf("\n");
}
