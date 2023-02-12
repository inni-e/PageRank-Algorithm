// COMP2521 Assignment 2
// Eoin Redoblado z5420312
// Implementation of the Directed Graph ADT
// Uses an adjacency matrix
// Code adapted from lab08
// https://cgi.cse.unsw.edu.au/~cs2521/22T3/labs/week08/files/Graph.c

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"

struct graph {
    int nV;         // #vertices
    int nE;         // #edges
    bool **edges;   // adjacency matrix storing true if adjacent
                    // false if nodes not adjacent
};

static int  validVertex(Graph g, Vertex v);

////////////////////////////////////////////////////////////////////////

// Creates a new instance of a graph
Graph GraphNew(int nV) {
    assert(nV > 0);

    Graph g = malloc(sizeof(*g));
    if (g == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    g->nV = nV;
    g->nE = 0;

    g->edges = malloc(nV * sizeof(int *));
    if (g->edges == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nV; i++) {
        g->edges[i] = calloc(nV, sizeof(int));
        if (g->edges[i] == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }
    }

    return g;
}

// Frees all memory associated with the given graph
void GraphFree(Graph g) {
    for (int i = 0; i < g->nV; i++) {
        free(g->edges[i]);
    }
    free(g->edges);
    free(g);
}

////////////////////////////////////////////////////////////////////////

// Returns the number of vertices in the graph
int GraphNumVertices(Graph g) {
    return g->nV;
}

// Inserts  an  edge into a graph. Does nothing if there is already an
// edge between `e.v` and `e.w`. Returns true if successful, and false
// if there was already an edge.
bool GraphInsertEdge(Graph g, Edge e) {
    assert(validVertex(g, e.v));
    assert(validVertex(g, e.w));
    assert(e.v != e.w);

    if (g->edges[e.v][e.w] == false) {
        g->edges[e.v][e.w] = true;
        g->nE++;
        return true;
    } else {
        return false;
    }
}

// Removes an edge from a graph. Returns true if successful, and false
// if the edge did not exist.
bool GraphRemoveEdge(Graph g, Vertex v, Vertex w) {
    assert(validVertex(g, v));
    assert(validVertex(g, w));

    if (g->edges[v][w] != false) {
        g->edges[v][w] = false;
        g->nE--;
        return true;
    } else {
        return false;
    }
}

// Returns true if an edge exists in the graph, false otherwise
bool GraphIsAdjacent(Graph g, Vertex v, Vertex w) {
    assert(validVertex(g, v));
    assert(validVertex(g, w));
    
    return g->edges[v][w];
}

// Displays information about the graph
void GraphShow(Graph g) {
    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n", g->nE);
    for (int v = 0; v < g->nV; v++) {
        for (int w = 0; w < g->nV; w++) {
            if (g->edges[v][w] != false) {
                printf("Edge %d - %d: %d\n", v, w, g->edges[v][w]);
            }
        }
    }
}

// Checks if vertex is within graph parameters
static int validVertex(Graph g, Vertex v) {
    return v >= 0 && v < g->nV;
}

// Gets the num of outlinks for a vertex
double GraphGetOutlinks(Graph g, int v) {
    double numOutlinks = 0;
    for (int i = 0; i < g->nV; i++) {
        if (g->edges[v][i] == true) {
            numOutlinks++;
        }
    }
    return numOutlinks;
}

// Gets the num of inlinks for a vertex
double GraphGetInlinks(Graph g, int w) {
    double numInlinks = 0;
    for (int i = 0; i < g->nV; i++) {
        if (g->edges[i][w] == true) {
            numInlinks++;
        }
    }
    return numInlinks;
}