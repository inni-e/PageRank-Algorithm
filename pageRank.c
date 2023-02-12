// COMP2521 Assignment 2

// Written by: Eoin Redoblado z5420312
// Date: 12/11/2022

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readData.h"
#include "List.h"
#include "Graph.h"

List calculatePageRank(Graph g, double d, double diffPR, int maxIterations, List listPageRanks);
static double doWeightSum(Graph g, int url, List listPageRanks, 
                          double *arrayOutlinks, double *arrayInlinks);
static double doW_in(Graph g, int inlink, int url, double *arrayInlinks);
static double doW_out(Graph g, int inlink, int url, double *arrayOutlinks);

int main(int argc, char *argv[]) {
    // argc is the number of command-line arguments, which includes the
    // program name
    // argv is the array of command-line arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s dampingFactor diffPR maxIterations\n",
                argv[0]);
        return EXIT_FAILURE;
    }

    // Converting terminal inputs to numbers
    double d = atof(argv[1]);
    double diffPR = atof(argv[2]);
    int maxIterations = atoi(argv[3]);

    // Create list of urls from collection.txt
    List listUrls = GetCollection();

    // Create graph from list of urls
    Graph graphUrls = GetGraph(listUrls); 

    // Calculate pageRank + sort based on pageRank 
    List listPageRanks = calculatePageRank(graphUrls, d, diffPR, maxIterations, listUrls);
    ListSortPageRank(listPageRanks);

    // Print everything
    ListPrintPageRank(listPageRanks);

    // Free everything
    ListFree(listPageRanks);
    GraphFree(graphUrls);

    return 0;
}

List calculatePageRank(Graph g, double d, double diffPR, int maxIterations, List listPageRanks) {
    int numUrls = GraphNumVertices(g);
    int doubleUrls = (double)numUrls;

    // Create Outlink and Inlink arrays derived from the graph implementation,
    // to use for the Weighted PageRank Algorithm + assign outDegrees to list items
    ListIterator iterNewPageRanks;
    ListIterator iterOldPageRanks = ListItNew(listPageRanks);
    double arrayOutlinks[numUrls];
    double arrayInlinks[numUrls];
    for (int i = 0; i < numUrls; i++) {
        arrayInlinks[i] = GraphGetInlinks(g, i);
        arrayOutlinks[i] = GraphGetOutlinks(g, i);
        Node nodeUrl = ListGetNode(iterOldPageRanks);
        ListEditOutdegree(nodeUrl, arrayOutlinks[i]);
        ListItNext(iterOldPageRanks);
    }
    ListItFree(iterOldPageRanks);

    // Fill listPageRanks with respective pageRanks, pseudocode from Assignment 2 spec
    // First initialise all page ranks to be 1 / numUrls
    iterOldPageRanks = ListItNew(listPageRanks);
    while (ListItHasNext(iterOldPageRanks) == true) {
        ListEditPageRank(ListGetNode(iterOldPageRanks), 1.0 / doubleUrls);
        ListItNext(iterOldPageRanks);
    }
    ListItFree(iterOldPageRanks);

    double diff = diffPR;

    // Begin iterations of PageRank
    // Each iteration create listNewPageRanks based on listPageRanks
    // Every next iteration copy listNewPageRanks to listPageRanks and clear listNewPageRanks
    for (int iteration = 0; iteration < maxIterations && diff >= diffPR; iteration++) {
        List listNewPageRanks = ListNew();

        // Perform for each url in listPageRanks
        iterOldPageRanks = ListItNew(listPageRanks);
        for (int url = 0; url < numUrls; url++) {
            
            // Calculate the summation -> end of Weighted PageRank algo
            double summation = doWeightSum(g, url, listPageRanks, arrayOutlinks, arrayInlinks);

            // Multiply value by damping factor equation
            summation = ((1.0 - d) / doubleUrls) + (d * summation);

            // Append new pageRank val to listNewPageRanks with associated url
            Node nodeOld = ListGetNode(iterOldPageRanks);
            ListAppend(listNewPageRanks, ListGetString(nodeOld), summation, ListGetOutdegree(nodeOld));
            ListItNext(iterOldPageRanks);
        }
        ListItFree(iterOldPageRanks);

        // Calculate new diff
        iterOldPageRanks = ListItNew(listPageRanks);
        iterNewPageRanks = ListItNew(listNewPageRanks);
        double diffsum = 0.0;
        for (int i = 0; i < numUrls; i++) {
            double newPageRank = ListGetPageRank(ListGetNode(iterNewPageRanks)); 
            double oldPageRank = ListGetPageRank(ListGetNode(iterOldPageRanks));
            diffsum += fabs(newPageRank - oldPageRank);
            ListItNext(iterOldPageRanks);
            ListItNext(iterNewPageRanks);
        }
        ListItFree(iterOldPageRanks);
        ListItFree(iterNewPageRanks);
        diff = diffsum;

        // Copy listNewPageRanks to listPageRanks + clear listNewPageRanks
        ListFree(listPageRanks);
        listPageRanks = listNewPageRanks;
        // listNewPageRanks gets re-allocated to a new empty list at the beginning
    }

    return listPageRanks;
}

static double doWeightSum(Graph g, int url, List listPageRanks, 
                          double *arrayOutlinks, double *arrayInlinks) {
    double res = 0.0;
    for (int inlink = 0; inlink < GraphNumVertices(g); inlink++) {

        // check if inlink (p_j) has an outgoing link to url (p_i)
        if (GraphIsAdjacent(g, inlink, url)) {

            // Get old pageRank value of inlink from listPageRanks
            ListIterator iterOldPageRanks = ListItNew(listPageRanks);
            for (int i = 0; i < inlink; i++) {
                ListItNext(iterOldPageRanks);
            }
            Node nodeInlink = ListGetNode(iterOldPageRanks);
            ListItFree(iterOldPageRanks);
            double oldPageRank = ListGetPageRank(nodeInlink);

            // Get W_in and W_out
            double W_in = doW_in(g, inlink, url, arrayInlinks);
            double W_out = doW_out(g, inlink, url, arrayOutlinks);

            res += (oldPageRank * W_in * W_out);
        }
    }

    return res;
}

static double doW_in(Graph g, int inlink, int url, double *arrayInlinks) {
    double denominator = 0.0;
    // Sum up the denominator of the equation with reference page list of inlink (p_j)
    for (int reference = 0; reference < GraphNumVertices(g); reference++) {
        if (GraphIsAdjacent(g, inlink, reference)) {
            double numInlinks = arrayInlinks[reference];
            denominator += numInlinks;
        }
    }

    double numerator = arrayInlinks[url];
    return numerator / denominator;
}

static double doW_out(Graph g, int inlink, int url, double *arrayOutlinks) {
    double denominator = 0.0;

    // Sum up the denominator of the equation with reference page list of inlink (p_j)
    for (int reference = 0; reference < GraphNumVertices(g); reference++) {
        if (GraphIsAdjacent(g, inlink, reference)) {
            double numOutlinks = arrayOutlinks[reference];

            // Check if 0 -> turn to 0.5 as per spec
            if (numOutlinks == 0.0) {
                numOutlinks = 0.5;
            }
            denominator += numOutlinks;
        }
    }

    double numerator = arrayOutlinks[url];
    if (numerator == 0.0) {
        numerator = 0.5;
    }

    return numerator / denominator;
}
