#include "insert.h"

void insertion_sort(Stats *stats, uint32_t *A, uint32_t n) {
    //This is literally just a translation from Long's provided python code
    uint32_t j, temp;
    for (uint32_t i = 1; i < n; i++) {
        j = i;
        temp = move(stats, A[i]);
        while (j > 0 && cmp(stats, temp, A[j - 1]) == -1) {
            A[j] = move(stats, A[j - 1]);
            j -= 1;
        }
        A[j] = move(stats, temp);
    }
}

//Long's provided python code
/*
def insertion_sort(A:list):
    for i in range(1,len(A)):
        j=i;
        temp=A[i]
        while j>0 and temp<A[j-i]:
            A[j]=A[j-1]
            j-=1
        A[j]=temp
*/
