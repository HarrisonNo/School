#include "graph.h"

#include "vertices.h"

#include <stdlib.h>

struct Graph {
    uint32_t vertices;
    bool undirected;
    bool visited[VERTICES];
    int matrix[VERTICES][VERTICES];
};

Graph *graph_create(uint32_t vertices, bool undirected) { //TAKEN FROM LONG'S ASGN4 PDF
    Graph *G = (Graph *) calloc(1, sizeof(Graph));
    G->vertices = vertices;
    G->undirected = undirected;
    for (int i = 0; i < VERTICES; i++) {
        G->visited[i] = false;
    }
    for (int row = 0; row < VERTICES; row++) {
        for (int column = 0; column < VERTICES; column++) {
            G->matrix[row][column] = 0;
        }
    }
    return G;
}

void graph_delete(Graph **G) { //TAKEN FROM LONG'S ASGN4 PDF
    free(*G);
    *G = NULL;
    return;
}

uint32_t graph_vertices(Graph *G) {
    return G->vertices;
}

bool graph_add_edge(Graph *G, uint32_t i, uint32_t j, uint32_t k) {
    if (i >= G->vertices || j >= G->vertices || i < 0 || j < 0 || k < 0) {
        return false;
    }
    G->matrix[i][j] = k;
    if (G->undirected) {
        G->matrix[j][i] = k;
    }
    return true;
}

bool graph_has_edge(Graph *G, uint32_t i, uint32_t j) {
    if (i > VERTICES || j > VERTICES || i < 0 || j < 0) {
        return false;
    }
    if (G->matrix[i][j]) //if this is nonzero
    {
        return true;
    }
    return false;
}

uint32_t graph_edge_weight(Graph *G, uint32_t i, uint32_t j) {
    if (i > VERTICES || j > VERTICES || i < 0 || j < 0) {
        return 0;
    }
    return G->matrix[i][j];
}

bool graph_visited(Graph *G, uint32_t v) {
    if (v > VERTICES || v < 0) {
        return false;
    }
    if (G->visited[v]) //if nonzero then it has been visited
    {
        return true;
    }
    return false;
}

void graph_mark_visited(Graph *G, uint32_t v) {
    if (v > VERTICES || v < 0) {
        return;
    }
    G->visited[v] = 1; //mark visisited
    return;
}

void graph_mark_unvisited(Graph *G, uint32_t v) {
    if (v > VERTICES || v < 0) {
        return;
    }
    G->visited[v] = 0; //mark unvisisited
    return;
}

void graph_print(Graph *G);
