// COMP2521 Assignment 2
// Eoin Redoblado z5420312
// Interface to the Directed Graph ADT
// Code adapted from lab08
// https://cgi.cse.unsw.edu.au/~cs2521/22T3/labs/week08/files/Graph.h
// - Vertices are identified by integers between 0 and nV - 1,
//   where nV is the number of vertices in the graph
// - Self-loops are not allowed
// - Duplicate loops are not allowed

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

typedef struct graph *Graph;

typedef int Vertex;

// Edges are pairs of vertices (end-points)
typedef struct Edge {
    Vertex v;
    Vertex w;
} Edge;

/**
 * Creates a new instance of a graph
 */
Graph  GraphNew(int nV);

/**
 * Frees all memory associated with the given graph
 */
void   GraphFree(Graph g);

/**
 * Returns the number of vertices in the graph
 */
int    GraphNumVertices(Graph g);

/**
 * Inserts  an  edge into a graph. Does nothing if there is already an
 * edge between `e.v` and `e.w`. Returns true if successful, and false
 * if there was already an edge.
  */
bool   GraphInsertEdge(Graph g, Edge e);

/**
 * Removes an edge from a graph. Returns true if successful, and false
 * if the edge did not exist.
 */
bool   GraphRemoveEdge(Graph g, Vertex v, Vertex w);

/**
 * Returns true if an edge exists in the graph, false otherwise
 */
bool GraphIsAdjacent(Graph g, Vertex v, Vertex w);

/**
 * Displays information about the graph
 */
void   GraphShow(Graph g);

/**
 * Gets the num of outlinks for a vertex
 */ 
double GraphGetOutlinks(Graph g, int v);

/**
 * Gets the num of inlinks for a vertex
 */ 
double GraphGetInlinks(Graph g, int w);

#endif
