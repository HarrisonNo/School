#include "path.h"

#include "stack.h"
#include "vertices.h"

#include <inttypes.h>
#include <stdlib.h>

struct Path {
    Stack *vertices; //The vertices comprising the path
    uint32_t length; //The total length of the path
};

Path *path_create(void) {
    Path *P = (Path *) calloc(1, sizeof(Path));
    P->vertices = stack_create(VERTICES);
    P->length = 0;
    return P;
}

void path_delete(Path **p) {
    stack_delete(&((*p)->vertices));
    free(*p);
    *p = NULL;
    return;
}

bool path_push_vertex(Path *p, uint32_t v, Graph *G) {
    uint32_t x;
    if (stack_empty(p->vertices)) {
        stack_push(p->vertices, v);
        return true;
    } else if (!stack_full(p->vertices)) {
        stack_peek(p->vertices, &x);
        stack_push(p->vertices, v);
        p->length += graph_edge_weight(G, x, v);
        return true;
    } else {
        return false;
    }
}

bool path_pop_vertex(Path *p, uint32_t *v, Graph *G) {
    uint32_t x;
    if (stack_empty(p->vertices)) {
        return false;
    } else if (path_vertices(p) == 1) {
        stack_pop(p->vertices, v);
        return true;
    } else {
        stack_peek(p->vertices, &x);
        stack_pop(p->vertices, v);
        p->length -= graph_edge_weight(G, x, *v);
        return true;
    }
}

uint32_t path_vertices(Path *p) {
    return stack_size(p->vertices);
}

uint32_t path_length(Path *p) {
    return p->length;
}

void path_copy(Path *dst, Path *src) {
    stack_copy(dst->vertices, src->vertices);
    dst->length = src->length;
    return;
}

void path_print(Path *p, FILE *outfile, char *cities[]) {
    fprintf(outfile, "Path length: %" PRIu32 "\n", path_length(p));
    fprintf(outfile, "Path:");
    stack_print(p->vertices, outfile, cities);
}

long long int path_peek(Path *
        p) //Technically a homemade function but I felt that it was rather silly that I couldn't look at the top value of the path
{
    uint32_t *ReturnValue = NULL;
    if (stack_peek(p->vertices, ReturnValue)) //if there is a value to return, return it
    {
        return (long int) *ReturnValue;
    }
    return -1; //path does not have a vertex to peek at
}
