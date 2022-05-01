#include "quick.h"

int partition(Stats *stats, uint32_t *A, int lo, int hi) {
    int i = lo - 1;
    for (int j = lo; j < hi; j++) {
        if (cmp(stats, A[j - 1], A[hi - 1]) == -1) {
            i += 1;
            swap(stats, &A[i - 1], &A[j - 1]);
        }
    }
    swap(stats, &A[i], &A[hi - 1]);
    return i + 1;
}

void quick_sorter(Stats *stats, uint32_t *A, int lo, int hi) {
    int p;
    if (lo < hi) {
        p = partition(stats, &A[0], lo, hi);
        quick_sorter(stats, &A[0], lo, p - 1);
        quick_sorter(stats, &A[0], p + 1, hi);
    }
}

void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    //This is literally just a translation from Long's provided python code
    quick_sorter(stats, &A[0], 1, (int) n);
}

//Long's provided python code
/*
def partition(A: list, lo: int, hi: int):
    i=lo-1
    for j in range(lo, hi):
        if A[j-1] < A[hi-1]:
            i+=1
            A[i-1],A[j-1] = A[j-1], A[i-1]
    A[i], A[hi-1] = A[hi-1], A[i]
    return i+1

def quick_sorter(A: list, lo: int, hi: int):
    if lo<hi:
        p=partition(A, lo ,hi)
        quick_sorter(A,lo,p-1)
        quick_sorter(A,p+1,hi)

def quick_sort(A:list):
    quick_sorter(A,1,len(A))
    */
