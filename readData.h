// COMP2521 Assignment 2
// Eoin Redoblado z5420312
// z5420312
// Interface to the Read Data ADT
// - Some cool info

#ifndef READDATA_H
#define READDATA_H


#include "List.h"
#include "Graph.h"

// PART 1

/**
 * Creates a list of urls to process by reading data from file "collection.txt"
 */
List GetCollection();

/**
 * Creates a graph of the urls
 */ 
Graph GetGraph(List listUrls);

// PART 2

// Reads data from pageRankList and creates an array
List getPageData(void);

// Read in positions from rank.txt files and store in an array of Lists
List *getPositions(int argc, char **argv);

#endif