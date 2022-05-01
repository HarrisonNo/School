#include "shell.h"

#include <math.h>

uint32_t gaps(uint32_t n) {
    static uint32_t iteration = 0;
    uint32_t i;
    i = (uint32_t) log(3 + 2 * n) / log(3);
    i = i - (iteration); //7 moves
    iteration++;
    return i;
}

void shell_sort(Stats *stats, uint32_t *A, uint32_t n) {
    //This is literally just a translation from Long's provided python code
    uint32_t j, temp;
    for (uint32_t gap = gaps(n); gap < n; gap = gaps(n)) /*6 moves*/ {
        for (uint32_t i = gap; cmp(stats, i, n) == -1; i++) /*2991 compares*/ {
            j = i; //2985 moves
            temp = move(stats, A[i]);
            while (j >= gap && temp < A[j - gap]) /* compares*/ {
                A[j] = A[j - gap]; //14080 moves
                j -= gap;
            }
            A[j] = move(stats, temp);
        }
    }
}

//Long's provided python code
/*
def gaps(n: int):
    for i in range(int(log(3+2*n)/log(3)),0,-1):
        yield(3**i-1)//2

def shell_sort(A: list):
    for gap in gaps(len(A)):
        for i in range(gap,len(A)):
            j=i
            temp=A[i]
            while j>=gap and temp<A[j-gap]:
                A[j]= A[j-gap]
                j-=gap
            A[j]=temp
*/
