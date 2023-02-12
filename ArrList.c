// COMP2521 Assignment 2
// Eoin Redoblado z5420312
// Interface to the Double Array List ADT
// Code modified from lab05
// https://cgi.cse.unsw.edu.au/~cs2521/22T3/labs/week05/files/List.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ArrList.h"

struct arrNode {
    double *doubleArray; // Will contain url indexes for Task3
    double footrule; // Contains calculated footrule from Task3
    ArrNode   next;
};

struct arrList {
    ArrNode   head;
    ArrNode   tail;
    int    size;
};

#define MAX_FOOTRULE 999.999

static ArrNode newNode(double *doubleArray);

// Creates a new array list
ArrList ArrListNew(void) {
	ArrList al = malloc(sizeof(*al));
	if (al == NULL) {
		fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
	}

	al->head = NULL;
	al->tail = NULL;
	al->size = 0;
	return al;
}

// Frees all memory allocated for the given list
void ArrListFree(ArrList al) {
    ArrNode curr = al->head;
    while (curr != NULL) {
        ArrNode temp = curr;
        curr = curr->next;
        free(temp->doubleArray);
        free(temp);
    }
    free(al);
}

// Adds a string to the end of the list
void ArrListAppend(ArrList al, double *doubleArray) {
    ArrNode n = newNode(doubleArray);
    if (al->head == NULL) {
        al->head = n;
    } else {
        al->tail->next = n;
    }
    al->tail = n;
    al->size++;
}

// Creates a new node, footrule is set to -1.0
static ArrNode newNode(double *doubleArray) {
    ArrNode n = malloc(sizeof(*n));
    if (n == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    n->doubleArray = doubleArray;
    n->footrule = -1.0;
    n->next = NULL; 
    return n;
}

// Returns num of items in list
int ArrListSize(ArrList al) {
	return al->size;
}

// Prints the best ranking permutation of all the lists
void ArrListPrintBest(ArrList al, int size, char **arrayUrls) {
	// Find node with the lowest footrule
	int index = 0;
	double minFootrule = MAX_FOOTRULE;
	int bestIndex = -1;
	ArrNode n = al->head;
    while (n != NULL) {
    	if (n->footrule < minFootrule) {
    		minFootrule = n->footrule;
    		bestIndex = index;
    	}
        n = n->next;
        index++;
    }

    // Travel to best node
    n = al->head;
    for (int i = 0; i < bestIndex; i++) {
    	n = n->next;
    }

    // Print best node
    printf("%.7lf\n", n->footrule);
    for (int j = 0; j < size; j++) {
    	int urlIndex = (int)n->doubleArray[j];
        printf("%s\n", arrayUrls[urlIndex]);
    }
}

////////////////////////////////////////////////////////////////////////

struct arrListIterator {
    ArrNode curr;
    ArrList list;
};

// Creates an iterator for the given list
ArrListIterator ArrListItNew(ArrList al) {
    ArrListIterator it = malloc(sizeof(*it));
    if (it == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    it->curr = al->head;
    it->list = al;
    return it;
}

// Gets the next Node in the list. The item should not be modified.
ArrListIterator ArrListItNext(ArrListIterator it) {
    if (it->curr == NULL) {
        fprintf(stderr, "error: no more items in iterator!\n");
        exit(EXIT_FAILURE);
    }

    it->curr = it->curr->next;
    return it;
}

// Gets node from iterator
ArrNode ArrListGetNode(ArrListIterator it) {
    return it->curr;
}

// Gets the double contained within the node
double ArrListGetFootrule(ArrNode n) {
    if (n == NULL) {
        fprintf(stderr, "error: node is empty! - pageRank\n");
        exit(EXIT_FAILURE);
    }

    return n->footrule;
}

// Edits the double contained within the node
// Returns old double val
double ArrListEditFootrule(ArrNode n, double newFootrule) {
    if (n == NULL) {
        fprintf(stderr, "error: node is empty! - pageRank\n");
        exit(EXIT_FAILURE);
    }
    double res = n->footrule;
    n->footrule = newFootrule;
    return res;
}

// Gets doubleArray from node
double *ArrListGetArray(ArrNode n) {
	if (n == NULL) {
		fprintf(stderr, "error: node is empty! - pageRank\n");
        exit(EXIT_FAILURE);
	}
	return n->doubleArray;
}

// Checks if the list has a next item
bool ArrListItHasNext(ArrListIterator it) {
    return it->curr != NULL;
}

// Frees the given iterator
void ArrListItFree(ArrListIterator it) {
    free(it);
}
