#include "header.h"
#include "defines.h"
#include "huffman.h"
#include "io.h"
#include "node.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

#define OPTIONS "hi:o:v"

void PrintHelpPage();

int main(int NumberOfArgs, char *Strings[]) {
    int opt = 0;
    int Inputfile = 1; //default
    int Outputfile = 1;
    uint8_t Bit;
    bool verbose = false;
    while ((opt = getopt(NumberOfArgs, Strings, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': //Prints out a help message describing the purpose of the program and the command-line op-tions it accepts, exiting the program afterwards.
            PrintHelpPage();
            return 0;
        case 'i': //Specifies the input file to decode using Huffman coding. The default input should be set as stdin.
            Inputfile = open(optarg, O_RDONLY);
            if (Inputfile < 0) {
                fprintf(stdout, "Error: failed to open infile");
                return 0;
            }
            break;
        case 'o': //Specifies the output file to write the decompressed input to. The default output should be set as stdout.
            Outputfile = open(optarg, O_WRONLY | O_CREAT);
            if (Outputfile < 0) {
                fprintf(stdout, "Error: failed to open outfile");
                return 0;
            }
            break;
        case 'v': //Prints decompression statistics to stderr. These statistics include the compressed file size, the decompressed file size, and space saving.
            verbose = true;
            break;
        }
    }
    Header DecodeHeader;
    read(Inputfile, &DecodeHeader, sizeof(Header));
    if (DecodeHeader.magic != MAGIC) //exit if file does not have magic number
    {
        printf("Error: Input file does not contain the magic number.\n");
        return 0;
    }
    fchmod(Outputfile, DecodeHeader.permissions);
    //Read the dumped tree
    uint8_t *tree_dump = (uint8_t *) calloc(DecodeHeader.tree_size, sizeof(uint8_t));
    for (uint16_t i = 0; i < DecodeHeader.tree_size; i++) {
        read_bytes(Inputfile, &(tree_dump[i]), 1); //read 1 section of the tree at a time
    }
    Node *RootNode = rebuild_tree(DecodeHeader.tree_size, tree_dump); //Rebuild tree
    //Read file one bit at a time
    Node *CurrentNode = RootNode;
    while (read_bit(Inputfile, &Bit)) //Should stop once it reaches the end of the Inputfile
    {
        if (Bit == 1) //go right if able
        {
            if (CurrentNode->right != NULL) {
                CurrentNode = CurrentNode->right;
            } else //has found a leaf
            {
                write_bytes(Outputfile, &(CurrentNode->symbol), 1);
                CurrentNode = RootNode; //move back to start
                if (CurrentNode->right
                    != NULL) //presumably right node does exist but just to be safe...
                {
                    CurrentNode = CurrentNode->right;
                }
            }
        } else //go left if able
        {
            if (CurrentNode->left != NULL) {
                CurrentNode = CurrentNode->left;
            } else //has found a leaf
            {
                write_bytes(Outputfile, &(CurrentNode->symbol), 1);
                CurrentNode = RootNode; //move back to start
                if (CurrentNode->left
                    != NULL) //presumably left node does exist but just to be safe...
                {
                    CurrentNode = CurrentNode->left;
                }
            }
        }
    }
    //Free and close everything
    delete_tree(&RootNode);
    free(tree_dump);
    tree_dump = NULL;
    close(Inputfile);
    close(Outputfile);
    return 0;
}

void PrintHelpPage() {
    printf("SYNOPSIS\n");
    printf("\tA Huffman decoder.\n");
    printf("\tDecompress a file using the Huffman coding algorithm.\n\n");
    printf("USAGE\n");
    printf("\t./decode [-h] [-i infile] [-o outfile]\n\n");
    printf("OPTIONS\n");
    printf("\t-h\t\tProgram usage and help.\n");
    printf("\t-v\t\tPrint compression statistics.\n");
    printf("\t-i infile\tInput file to decompress.\n");
    printf("\t-o outfile\tOutput of decompressed data.\n");
}
