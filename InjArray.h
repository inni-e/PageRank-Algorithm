// COMP2521 Assignment 2
// Eoin Redoblado z5420312
// Header file for the Injection Array ADT
// Invented by Eoin Redoblado for COMP2521 Ass2 Task 3 

#ifndef INJARRAY_H
#define INJARRAY_H

#include <stdbool.h>

#include "ArrList.h"

typedef struct injArray *InjArray;

// Creates an InjArray with size
InjArray InjNew(int size);

// FOR MARKER
// For a more visual understanding of what the InjArray takes in, be free to 
// uncomment this function. This will print out what the InjArray took in
/*
void InjPrint(InjArray IA, int size);
*/

// Frees injection array
void InjFree(InjArray injArray, int size);

// Fills injArray determined by a List of ranges
InjArray InjFill(InjArray injArray, List listRanges);

// Returns an array of all allowable ranking permutations based on the InjArray
ArrList InjFindPerms(InjArray IA, int size);

#endif

/** Task3 implementation explanation
 * This ADT works on the hypothesis that the best ranking position for a url 'c' will be in 
 * between the highest and lowest read ranking position of 'c' in all rank.txt files. We will
 * call this a url's RANGE. Permutations will only be considered if all urls in the 
 * permutation are within their calculated RANGE.
 * 
 * We also add extra restrictions to further decrease the number of permutations:
 * - If the RANGE of a url is 1, it is locked in that ranking position, no other urls will
 * 	 be permutated through this position. 
 * ---> Explanation: If the RANGE of a url is 1, this would mean no other urls in the rank.txt
 * 		files occupy the url's ranking position. We can lock this position to be this
 * 		specific url.
 * 
 * - If there are any duplicate urls in the permuations, skip the permutation
 * ---> Explanation: This permutation is invalid as we must have all urls.
 * 
 * There is a worst case time complexity of O(n!) given that there are greater than n + 1 files for 
 * n urls, and there exists a file with a url 'c' in the highest position, whilst being in the
 * lowest position in another file, and this is true for all urls 'c' in the set of all urls.
 * -> This would mean the range of all urls is n, which would lead to an O(n!) time complexity.
 */ 


/** ADT explanation
 * The ADT utilises a 3D array, which functionally acts more as a 2D array with structs with
 * 2 keys. Let's consider this as a 2D array which is easier to explain.
 * -> The rows of the 2D array indicate the ranking positions of a possible permutation
 * -> The columns indicate valid urls to permutate through for each ranking position.
 * 
 * Based on a list of url RANGES 'listRanges', the ADT will fill out the 2D array with valid urls
 * associated to the RANGES of each of the urls. 
 * For example: let's say we have urls A, B and C with respective RANGES as 1st-1st, 1st-3rd and
 * 1st-2nd. 
 * -> Row 1 will be filled with A B C
 * -> Row 2 will be filled with B C
 * -> Row 3 will be filled with B
 * ---> Since the range of A is 1, we lock this array:
 * -> Row 1 is now filled with  A ARRAY_LOCKED (where ARRAY_LOCKED is a negative constant)
 * 
 * The ADT will now output ranking permutations, iterating through the 2D array like an abacus.
 * For all valid permutations, the footrule is calculated and stored with the permutation.
 */ 
