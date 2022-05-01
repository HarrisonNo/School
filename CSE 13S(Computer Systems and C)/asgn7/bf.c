#include "bf.h"
#include "salts.h"
#include "speck.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct BloomFilter {
    uint64_t primary[2]; //Primary hash function salt
    uint64_t secondary[2]; //Secondary hash function salt
    uint64_t tertiary[2]; //Tertiary hash function salt
    BitVector *filter;
};

BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(1 * sizeof(BloomFilter));
    bf->primary[0] = SALT_PRIMARY_LO;
    bf->primary[1] = SALT_PRIMARY_HI;
    bf->secondary[0] = SALT_SECONDARY_LO;
    bf->secondary[1] = SALT_SECONDARY_HI;
    bf->tertiary[0] = SALT_TERTIARY_LO;
    bf->tertiary[1] = SALT_TERTIARY_HI;
    bf->filter = bv_create(size);
    return bf;
}

void bf_delete(BloomFilter **bf) {
    if (bf != NULL && *bf != NULL) {
        bv_delete(&((*bf)->filter));
        free(*bf);
        *bf = NULL;
        bf = NULL;
    }
    return;
}

uint32_t bf_size(BloomFilter *bf) {
    if (bf == NULL) //exit if nonexistant
    {
        return 0;
    }
    return bv_length(bf->filter);
}

void bf_insert(BloomFilter *bf, char *oldspeak) {
    if (bf == NULL) {
        return;
    }
    if (oldspeak == NULL) {
        return;
    }
    //% the hash to ensure it fits in the bitvector, may result in collisions with smaller words
    uint32_t PrimaryHash = hash(&(bf->primary[0]), oldspeak) % (bv_length(bf->filter));
    uint32_t SecondaryHash = hash(&(bf->secondary[0]), oldspeak) % (bv_length(bf->filter));
    uint32_t TertiaryHash = hash(&(bf->tertiary[0]), oldspeak) % (bv_length(bf->filter));
    bv_set_bit(bf->filter, PrimaryHash);
    bv_set_bit(bf->filter, SecondaryHash);
    bv_set_bit(bf->filter, TertiaryHash);
    return;
}

bool bf_probe(BloomFilter *bf, char *oldspeak) {
    if (bf == NULL) {
        return false;
    }
    //% the hash to ensure it fits in the bitvector, may result in collisions with smaller words
    uint32_t PrimaryHash = hash(&(bf->primary[0]), oldspeak) % (bv_length(bf->filter));
    uint32_t SecondaryHash = hash(&(bf->secondary[0]), oldspeak) % (bv_length(bf->filter));
    uint32_t TertiaryHash = hash(&(bf->tertiary[0]), oldspeak) % (bv_length(bf->filter));
    //if all three bits are set
    if (bv_get_bit(bf->filter, PrimaryHash) && bv_get_bit(bf->filter, SecondaryHash)
        && bv_get_bit(bf->filter, TertiaryHash)) {
        return true;
    }
    return false;
}

uint32_t bf_count(BloomFilter *bf) {
    if (bf == NULL) {
        return 0;
    }
    uint32_t NumberSetBits = 0;
    for (uint32_t i = 0; i < bv_length(bf->filter); i++) {
        if (bv_get_bit(bf->filter, i)) {
            NumberSetBits++;
        }
    }
    return NumberSetBits;
}

void bf_print(BloomFilter *bf) {
    printf("Primary low: %" PRIu64 "\n", bf->primary[0]);
    printf("Primary high: %" PRIu64 "\n", bf->primary[1]);
    printf("Secondary low: %" PRIu64 "\n", bf->secondary[0]);
    printf("Secondary high: %" PRIu64 "\n", bf->secondary[1]);
    printf("Tertiary low: %" PRIu64 "\n", bf->tertiary[0]);
    printf("Tertiary high: %" PRIu64 "\n", bf->tertiary[1]);
    bv_print(bf->filter);
}
