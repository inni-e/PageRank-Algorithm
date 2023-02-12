// COMP2521 Assignment 2

// Written by: Eoin Redoblado z5420312
// Date: 16/11/2022

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readData.h"
#include "List.h"
#include "InjArray.h"
#include "ArrList.h"

#define MAX_URL_SIZE 100

static List getRanges(List *listPositions, int numFiles);
static void updateRange(List listUrlRanges, char *url, int position);
static double calcTotalFootrule(List *T, char **arrayUrls, double *C, int k, int n);
static double calcFootrule(List *T, int indexc, double p, int n, 
						   char **arrayUrls, int k);

int main(int argc, char *argv[]) {
	// Create an array of Lists with containing all
	// Url positions
	List *listPositions = getPositions(argc, argv);

	// Create a List containing all the urls and their range of positions
	List listUrlRanges = getRanges(listPositions, argc - 1);

	int numUrls = ListSize(listUrlRanges);

	// Create Injection Array
	InjArray IAUrls = InjNew(numUrls);
	IAUrls = InjFill(IAUrls, listUrlRanges);

	// FOR MARKER -> read InjArray.h for more information on this section
	// InjPrint(IAUrls, numUrls);

	// Create Array of urls for quick lookup / reference -> char **arrayUrls
	char **arrayUrls = calloc(numUrls, sizeof(*arrayUrls));
	ListIterator iterUrls = ListItNew(listUrlRanges);
	for (int i = 0; i < numUrls; i++) {
		Node nodeUrl = ListGetNode(iterUrls);
		arrayUrls[i] = calloc(strlen(ListGetString(nodeUrl)) + 1, sizeof(char));
		strcpy(arrayUrls[i], ListGetString(nodeUrl));
		ListItNext(iterUrls);
	}
	ListItFree(iterUrls);
	
	// Obtain all valid permutations of Url indexes based on InjArray
	ArrList listPermutations = InjFindPerms(IAUrls, numUrls);

	// Calculate Footrule for each permutation
	ArrListIterator iterPerms = ArrListItNew(listPermutations);
	while (ArrListItHasNext(iterPerms)) {
		ArrNode nodePerm = ArrListGetNode(iterPerms);
		double footrule = calcTotalFootrule(listPositions, 
											arrayUrls, 
											ArrListGetArray(nodePerm),
											argc - 1,
											numUrls);
		ArrListEditFootrule(nodePerm, footrule);
		ArrListItNext(iterPerms);
	}
	ArrListItFree(iterPerms);

	ArrListPrintBest(listPermutations, numUrls, arrayUrls);

	// Free everything
	ArrListFree(listPermutations);
	InjFree(IAUrls, ListSize(listUrlRanges));
	ListFree(listUrlRanges);
	for (int i = 0; i < argc - 1; i++) {
		ListFree(listPositions[i]);
	}
	free(listPositions);
	for (int j = 0; j < numUrls; j++) {
		free(arrayUrls[j]);
	}
	free(arrayUrls);
	
}

static List getRanges(List *listPositions, int numFiles) {
	List listUrlRanges = ListNew();
	char url[MAX_URL_SIZE];

	for (int i = 0; i < numFiles; i++) {

		// Iterate through each file list and update ranges accordingly
		ListIterator iterFile = ListItNew(listPositions[i]);
		int position = 1;
		while (ListItHasNext(iterFile)) {
			Node nodeUrl = ListGetNode(iterFile);
			memset(url, 0, MAX_URL_SIZE);
			strcpy(url, ListGetString(nodeUrl));

			// Check if url already exists in listUrlRanges, if not, add 
			// to listUrlRanges
			if (!ListStringExists(listUrlRanges, url)) {
				ListAppend(listUrlRanges, url, 0.0, 0.0);
			}

			// Update url's range if it exists in listUrlRanges
			updateRange(listUrlRanges, url, position);

			ListItNext(iterFile);
			position++;
		}
		ListItFree(iterFile);
	}

	return listUrlRanges;
}

static void updateRange(List listUrlRanges, char *url, int position) {
	// Find the url in listUrlRanges
	ListIterator iterRange = ListItNew(listUrlRanges);
	Node nodeUrl = ListGetNode(iterRange);
	while (strcmp(ListGetString(nodeUrl), url) != 0) {
		ListItNext(iterRange);
		nodeUrl = ListGetNode(iterRange);
	}
	ListItFree(iterRange);

	// Update nodeUrl->lo/hiPos if position is outside its lo/hiPos
	int loPos = ListGetLoPos(nodeUrl);
	int hiPos = ListGetHiPos(nodeUrl);
	if (loPos == -1 || position < loPos) {
		ListEditLoPos(nodeUrl, position);
	}
	if (hiPos == -1 || position > hiPos) {
		ListEditHiPos(nodeUrl, position);
	}
}

/** Calculates Footrule
 * List *T is the original rankings
 * int *C is the set of pages to be ranked
 * char **arrayUrls is a reference to URL strings for int *C
 */
static double calcTotalFootrule(List *T, 
						   char **arrayUrls, 
						   double *C,
						   int k, int n) {
	// Iterate through all c of C
	double totalSum = 0.0;
	for (int i = 0; i < n; i++) {

		totalSum += calcFootrule(T, C[i], (double)(i + 1), (double)n, arrayUrls, k);		
	}
	return totalSum;
}

static double calcFootrule(List *T, int indexc, double p, int n, 
						   char **arrayUrls, int k) {

	char *url;
	url = arrayUrls[indexc];

	double sum = 0.0;
	// Iterate through all rankings and find sum of all
	for (int i = 0; i < k; i++) {
		// find position of url in original ranking
		double ogPos = 1.0;
		ListIterator iterRanking = ListItNew(T[i]);
		while (ogPos <= (double)ListSize(T[i])) {
			Node nodeUrl = ListGetNode(iterRanking);
			if (strcmp(ListGetString(nodeUrl), url) == 0) {
				break;
			}
			ListItNext(iterRanking);
			ogPos++;
		}
		ListItFree(iterRanking);

		// If url was not found in original ranking, skip to next ranking
		if (ogPos > (double)ListSize(T[i])) {
			continue;
		}

		// Otherwise calculate footrule
		double leftFrac = ogPos / (double)ListSize(T[i]);
		double rightFrac = p / n;
		sum += fabs(leftFrac - rightFrac);
	}
	return sum;
}

