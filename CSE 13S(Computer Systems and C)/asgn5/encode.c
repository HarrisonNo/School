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

#define OPTIONS "hi:o:v"

void NumUniqueSymbols(Node *CurrentNode, uint16_t *Size);
void PrintHelpPage();

int main(int NumberOfArgs, char *Strings[]) {
    struct stat InputfileStat;
    int opt = 0;
    uint16_t TreeSize = 0;
    int Inputfile = 1; //default
    int Outputfile = 1;
    uint8_t InputChar;
    bool verbose = false;
    while ((opt = getopt(NumberOfArgs, Strings, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': //Prints out a help message describing the purpose of the program and the command-line op-tions it accepts, exiting the program afterwards.
            PrintHelpPage();
            return 0;
        case 'i': //Specifies the input file to encode using Huffman coding. The default input should be set as stdin.
            Inputfile = open(optarg, O_RDONLY);
            if (Inputfile < 0) {
                fprintf(stdout, "Error: failed to open infile");
                return 0;
            }
            break;
        case 'o': //Specifies the output file to write the compressed input to. The default output should be set as stdout.
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
    uint64_t Histogram[ALPHABET] = { 0 }; //create histogram
    Histogram[0]++;
    Histogram[255]++;
    while (read_bytes(Inputfile, &InputChar, 1)) //goes until no bytes are read
    {
        Histogram[InputChar]++;
    }
    //Create Huffman Tree
    Node *RootNode = build_tree(Histogram);
    //Construct a code table
    Code Table[ALPHABET];
    build_codes(RootNode, Table); //Honestly no idea why this modifies the original table
    //Construct header
    Header EncodeHeader;
    EncodeHeader.magic = MAGIC;
    //Get data from inputfile
    fstat(Inputfile, &InputfileStat);
    EncodeHeader.permissions = InputfileStat.st_mode;
    EncodeHeader.file_size = InputfileStat.st_size;
    fchmod(Outputfile, EncodeHeader.permissions);
    NumUniqueSymbols(RootNode, &TreeSize);
    TreeSize *= 3;
    TreeSize -= 1;
    EncodeHeader.tree_size = TreeSize;
    write(Outputfile, &EncodeHeader, sizeof(Header)); //print header
    //Print huffman tree to output
    dump_tree(Outputfile, RootNode);
    //Print converted version of input file to output
    lseek(Inputfile, 0, SEEK_SET); //moves to start of inputfile
    while (read_bytes(Inputfile, &InputChar, 1)) {
        write_code(
            Outputfile, &Table[InputChar]); //write codeified version of the char to the buffer
    }
    flush_codes(Outputfile); //toilet sounds
    if (verbose) {
        printf("Uncompressed file size: bytes\n");
        printf("Compressed file size: bytes\n");
        printf("Space saving: "); //in percent
    }
    //Free and close everything
    delete_tree(&RootNode);
    close(Inputfile);
    close(Outputfile);
    return 0;
}

void PrintHelpPage() {
    printf("SYNOPSIS\n");
    printf("\tA Huffman encoder.\n");
    printf("\tCompress a file using the Huffman coding algorithm.\n\n");
    printf("USAGE\n");
    printf("\t./encode [-h] [-i infile] [-o outfile]\n\n");
    printf("OPTIONS\n");
    printf("\t-h\t\tProgram usage and help.\n");
    printf("\t-v\t\tPrint compression statistics.\n");
    printf("\t-i infile\tInput file to compress.\n");
    printf("\t-o outfile\tOutput of compressed data.\n");
}

void NumUniqueSymbols(Node *CurrentNode, uint16_t *Size) {
    if (CurrentNode->left != NULL) {
        NumUniqueSymbols(CurrentNode->left, Size);
    }
    if (CurrentNode->right != NULL) {
        NumUniqueSymbols(CurrentNode->right, Size);
    }
    if (CurrentNode->symbol != '$') {
        (*Size)++;
    }
    return;
}
