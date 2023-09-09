#ifndef GRAPH_H
#define GRAPH_H

#define N 150

typedef struct Vertex {

    int index, state, count;
    struct Vertex *neighbours[N];

} Vertex;

typedef struct Graph {

    int count;
    Vertex *vertices[N];

} Graph;

Graph *newGraph();
void newVertex(Graph *g, int state);
void makeNeighbours(Graph *g, int src, int dst);
void freeGraph(Graph *g);
void printGraph(Graph *g);
void printAdj(Graph *g);

#endif