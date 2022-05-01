#include <unistd.h>
#include <stdlib.h> //atoi
#include <stdio.h> //printf
#include <ctype.h> //tolower
#include <inttypes.h>
#include <string.h> //strlen
#include "ht.h"
#include "bf.h"
#include "parser.h"
#include "messages.h"
#include "node.h"

#define OPTIONS "ht:f:s"
//Credit for regex: https://stackoverflow.com/questions/47545450/regex-match-any-single-character-one-character-only
#define WORD "[a-zA-Z0-9_']+(-[a-zA-Z0-9'_]+)*" //Potentially needs more?
//old regex [a-zA-Z0-9_'-]+

void PrintHelpPage();

int main(int NumberOfArgs, char *Strings[]) {
    int opt = 0, HashTableSize = 65536, BloomFilterSize = 1048576;
    bool PrintStats = false;
    char *FirstWord = malloc(100 * sizeof(char)),
         *SecondWord = malloc(100 * sizeof(char)); //technically a max string size of 100
    char *OGFirst = FirstWord, *OGSecond = SecondWord;
    Node *PlaceHolder = NULL;
    Node *BadSpeakTree = bst_create();
    Node *OldSpeakTree = bst_create();
    double HTLoad = 0, BFLoad = 0;
    double AverageBranchTrav = 0;
    //get user input
    while ((opt = getopt(NumberOfArgs, Strings, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': //Prints out a help message describing the purpose of the program and the command-line options it accepts, exiting the program afterwards.
            PrintHelpPage();
            free(OGFirst);
            free(OGSecond);
            return 0;
        case 't': //specifies that the hash table will have size entries (the default will be 2^16).
            HashTableSize = atoi(optarg);
            if (HashTableSize < 1) {
                printf("Invalid hash table size.\n");
                free(OGFirst);
                free(OGSecond);
                return 0;
            }
            break;
        case 'f': //specifies that the Bloom filter will have size entries (the default will be 2^20).
            BloomFilterSize = atoi(optarg);
            if (BloomFilterSize < 1) {
                printf("Invalid bloom filter size.\n");
                free(OGFirst);
                free(OGSecond);
                return 0;
            }
            break;
        case 's': //prints various statistics to stdout
            PrintStats = true;
            break;
        }
    }
    //Open text files
    FILE *BadSpeakList = fopen("badspeak.txt", "r");
    if (BadSpeakList == NULL) {
        printf("Could not open badspeak.txt, exiting program.\n");
        return 0;
    }
    FILE *OldNewSpeakList = fopen("newspeak.txt", "r");
    if (OldNewSpeakList == NULL) {
        printf("Could not open newspeak.txt, exiting program.\n");
        return 0;
    }
    //Scan in words from oldspeak.txt and populate bf/ht
    BloomFilter *BF = bf_create(BloomFilterSize);
    HashTable *HT = ht_create(HashTableSize);
    while (fscanf(BadSpeakList, "%s\n", FirstWord) != EOF) {
        bf_insert(BF, FirstWord);
        ht_insert(HT, FirstWord, NULL);
    }
    //Scan in words from newspeak.txt and populate bf/ht
    while (fscanf(OldNewSpeakList, "%s %s\n", FirstWord, SecondWord) != EOF) {
        bf_insert(BF, FirstWord);
        ht_insert(HT, FirstWord, SecondWord);
    }
    //Create regex stuff
    regex_t reg;
    if (regcomp(&reg, WORD, REG_EXTENDED)) //taken from asgn7 pdf
    {
        printf("Failed to compile regex.\n");
        return 0;
    }
    FirstWord = NULL;
    //Read in stdin input using parser.h
    while ((FirstWord = next_word(stdin, &reg)) != NULL) {
        for (unsigned long i = 0; i < strlen(FirstWord); i++) //convert word to lowercase
        {
            FirstWord[i] = tolower(FirstWord[i]);
        }
        if (bf_probe(BF, FirstWord)) //bad word exists
        {
            PlaceHolder = ht_lookup(HT, FirstWord);
            if (PlaceHolder
                != NULL) //bf returned false positive if NULL, word does not exist in hashtable
            {
                if (PlaceHolder->newspeak == NULL) //word is badspeak
                {
                    //Used for printing later
                    BadSpeakTree = bst_insert(BadSpeakTree, FirstWord, NULL);
                } else //word is simply oldspeak
                {
                    OldSpeakTree = bst_insert(OldSpeakTree, FirstWord, PlaceHolder->newspeak);
                }
            }
        }
    }
    //Should not print any transgression messages when printing stats
    if (PrintStats == false) {
        //Notifying user of any transgressions
        if (bst_size(BadSpeakTree) != 0 && bst_size(OldSpeakTree) != 0) //mixspeak
        {
            printf("%s", mixspeak_message);
            //Print out badspeak words
            bst_print(BadSpeakTree);
            //Print out oldspeak words
            bst_print(OldSpeakTree);
        } else if (bst_size(BadSpeakTree) != 0) //badspeak
        {
            printf("%s", badspeak_message);
            //Print out badspeak words
            bst_print(BadSpeakTree);
        } else if (bst_size(OldSpeakTree) != 0) //oldspeak
        {
            printf("%s", goodspeak_message);
            //Print out oldspeak words
            bst_print(OldSpeakTree);
        }
    } else //Print out stats
    {
        //First calculate stats
        AverageBranchTrav = (double) branches / (double) lookups;
        HTLoad = 100 * ((double) ht_count(HT) / (double) ht_size(HT));
        BFLoad = 100 * ((double) bf_count(BF) / (double) bf_size(BF));
        //Print stats
        printf("Average BST size: %0.6f\n", ht_avg_bst_size(HT));
        printf("Average BST height: %0.6f\n", ht_avg_bst_height(HT));
        printf("Average branches traversed: %0.6f\n", AverageBranchTrav);
        printf("Hash table load: %0.6f%%\n",
            HTLoad); //double percent is apparently needed to actually just print a percent sign
        printf("Bloom filter load: %0.6f%%\n", BFLoad);
    }
    //Free all necessary stuff
    clear_words(); //may need to move this to earlier
    bst_delete(&BadSpeakTree);
    bst_delete(&OldSpeakTree);
    ht_delete(&HT);
    bf_delete(&BF);
    free(OGFirst);
    free(OGSecond);
    fclose(BadSpeakList);
    fclose(OldNewSpeakList);
    regfree(&reg);
    return 0;
}

void PrintHelpPage() {
    printf("SYNOPSIS\n");
    printf("\tA word filtering program for the GPRSC.\n");
    printf("\tFilters out and reports bad words parsed from stdin.\n\n");
    printf("USAGE\n");
    printf("\t./banhammer [-hs] [-t size] [-f size]\n\n");
    printf("OPTIONS\n");
    printf("\t-h\t\tProgram usage and help.\n");
    printf("\t-s\t\tPrint program statistics.\n");
    printf("\t-t size\tSpecify hash table size (default: 2^16).\n");
    printf("\t-f size\tSpecify Bloom filter size (default: 2^20).\n");
    return;
}
