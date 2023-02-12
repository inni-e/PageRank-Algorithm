// COMP2521 Assignment 2
// Eoin Redoblado z5420312
// Interface to the Read Data ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readData.h"

#define MAX_URL_SIZE 100

////////////////////////////////////////////////////////////////////////

// Gets list of urls from collection.txt
List GetCollection() {
	FILE *fp = fopen("collection.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "Can't open collection.txt");
        exit(EXIT_FAILURE);
    }

    List listOfUrls = ListNew();
    char url[MAX_URL_SIZE];

    while (fscanf(fp, "%s ", url) != EOF) {
		ListAppend(listOfUrls, url, 0.0, 0);
	}

	fclose(fp);
	return listOfUrls;
}

// Creates a graph of the urls O(n^3) 
Graph GetGraph(List listUrls) {
	int numUrls = ListSize(listUrls);
	Graph graphUrls = GraphNew(numUrls);

	ListIterator nodeUrlIterator = ListItNew(listUrls);
	char nodeUrl[MAX_URL_SIZE];
	int nodeUrlNum = 0;
	char linkUrl[MAX_URL_SIZE];
	int linkUrlNum = 0;
	while (ListItHasNext(nodeUrlIterator) == true) {
		// Copy list item into nodeUrl for use
		memset(nodeUrl, 0, strlen(nodeUrl));
		strncpy(nodeUrl, ListGetString(ListGetNode(nodeUrlIterator)), MAX_URL_SIZE);
		strcat(nodeUrl, ".txt");

		FILE *fp = fopen(nodeUrl, "r");
		if (fp == NULL) {
			fprintf(stderr, "Can't open %s\n", nodeUrl);
			exit(EXIT_FAILURE);
		}

		// Skip thru first #start and Section-1
		fscanf(fp, "%s ", linkUrl);
		fscanf(fp, "%s ", linkUrl);

		// Read linkUrls from nodeUrl.txt file
		fscanf(fp, "%s ", linkUrl);
		while (strcmp(linkUrl, "#end") != 0) {

			// Find index of linkUrl in listUrls -> assumes it exists
			ListIterator linkUrlIterator = ListItNew(listUrls);
			linkUrlNum = 0;
			while (strcmp(ListGetString(ListGetNode(linkUrlIterator)), linkUrl) != 0) {
				linkUrlNum++;
				// Move linkUrl iterator down
				ListItNext(linkUrlIterator);
			}
			ListItFree(linkUrlIterator); // Not too sure where to place this Free

			// Check if self-link (not allowed)
			if (nodeUrlNum == linkUrlNum) {
				fscanf(fp, "%s ", linkUrl);
				continue;
			}

			// Add edge in graph
			Edge edgeUrl;
			edgeUrl.v = nodeUrlNum;
			edgeUrl.w = linkUrlNum;
			GraphInsertEdge(graphUrls, edgeUrl);

			// Read next linkUrl
			fscanf(fp, "%s ", linkUrl);
		}

		fclose(fp);
		// Move nodeUrl iterator down
		nodeUrlNum++;
		ListItNext(nodeUrlIterator);
	}
	ListItFree(nodeUrlIterator);

	return graphUrls;
}

// PART 2

// Gets data from pageRankList.txt
List getPageData(void) {
	FILE *fp = fopen("pageRankList.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "Can't open pageRankList.txt\n");
		exit(EXIT_FAILURE);
	}

	char url[MAX_URL_SIZE];
	int outDegree;
	double pageRank;

	List listPageData = ListNew();

	while (fscanf(fp, "%s %d %lf ", url, &outDegree, &pageRank) == 3) {
		ListAppend(listPageData, url, pageRank, 0);
	}

	fclose(fp);

	return listPageData;
}

// PART 3

List *getPositions(int argc, char **argv) {
	// Malloc space for an array of Lists, init list for each index
	List *listPositions = calloc(argc - 1, sizeof(List));
	for (int numFile = 0; numFile < argc - 1; numFile++) {
		listPositions[numFile] = ListNew();
	}

	// Read in rank.txt files into listPositions
	for (int i = 1; i < argc; i++) {
		FILE *fp = fopen(argv[i], "r");
		if (fp == NULL) {
			fprintf(stderr, "Can't open %s\n", argv[i]);
			exit(EXIT_FAILURE);
		}

		char url[MAX_URL_SIZE];

		while (fscanf(fp, "%s ", url) != EOF) {
			ListAppend(listPositions[i-1], url, 0.0, 0.0);
		}

		fclose(fp);
	}

	return listPositions;
}