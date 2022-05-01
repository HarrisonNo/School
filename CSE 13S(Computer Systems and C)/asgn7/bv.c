#include "bv.h"
#include <stdlib.h>
#include <stdio.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

#define BITS_PER_UNIT 8

BitVector *bv_create(uint32_t length) {
    BitVector *BV = (BitVector *) malloc(1 * sizeof(BitVector));
    BV->vector = calloc(length, sizeof(uint8_t)); //calloc initializes entire array to be zero
    BV->length = length;
    return BV;
}

void bv_delete(BitVector **bv) {
    if (bv != NULL) {
        free((*bv)->vector);
        free(*bv);
        *bv = NULL;
        bv = NULL;
    }
    return;
}

uint32_t bv_length(BitVector *bv) {
    if (bv != NULL) {
        return bv->length;
    }
    return 0;
}

bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length || i < 0) {
        return false;
    }
    bv->vector[i / 8] |= (0x1 << i % 8); //TAKEN FROM THE CODE COMMENTS GITLAB
    return true;
}

bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length || i < 0) {
        return false;
    }
    bv->vector[i / BITS_PER_UNIT]
        &= ~(0x1 << i % BITS_PER_UNIT); //TAKEN FROM THE CODE COMMENTS GITLAB
    return true;
}

bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length || i < 0) {
        return false;
    }
    return (bv->vector[i / BITS_PER_UNIT] >> i % BITS_PER_UNIT)
           & 0x1; //TAKEN FROM THE CODE COMMENTS GITLAB
}

void bv_print(BitVector *bv) //taken from my code.c
{
    for (int i = bv->length; i > 0; i--) {
        if (i % BITS_PER_UNIT == 0) {
            printf(" ");
        }
        printf("%d", bv_get_bit(bv, i));
    }
    printf("\n");
}
