#include "graph.h"
#include "path.h"
#include "vertices.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "hvui:o:"

long long int path_peek(Path *p);
void PrintHelpPage();

int main(int NumberOfArgs, char *Strings[]) {
    //TAKE IN USER INPUT
    int opt = 0;
    int startvertex = 0, endvertex = 0, edgeweight = 0;
    uint32_t vertices = 0;
    FILE *inputfile = stdin, *outputfile = stdout;
    bool verbose = false, undirected = false;
    char **VertexNamesArray;
    while ((opt = getopt(NumberOfArgs, Strings, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': //Prints out a help message
            PrintHelpPage();
            return 0;
        case 'v': //Enables verbose printing. If enabled, the program prints out all Hamiltonian paths found as well as the total number of recursive calls to dfs().
            verbose = true;
            break;
        case 'u': //Specifies the graph to be undirected.
            undirected = true;
            break;
        case 'i': //Specify the input file path containing the cities and edges of a graph. If not specified, the default input should be set as stdin.
            inputfile = fopen(optarg, "r");
            if (inputfile == NULL) {
                fprintf(stdout, "Error: failed to open infile");
                return 0;
            }
            break;
        case 'o': //Specify the output file path to print to. If not specified, the default output should be set as stdout.
            outputfile = fopen(optarg, "w");
            if (outputfile == NULL) {
                fprintf(stdout, "Error: failed to open outfile");
                return 0;
            }
            return 0;
        }
    }
    //SCAN FILE FOR INPUT
    fscanf(inputfile, "%d\n", &vertices);
    Graph *G = graph_create(vertices, undirected); //create the graph here
    VertexNamesArray = (char **) calloc(vertices, sizeof(char *));
    for (uint32_t i = 0; i < vertices; i++) {
        VertexNamesArray[i] = (char *) calloc(100,
            sizeof(char)); //since vertexnamesarray is a 2d array, have to allocate rows and columns
        fgets(VertexNamesArray[i], 100,
            inputfile); //Inserts the names of the vertices into an array of strings
    }

    while (fscanf(inputfile, "%i %i %i", &startvertex, &endvertex, &edgeweight)
           == 3) //loop to insert the vertices edges and weights into the graph until end of file
    {
        if (!graph_add_edge(G, startvertex, endvertex, edgeweight)) {
            fprintf(outputfile, "Warning: Invalid number entered into the graph, exiting "
                                "program\n"); //error checking
            return 0;
        }
        if (undirected) //if the graph is undirected, mirror the edge
        {
            if (!graph_add_edge(G, endvertex, startvertex, edgeweight)) {
                fprintf(outputfile,
                    "Warning: Invalid number entered into the graph, exiting program\n");
                return 0;
            }
        }
    }
    //BEGIN DFS STUFFS
    //Create the two paths
    Path *CurrentPath = path_create();
    Path *ShortestPath = path_create();
    //THIS IS WHERE DFS WOULD GO IF I COULD EVER GET IT TO FUNCTION PROPERLY
    path_print(ShortestPath, outputfile,
        VertexNamesArray); //prints the shortest length even if it is verbose and already printed
    //Close and free everything
    graph_delete(&G);
    path_delete(&CurrentPath);
    path_delete(&ShortestPath);
    for (uint32_t i = 0; i < vertices; i++) {
        free(VertexNamesArray[i]);
    }
    free(VertexNamesArray);
    fclose(inputfile); //closing the files
    fclose(outputfile);
    return 0;
}

void PrintHelpPage() {
    printf("SYNOPSIS\n");
    printf("\tTraveling Salesman Problem using DFS.\n\n");
    printf("USAGE\n");
    printf("\t./tsp [-u] [-v] [-h] [-i infile] [-o outfile]\n\n");
    printf("OPTIONS\n");
    printf("\t-u\t\tUse undirected graph.\n");
    printf("\t-v\t\tEnable verbose printing.\n");
    printf("\t-h\t\tProgram usage and help.\n");
    printf("\t-i infile\tInput containing graph (default: stdin)\n");
    printf("\t-o outfile\tOutput of computed path (default: stdout)\n");
    return;
}
