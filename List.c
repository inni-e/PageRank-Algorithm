// COMP2521 Assignment 2
// Eoin Redoblado (z5420312)
// Interface to the String List ADT
// Code modified from lab05
// https://cgi.cse.unsw.edu.au/~cs2521/22T3/labs/week05/files/List.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"

struct node {
    char  *s;
    double pageRank;
    double outDegree;
    int occurences;
    int loPos;
    int hiPos;
    Node   next;
};

struct list {
    Node   head;
    Node   tail;
    int    size;
};

static Node newNode(char *s, double pageRank, double outDegree);
static char *myStrdup(char *s);
static void doSwap(Node a, Node b);

////////////////////////////////////////////////////////////////////////

// Creates a new empty list
List ListNew(void) {
    List l = malloc(sizeof(*l));
    if (l == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    return l;
}

// Frees all memory allocated for the given list
void ListFree(List l) {
    Node curr = l->head;
    while (curr != NULL) {
        Node temp = curr;
        curr = curr->next;
        free(temp->s);
        free(temp);
    }
    free(l);
}

// Adds a string to the end of the list
void ListAppend(List l, char *s, double pageRank, double outDegree) {
    Node n = newNode(s, pageRank, outDegree);
    if (l->head == NULL) {
        l->head = n;
    } else {
        l->tail->next = n;
    }
    l->tail = n;
    l->size++;
}

// Creates a new node, occurence is set to 0 always
static Node newNode(char *s, double pageRank, double outDegree) {
    Node n = malloc(sizeof(*n));
    if (n == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    n->s = myStrdup(s);
    n->pageRank = pageRank;
    n->outDegree = outDegree;
    n->occurences = 0;
    n->next = NULL;
    n->loPos = -1;
    n->hiPos = -1;
    return n;
}

static char *myStrdup(char *s) {
    char *copy = malloc((strlen(s) + 1) * sizeof(char));
    if (copy == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    return strcpy(copy, s);
}

// Returns the number of items in the list
int  ListSize(List l) {
    return l->size;
}

// Prints the list, containing outDegree and pageRank
void ListPrintPageRank(List l) {
    Node n = l->head;
    while (n != NULL) {
        printf("%s %.0lf %.7lf\n", n->s, n->outDegree, n->pageRank);
        n = n->next;
    }
}

// Prints the list's urls
// If occurences are 0, omit them
void ListPrintOccurence(List l) {
    for (Node n = l->head; n != NULL; n = n->next) {
        if (n->occurences == 0) break;
        printf("%s\n", n->s);
    }
}

// Bubble sort algorithms gotten from Geeksforgeeks.org, modified to suit assignment
// https://www.geeksforgeeks.org/c-program-bubble-sort-linked-list/

// Bubble sorts the list, 
// Compares by pageRank, then url
void ListSortPageRank(List l) {
    if (l == NULL) {
        fprintf(stderr, "error: list does not exist!\n");
        exit(EXIT_FAILURE);
    }

    bool swapped = true;
    Node lower = l->head, highest = NULL;

    while (swapped) {
        swapped = false;
        lower = l->head;
        while (lower->next != highest) {
            if (lower->pageRank < lower->next->pageRank) {
                doSwap(lower, lower->next);
                swapped = true;
            } else if (lower->pageRank == lower->next->pageRank
                       && strcmp(lower->s, lower->next->s) < 0) {
                doSwap(lower, lower->next);
                swapped = true;
            }
            lower = lower->next;
        }
        highest = lower;
    }   
}

// Bubble sorts list
// Compares occurence, pageRank then url 
void ListSortOccurence(List l) {
    if (l == NULL) {
        fprintf(stderr, "error: list does not exist!\n");
        exit(EXIT_FAILURE);
    }

    bool swapped = true;
    Node lower = l->head, highest = NULL;

    while (swapped) {
        swapped = false;
        lower = l->head;
        while (lower->next != highest) {
            if (lower->occurences < lower->next->occurences) {
                doSwap(lower, lower->next);
                swapped = true;
            } else if (lower->occurences == lower->next->occurences 
                       && lower->pageRank < lower->next->pageRank) {
                doSwap(lower, lower->next);
                swapped = true;
            } else if (lower->pageRank == lower->next->pageRank
                       && strcmp(lower->s, lower->next->s) < 0) {
                doSwap(lower, lower->next);
                swapped = true;
            }
            lower = lower->next;
        }
        highest = lower;
    }    
}

static void doSwap(Node a, Node b) {
    double tempPageRank = a->pageRank;
    char *tempStr = a->s;
    int tempOutDeg = a->outDegree;
    int tempOccur = a->occurences;
    a->pageRank = b->pageRank;
    a->s = b->s;
    a->outDegree = b->outDegree;
    a->occurences = b->occurences;
    b->pageRank = tempPageRank;
    b->s = tempStr;
    b->outDegree = tempOutDeg;
    b->occurences = tempOccur;
}


////////////////////////////////////////////////////////////////////////

struct listIterator {
    Node curr;
    List list;
};

// Creates an iterator for the given list
ListIterator ListItNew(List l) {
    ListIterator it = malloc(sizeof(*it));
    if (it == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    it->curr = l->head;
    it->list = l;
    return it;
}

// Gets the next Node in the list. The item should not be modified.
ListIterator ListItNext(ListIterator it) {
    if (it->curr == NULL) {
        fprintf(stderr, "error: no more items in iterator!\n");
        exit(EXIT_FAILURE);
    }

    it->curr = it->curr->next;
    return it;
}

// Gets node from iterator
Node ListGetNode(ListIterator it) {
    return it->curr;
}

// Gets the string contained within the node
char *ListGetString(Node n) {
    if (n == NULL) {
        fprintf(stderr, "error: node is empty! - string\n");
        exit(EXIT_FAILURE);
    }

    return n->s;
}


// Gets the double contained within the node
double ListGetPageRank(Node n) {
    if (n == NULL) {
        fprintf(stderr, "error: node is empty! - pageRank\n");
        exit(EXIT_FAILURE);
    }

    return n->pageRank;
}

// Edits the double contained within the node
double ListEditPageRank(Node n, double newPageRank) {
    if (n == NULL) {
        fprintf(stderr, "error: node is empty! - pageRank\n");
        exit(EXIT_FAILURE);
    }
    double res = n->pageRank;
    n->pageRank = newPageRank;
    return res;
}

// Gets the outdegree
double ListGetOutdegree (Node n) {
    if (n == NULL) {
        fprintf(stderr, "error: node is empty! - pageRank\n");
        exit(EXIT_FAILURE);
    }
    return n->outDegree;
}

// Edits the outdegree of a node
double ListEditOutdegree (Node n, double newOutdegree) {
    if (n == NULL) {
        fprintf(stderr, "error: node is empty! - pageRank\n");
        exit(EXIT_FAILURE);
    }
    double res = n->outDegree;
    n->outDegree = newOutdegree;
    return res;
}

//Adds an occurence to a node, returns updated occurences
int ListAddOccurence(Node n) {
    if (n == NULL) {
        fprintf(stderr, "error: node is empty! - string\n");
        exit(EXIT_FAILURE);
    }

    n->occurences++;
    return n->occurences;
}

// Gets the loPos of a node
int ListGetLoPos(Node n) {
    if (n == NULL) {
        fprintf(stderr, "error: node is empty! - pageRank\n");
        exit(EXIT_FAILURE);
    }
    return n->loPos;
}

int ListEditLoPos(Node n, int newLoPos) {
    if (n == NULL) {
        fprintf(stderr, "error: node is empty! - pageRank\n");
        exit(EXIT_FAILURE);
    }
    n->loPos = newLoPos;
    return n->loPos;
}

int ListGetHiPos(Node n) {
    if (n == NULL) {
        fprintf(stderr, "error: node is empty! - pageRank\n");
        exit(EXIT_FAILURE);
    }
    return n->hiPos;
}

int ListEditHiPos(Node n, int newHiPos) {
    if (n == NULL) {
        fprintf(stderr, "error: node is empty! - pageRank\n");
        exit(EXIT_FAILURE);
    }
    n->hiPos = newHiPos;
    return n->hiPos;
}

// Checks if the list has a next item
bool ListItHasNext(ListIterator it) {
    return it->curr != NULL;
}

// Checks if string exists in the List already
bool ListStringExists(List l, char *s) {
    // Check if string is empty
    if (l->head == NULL) {
        return false;
    }

    Node curr = l->head;

    // iterate through string to check
    while (curr != NULL) {
        if (strcmp(curr->s, s) == 0) {
            return true;
        }
        curr = curr->next;
    }

    // If similar string is not found, return false
    return false;
}

// Frees the given iterator
void ListItFree(ListIterator it) {
    free(it);
}

