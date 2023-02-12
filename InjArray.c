// COMP2521 Assignment 2
// Eoin Redoblado z5420312
// Implementation file for the Injection Array ADT
// Invented by Eoin Redoblado for COMP2521 Ass2 Task 3 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"
#include "InjArray.h"
#include "ArrList.h"

#define MAX_URL_SIZE 100
#define ARRAY_LOCKED -5
#define EMPTY -1
#define MARKER_SIZE 2
#define DATA 0
#define MARKER 1
#define UNMARKED 0
#define MARKED 1
#define IA_READABLE 1

struct injArray {
	// content[i][j][k]
	// i is the row
	// j is the column
	// k = 0 indicates cell is in DATA mode
	// k = 1 is in MARKER mode -> important for tracking iterations
	int ***content;
};

static bool moveMarkers(int i, InjArray IA, int size);
static double *readMarkers(InjArray IA, double *possibleRanks, int size);
static bool checkDuplicates(double *array, int size);


// Creates an InjArray of height and width 'size' and 
// depth 2 for DATA and MARKER indicators
InjArray InjNew(int size) {
	InjArray newIA = malloc(sizeof(*newIA));
	// Malloc a 2D array of url indexes
	newIA->content = calloc(size, sizeof(**(newIA->content)));
	for (int i = 0; i < size; i++) {
		newIA->content[i] = calloc(size, sizeof(*(newIA->content[i])));
		for (int j = 0; j < size; j++) {
			newIA->content[i][j] = calloc(MARKER_SIZE, sizeof(newIA->content[i][j])); 
			newIA->content[i][j][DATA] = EMPTY;
			newIA->content[i][j][MARKER] = UNMARKED;
		}
	}
	return newIA;
}

// Read InjArray.h for information about this commented function
/*
void InjPrint(InjArray IA, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			printf("- %d -", IA->content[i][j][DATA]);
		}
		printf("\n");
	}
}
*/


// Frees injection array
void InjFree(InjArray IA, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			free(IA->content[i][j]);
		}
		free(IA->content[i]);
	}
	free(IA->content);
	free(IA);
}

// Fills injArray determined by a List of ranges
InjArray InjFill(InjArray injArray, List listRanges) {

	// Iterate through list
	ListIterator iterRanges = ListItNew(listRanges);
	int urlIndex = 0;
	while (ListItHasNext(iterRanges)) {

		// Iterate through possible ranking positions

		// Get some data from current list node
		Node nodeUrl = ListGetNode(iterRanges);

		// Add to ranking position arrays
		// If range of url is 1, delete rest of entries and lock adding to array
		if (ListGetLoPos(nodeUrl) - ListGetHiPos(nodeUrl) == 0) {
			// Lock position with urlIndex if url range is 1
			// Make first index of ranking position array the locked url
			int lockPos = ListGetLoPos(nodeUrl) - 1;
			injArray->content[lockPos][0][DATA] = urlIndex;

			// Afterwards lock the rest of the array away with ARRAY_LOCKED
			injArray->content[lockPos][1][DATA] = ARRAY_LOCKED;
		}

		// else add to end of ranking position arrays
		for (int i = ListGetLoPos(nodeUrl) - 1; i < ListGetHiPos(nodeUrl); i++) {
			for (int j = 0; j < ListSize(listRanges); j++) {
				if (injArray->content[i][j][DATA] == ARRAY_LOCKED) {
					// If array is locked away due to a range-1 url locking position
					break;
				} else if (injArray->content[i][j][DATA] != EMPTY) {
					// If spot in ranking array isn't empty
					continue;
				} else {
					// If spot is empty copy urlIndex in
					injArray->content[i][j][DATA] = urlIndex;
					break;
				}
			}
		}
		ListItNext(iterRanges);
		urlIndex++;
	}
	ListItFree(iterRanges);

	return injArray;
}

// Returns an array of all allowable ranking permutations based on the InjArray
ArrList InjFindPerms(InjArray IA, int size) {
	ArrList listPermutations = ArrListNew();

	// Initialise all IA markers to first index
	for (int i = 0; i < size; i++) {
		IA->content[i][0][MARKER] = MARKED;
	}

	// Do while IA is still readable
	while (IA_READABLE) {
		// Read in IA markers into possibleRanks
		double *possibleRanks = calloc(size, sizeof(double));

		possibleRanks = readMarkers(IA, possibleRanks, size);
		if (possibleRanks != NULL) {
			// Add this possible rank permutation to collection
			ArrListAppend(listPermutations, possibleRanks);
		}

		// Iterate MARKERS
		// Attempt to move marker
		int row = 0;
		while (row < size) {
			if (moveMarkers(row, IA, size)) {
				// Marker succesfully moved
				row = 0;
				break;
			} else {
				// move to row below and attempt to move that marker
				row++;
			}
		}

		// Check if all rows were unsuccessful marker moves
		if (row == size) {
			// Finished iterating through all possible perms, exit out of loop
			break;
		}
	}

	return listPermutations;
}

// Tries to push marker forward
// If successful returns true,
// If unsuccessful, moves marker back to first index and returns false
static bool moveMarkers(int i, InjArray IA, int size) {
	// Look for marker
	int j = 0;
	while (IA->content[i][j][MARKER] != 1) {
		j++;
	}
	if (j >= size || IA->content[i][j + 1][DATA] == EMPTY ||
		IA->content[i][j + 1][DATA] == ARRAY_LOCKED) {
		// If unable to push forward, return marker to beginning of array
		// and return false
		IA->content[i][j][MARKER] = UNMARKED;
		IA->content[i][0][MARKER] = MARKED;
		return false;
	}
	// Otherwise push marker forward
	IA->content[i][j][MARKER] = UNMARKED;
	IA->content[i][j + 1][MARKER] = MARKED;
	return true;
}

static double *readMarkers(InjArray IA, double *possibleRanks, int size) {
	// Read marked vals into possibleRanks array
	for (int i = 0; i < size; i++) {
		int j = 0;
		while (IA->content[i][j][MARKER] != MARKED) {
			j++;
		}
		possibleRanks[i] = (double)IA->content[i][j][DATA];
	}

	// Check if there are any duplicate values, if so return NULL
	if (checkDuplicates(possibleRanks, size)) {
		free(possibleRanks);
		return NULL;
	};

	// Otherwise this is a valid permutation, return possibleRanks
	return possibleRanks;
}

// checks if an array has duplicate entries, returns true if duplicates exist,
// false otherwise
static bool checkDuplicates(double *array, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if (array[i] == array[j]) return true;
		}
	}
	return false;
}