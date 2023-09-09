#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

Graph *newGraph() {

    Graph *new = (Graph *) malloc(sizeof(Graph));
    new->count = 0;

    return new;

}

void newVertex(Graph *g, int state) {

    Vertex *new = (Vertex *) malloc(sizeof(Vertex));
    new->state = state;
    new->count = 0;
    new->index = g->count;

    g->vertices[g->count++] = new;

}

// src = source
// dst = destination
void makeNeighbours(Graph *g, int src, int dst) {

    Vertex *vsrc = g->vertices[src];
    Vertex *vdst = g->vertices[dst];

    vsrc->neighbours[vsrc->count++] = vdst;
    vdst->neighbours[vdst->count++] = vsrc;

}

void freeGraph(Graph *g) {

    for (int i = 0; i < g->count; i++)
        free(g->vertices[i]);

    free(g);

}

void printGraph(Graph *g) {

    for (int i = 0; i < g->count; i++)
        printf("Estado (%d) = %d\n", i, g->vertices[i]->state);

}

void printAdj(Graph *g) {

    for (int i = 0; i < g->count; i++) {

        Vertex *v = g->vertices[i];
        printf("Vertice %d: {", v->index);

        for (int j = 0 ; j < v->count; j++)
            printf("%d, ", v->neighbours[j]->index);

        printf("\b\b}\n");

    }

}