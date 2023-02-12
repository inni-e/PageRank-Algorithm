// COMP2521 Assignment 2
// Eoin Redoblado (z5420312)
// Interface to the String List ADT
// Code modified from lab05
// https://cgi.cse.unsw.edu.au/~cs2521/22T3/labs/week05/files/List.h

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct node *Node;
typedef struct list *List;

// Creates a new empty list
// Complexity: O(1)
List ListNew(void);

// Frees all memory allocated for the given list
// Complexity: O(n)
void ListFree(List l);

// Adds a string to the end of the list. Makes a copy of the string
// before adding it to the list.
// Complexity: O(1)
void ListAppend(List l, char *s, double pageRank, double outDegree);

// Returns the number of items in the list
// Complexity: O(1)
int  ListSize(List l);

// Prints the list, one string per line
// If the strings themselves contain newlines, too bad
void ListPrintPageRank(List l);

// Prints the list's urls
// If occurences are 0, omit them
void ListPrintOccurence(List l);

// Bubble sorts the list, 
// Compares by pageRank, then url
void ListSortPageRank(List l);

// Bubble sorts list
// Compares occurence, pageRank then url 
void ListSortOccurence(List l);

////////////////////////////////////////////////////////////////////////

typedef struct listIterator *ListIterator;

// Creates an iterator for the given list
// Complexity: O(1)
ListIterator ListItNew(List l);

// Moves iterator to next Node in the list. The item should not be modified.
// Complexity: O(1)
ListIterator ListItNext(ListIterator it);

// Gets node from iterator
Node ListGetNode(ListIterator it);

// Gets the string contained within the node
char *ListGetString(Node n);

// Gets the double contained within the node
double ListGetPageRank(Node n);

// Edits the double contained within the node, returns old double
double ListEditPageRank(Node n, double newPageRank);

// Gets the outDegree of a node
double ListGetOutdegree (Node n);

// Edits the outdegree of a node
double ListEditOutdegree (Node n, double newOutdegree);

// Adds an occurence to a node, returns updated occurences
int ListAddOccurence(Node n);

// Gets the loPos of a node
int ListGetLoPos(Node n);

// Edits the loPos of a node
int ListEditLoPos(Node n, int newLoPos);

// Gets the hiPos of a node
int ListGetHiPos(Node n);

// Edits the hiPos of a node
int ListEditHiPos(Node n, int newHiPos);

// Checks if the list has a next item
// Complexity: O(1)
bool ListItHasNext(ListIterator it);

// Checks if the string exists
bool ListStringExists(List l, char *s);

// Frees the given iterator
// Complexity: O(1)
void ListItFree(ListIterator it);

/////////////////////////////////////////////////////////////////////////

#endif