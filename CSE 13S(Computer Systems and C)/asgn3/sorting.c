#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h> //USED FOR SRANDOM AND RANDOM
#include <unistd.h>

#define OPTIONS "aeisqr:n:p:h"
#define MaxInt  1073741824
void PrintHelpPage(int NumberOfArgs);
uint32_t *CreateRandomArray(long size, unsigned int seed);
void PrintSortStats(Stats *stats, uint32_t *Array, long arraysize, long elements, char *SortType);
int main(int NumberOfArgs, char *Strings[]) {
    if (NumberOfArgs == 1) //user input nothing(besides command to run program)
    {
        PrintHelpPage(NumberOfArgs);
        return 0; //exit program if nothing was specified after printing help page
    }
    int opt = 0; //This is taken from the Long's asgn2.pdf
    long arraysize = 100, printelements = 100; //default values
    uint32_t *MainArray = NULL;
    unsigned int randomseed = 13371453;
    int Tests[4] = { 0, 0, 0, 0 }; //Represents true/false for tests: heap,insertion,shell,quicksort
    //The reason for storing in an array is to ensure that everything prints in the same exact order as the binary since the getopt would otherwise just print the first test the user requests
    while ((opt = getopt(NumberOfArgs, Strings, OPTIONS)) != -1) {
        switch (opt) {
        case 'e': //Enables Heap Sort.
            Tests[0] = 1;
            break;
        case 'i': //Enables Insertion Sort.
            Tests[1] = 1;
            break;
        case 's': //Enables Shell Sort.
            Tests[2] = 1;
            break;
        case 'q': //Enables Quicksort.
            Tests[3] = 1;
            break;
        case 'h': //Display a help message detailing program usage.
            PrintHelpPage(NumberOfArgs); //Exit program after displaying help message
            return 0;
        case 'a': //Runs all tests.
            for (int i = 0; i < 4; i++) {
                Tests[i] = 1;
            } //Enables all tests
            break;
        case 'r': //Set the random seed to seed. The default seed should be 13371453.
            randomseed = atoi(optarg);
            break;
        case 'n': //Set the array size to size. The default size should be 100.
            arraysize = atoi(optarg);
            break;
        case 'p': //Print out elements number of elements from the array. The default number of elements to print out should be 100.
            printelements = atoi(optarg);
            break;
        }
    }
    if (!Tests[0] && !Tests[1] && !Tests[2] && !Tests[3]) //If didn't actually choose anything
    {
        NumberOfArgs = 1;
        PrintHelpPage(NumberOfArgs);
        return 0;
    }
    if (arraysize < printelements) {
        printelements = arraysize;
    }
    //HEAP SORT
    if (Tests[0] == 1) {
        Stats *HeapStats = (Stats *) malloc(sizeof(Stats));
        HeapStats->compares = 0; //Initilize values
        HeapStats->moves = 0;
        MainArray = CreateRandomArray(arraysize, randomseed);
        heap_sort(HeapStats, &MainArray[0], arraysize);
        PrintSortStats(HeapStats, &MainArray[0], arraysize, printelements, "Heap Sort");
        free(HeapStats);
        free(MainArray);
    }
    //SHELL SORT
    if (Tests[1] == 1) {
        Stats *ShellStats = (Stats *) malloc(sizeof(Stats));
        ShellStats->compares = 0; //Initilize values
        ShellStats->moves = 0;
        MainArray = CreateRandomArray(arraysize, randomseed);
        shell_sort(ShellStats, &MainArray[0], arraysize);
        PrintSortStats(ShellStats, &MainArray[0], arraysize, printelements, "Shell Sort");
        free(ShellStats);
        free(MainArray);
    }
    //INSERTION SORT
    if (Tests[2] == 1) {
        Stats *InsertionStats = (Stats *) malloc(sizeof(Stats));
        InsertionStats->compares = 0; //Initilize values
        InsertionStats->moves = 0;
        MainArray = CreateRandomArray(arraysize, randomseed);
        insertion_sort(InsertionStats, &MainArray[0], arraysize);
        PrintSortStats(InsertionStats, &MainArray[0], arraysize, printelements, "Insertion Sort");
        free(InsertionStats);
        free(MainArray);
    }
    //QUICK SORT
    if (Tests[3] == 1) {
        Stats *QuickStats = (Stats *) malloc(sizeof(Stats));
        QuickStats->compares = 0; //Initilize values
        QuickStats->moves = 0;
        MainArray = CreateRandomArray(arraysize, randomseed);
        quick_sort(QuickStats, &MainArray[0], arraysize);
        PrintSortStats(QuickStats, &MainArray[0], arraysize, printelements, "Quick Sort");
        free(QuickStats);
        free(MainArray);
    }
    return 0;
}

uint32_t *CreateRandomArray(long size, unsigned int seed) {
    uint32_t *DynamicArray;
    DynamicArray = calloc(size, sizeof(int)); //Need to free after using
    srandom(seed);
    for (int i = 0; i < size; i++) //fill array with random numbers
    {
        DynamicArray[i] = random() % MaxInt;
    }
    return DynamicArray;
}

void PrintSortStats(Stats *stats, uint32_t *Array, long arraysize, long elements, char *SortType) {
    printf("%s, %li elements, %" PRIu64 " moves, %" PRIu64 " compares\n", SortType, arraysize,
        stats->moves, stats->compares);
    for (int i = 0; i < elements;) {
        for (int j = 0; j < 5 && i < elements; j++) //only print 5 values in a row
        {
            printf("%13i", Array[i]);
            i++;
        }
        printf("\n");
    }
}

void PrintHelpPage(int NumberOfArgs) {
    if (NumberOfArgs == 1) {
        printf("Select at least one sort to perform.\n");
    }
    printf("SYNOPSIS\n");
    printf("SYNOPSIS\n");
    printf("\tA collection of comparison-based sorting algorithms.\n\n");
    printf("USAGE\n");
    printf("\t./sorting [-haeisqn:p:r:] [-n length] [-p elements] [-r seed]\n\n");
    printf("OPTIONS\n");
    printf("\t-h\t\tdisplay program help and usage.\n");
    printf("\t-a\t\tenable all sorts.\n");
    printf("\t-e\t\tenable Heap Sort.\n");
    printf("\t-i\t\tenable Insertion Sort.\n");
    printf("\t-s\t\tenables Shell Sort.\n");
    printf("\t-q\t\tenables Quick Sort.\n");
    printf("\t-n length\tspecify number of array elements (default: 100).\n");
    printf("\t-p elements\tspecify number of elements to print (default: 100).\n");
    printf("\t-r seed\t\tspecify random seed (default: 13371453).\n");
}
