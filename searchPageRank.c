// COMP2521 Assignment 2

// Written by: Eoin Redoblado z5420312
// Date: 13/11/2022

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readData.h"
#include "List.h"

#define MAX_LINE_LENGTH 1000
#define MAX_URL_SIZE 1000

static List updateOccurences(List listPageData, char *searchTerm);
static void doAddOccurence(List listPageData, char *url);

int main(int argc, char *argv[]) {
	// Get each url page's pageRank data from pageRank.txt
	List listPageData = getPageData();

	// Update list with occurences in invertedIndex.txt
	for (int i = 1; i < argc; i++) {
		listPageData = updateOccurences(listPageData, argv[i]);
	}

	// BubbleSort based on occurence, pageRank, then alphabetical order
	ListSortOccurence(listPageData);

	// Print out results
	ListPrintOccurence(listPageData);
}

static List updateOccurences(List listPageData, char *searchTerm) {
	FILE *fp = fopen("invertedIndex.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "Can't open invertedIndex.txt\n");
		exit(EXIT_FAILURE);
	}

	int searchTermLen = strlen(searchTerm);

	// Initialise a buffer to read each line of invertedIndex.txt
	char buffer[MAX_LINE_LENGTH];
	char *buffCheck = calloc(searchTermLen + 1, sizeof(char));

	// Create a string 'roadBlock' to stop reading urls past the '\n'
	char roadBlock[MAX_LINE_LENGTH];

	char url[MAX_URL_SIZE];

	// Read each line of invertedIndex.txt into the buffer
	while (fgets(buffer, MAX_LINE_LENGTH, fp) != NULL) {
		// Copy searchTermLen + 1 bytes from the buffer into buffCheck
		// We use buffCheck to see if the first word of the line matches the 
		// search term.
		strncpy(buffCheck, buffer, searchTermLen + 1);

		// Check if the first part of the buffer matches the search term
		// Do secondary check that immediately after search term is a " " char
		// -> avoids validating "hope" from "hopeful"
		if (strncmp(buffer, searchTerm, searchTermLen) == 0 && 
			buffCheck[searchTermLen] == ' ') {

			// Know when to stop reading from line -> where roadBlock comes in
			if (fscanf(fp, "%s ", roadBlock) == EOF) {
				memset(roadBlock, 0, MAX_LINE_LENGTH);
			}

			// Move file pointer back to front of search term to read rest of urls
			fseek(fp, -(strlen(buffer) + strlen(roadBlock) - searchTermLen), SEEK_CUR);

			// Scan urls until roadblock
			while (fscanf(fp, "%s ", url) != EOF && strcmp(url, roadBlock) != 0) {
				doAddOccurence(listPageData, url);
			}

			break;
		}
	}
	free(buffCheck);
	return listPageData;
}

// Search for url in listPageData and adds an occurence
static void doAddOccurence(List listPageData, char *url) {
	ListIterator iterPageData = ListItNew(listPageData);
	for (int i = 0; i < ListSize(listPageData); i++) {
		Node urlData = ListGetNode(iterPageData);
		if (strcmp(url, ListGetString(urlData)) == 0) {
			ListAddOccurence(urlData);
			break;
		}
		ListItNext(iterPageData);
	}
	ListItFree(iterPageData);
}