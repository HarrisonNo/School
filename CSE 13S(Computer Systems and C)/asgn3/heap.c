#include "heap.h"

#include <stdbool.h>

int max_child(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    //This is literally just a translation from Long's provided python code
    uint32_t left = 2 * first; //potential move here
    uint32_t right = left + 1; //potential move here
    if (right <= last && cmp(stats, A[right - 1], A[left - 1]) == 1) {
        return right;
    }
    return left;
}

void fix_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    //This is literally just a translation from Long's provided python code
    bool found = false;
    uint32_t mother = first; //potential move here
    uint32_t great = max_child(stats, &A[0], mother, last);
    while (mother <= (uint32_t) last / 2 && !found) {
        if (cmp(stats, A[mother - 1], A[great - 1]) == -1) {
            swap(stats, &A[mother - 1], &A[great - 1]);
            mother = great; //potential move here
            great = max_child(stats, A, mother, last); //potential move here
        } else {
            found = true;
        }
    }
}

void build_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    //This is literally just a translation from Long's provided python code
    for (uint32_t father = (int) last / 2; father > first - 1; father--) /*101*/ {
        fix_heap(stats, &A[0], father, last);
    }
}

void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    //This is literally just a translation from Long's provided python code
    uint32_t first = 1;
    uint32_t last = n; //potential move here
    build_heap(stats, &A[0], first, last);
    for (uint32_t leaf = last; leaf > first; leaf--) {
        swap(stats, &A[first - 1], &A[leaf - 1]);
        fix_heap(stats, &A[0], first, leaf - 1);
    }
}

//Long's provided python code
/*
def max_child(A: list, first: int, last: int):
    left=2*first
    right=left+1
    if right<=last and A[right-1]>A[left-1]:
        return right
    return left

def fix_heap(A: list, first: int, last: int):
    found=False
    mother=first
    great=max_child(A,mother,last)
    while mother<=last//2 and not found:
        if A[mother-1] <A[great-1]:
            A[mother-1], A[great-1] = A[great-1], A[mother-1]
            mother=great
            great=max_child(A,mother,last)
        else:
            found=True

def build_heap(A: list, first: int, last: int):
    for father in range(last//2,first-1,-1):
        fix_heap(A,father,last)

def heap_sort(A:list):
    first=1
    last=len(A)
    build_heap(A,first,last)
    for leaf in range(last, first, -1):
        A[first-1],A[leaf-1] = A[leaf-1],A[first-1]
        fix_heap(A,first,leaf-1)
*/
