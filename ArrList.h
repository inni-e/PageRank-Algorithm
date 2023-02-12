// COMP2521 Assignment 2
// Eoin Redoblado z5420312
// Interface to the Double Array List ADT
// Code modified from lab05
// https://cgi.cse.unsw.edu.au/~cs2521/22T3/labs/week05/files/List.h

#ifndef ARRLIST_H
#define ARRLIST_H

#include <stdbool.h>

typedef struct arrNode *ArrNode;
typedef struct arrList *ArrList;

// Creates a new array list
ArrList ArrListNew(void);

// Frees all memory allocated for the given list
void ArrListFree(ArrList l);

// Adds a string to the end of the list
void ArrListAppend(ArrList al, double *doubleArray);

// Returns num of items in list
int ArrListSize(ArrList al);

// Prints the best ranking permutation of all the lists
void ArrListPrintBest(ArrList al, int size, char **arrayUrls);

////////////////////////////////////////////////////////////////////////

typedef struct arrListIterator *ArrListIterator;

// Creates an iterator for the given list
ArrListIterator ArrListItNew(ArrList al);

// Gets the next Node in the list. The item should not be modified.
ArrListIterator ArrListItNext(ArrListIterator it);

// Gets node from iterator
ArrNode ArrListGetNode(ArrListIterator it);

// Gets the double contained within the node
double ArrListGetFootrule(ArrNode n);

// Edits the double contained within the node
// Returns old double val
double ArrListEditFootrule(ArrNode n, double newFootrule);

// Gets doubleArray from node
double *ArrListGetArray(ArrNode n);

// Checks if the list has a next item
bool ArrListItHasNext(ArrListIterator it);

// Frees the given iterator
void ArrListItFree(ArrListIterator it);

#endif