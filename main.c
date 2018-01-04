////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//      main.c                                                                //
//      ------                                                                //
//                                                                            //
// Content: A set of tests for the BinaryTrees library.                       //
// Author:  Carlos Luna-Mota <el.luna@gmail.com>                              //
// Date:    January 2018                                                      //
//                                                                            //
// This is free and unencumbered software released into the public domain.    //
//                                                                            //
// Anyone is free to copy, modify, publish, use, compile, sell, or            //
// distribute this software, either in source code form or as a compiled      //
// binary, for any purpose, commercial or non-commercial, and by any means.   //
//                                                                            //
// In jurisdictions that recognize copyright laws, the author or authors of   //
// this software dedicate any and all copyright interest in the software to   //
// the public domain. We make this dedication for the benefit of the public   //
// at large and to the detriment of our heirs and successors. We intend this  //
// dedication to be an overt act of relinquishment in perpetuity of all       //
// present and future rights to this software under copyright law.            //
//                                                                            //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,            //
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF         //
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.     //
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR          //
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,      //
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR      //
// OTHER DEALINGS IN THE SOFTWARE.                                            //
//                                                                            //
// For more information, please refer to <http://unlicense.org>               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



// LIBRARIES ///////////////////////////////////////////////////////////////////
#include <stdlib.h>             // srand, rand, malloc, free
#include <time.h>               // time, clock
#include <stdio.h>              // printf, fprintf, stderr
#include <assert.h>             // assert
#include "BinaryTrees.h"        // BinaryTrees library headers
////////////////////////////////////////////////////////////////////////////////



// DEFINITIONS: ////////////////////////////////////////////////////////////////

#define FAIL 1
#define PASS 0

// Dummy data holder, just contains an integer key:
typedef struct MyData { int key; } MyData;

// Comparing function:
int MyComp(const void *ptr1, const void *ptr2) {
    MyData *d1 = (MyData *) ptr1;
    MyData *d2 = (MyData *) ptr2; 
    if      (d1->key < d2->key) { return -1; }
    else if (d1->key > d2->key) { return +1; }
    else                        { return  0; }
}

// Printing function: This is, indeed, optional.
void MyPrint(const void *ptr) {
    MyData *d = (MyData *) ptr;
    printf("(%d)", d->key);
}

////////////////////////////////////////////////////////////////////////////////



// TESTS: //////////////////////////////////////////////////////////////////////

// Sequential insertions & complete deletion:
int bs_tree_sequential_test(int max_size) {

    int i;
    bs_tree *tree  = new_bs_tree(MyComp);
    MyData  *data  = NULL;
    MyData  *found = NULL;

    // It is a bs_tree:
    if (is_bs_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (bs_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=0; i<max_size; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = bs_tree_insert(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a bs_tree:
        if (is_bs_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (bs_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=0; i<max_size; i++) {
        data->key = i;
        found = bs_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = bs_tree_min(tree);
    if (found == NULL)   { return FAIL; }
    if (found->key != 0) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = bs_tree_max(tree);
    if (found == NULL)            { return FAIL; }
    if (found->key != max_size-1) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = bs_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    bs_tree_remove_all(tree, free);

    // It is a bs_tree:
    if (is_bs_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (bs_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=max_size; i>0; i--) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = bs_tree_insert(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a bs_tree:
        if (is_bs_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (bs_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=max_size; i>0; i--) {
        data->key = i;
        found = bs_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = bs_tree_min(tree);
    if (found == NULL)   { return FAIL; }
    if (found->key != 1) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = bs_tree_max(tree);
    if (found == NULL)          { return FAIL; }
    if (found->key != max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = bs_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    bs_tree_remove_all(tree, free);

    // It is a bs_tree:
    if (is_bs_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (bs_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=1; i<=max_size; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = bs_tree_insert(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a bs_tree:
        if (is_bs_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (bs_tree_is_empty(tree) == YES) { return FAIL; }

        data = (MyData *) malloc(sizeof(MyData));
        data->key = -i;
        found = bs_tree_insert(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a bs_tree:
        if (is_bs_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (bs_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=1; i<=max_size; i++) {
        data->key = i;
        found = bs_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
        data->key = -i;
        found = bs_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = bs_tree_min(tree);
    if (found == NULL)           { return FAIL; }
    if (found->key != -max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = bs_tree_max(tree);
    if (found == NULL)          { return FAIL; }
    if (found->key != max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = bs_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    bs_tree_remove_all(tree, free);

    // It is a bs_tree:
    if (is_bs_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (bs_tree_is_empty(tree) == NO) { return FAIL; }

    free(tree);
    
    return PASS;
}

// Fast Sequential insertions & deletions:
int bs_tree_fast_sequential_test(int max_size) {

    int i;
    bs_tree *tree  = new_bs_tree(MyComp);
    MyData  *data  = NULL;
    MyData  *found = NULL;

    // It is a bs_tree:
    if (is_bs_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (bs_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=0; i<max_size; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = bs_tree_insert_max(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a bs_tree:
        if (is_bs_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (bs_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=0; i<max_size; i++) {
        data->key = i;
        found = bs_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = bs_tree_min(tree);
    if (found == NULL)   { return FAIL; }
    if (found->key != 0) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = bs_tree_max(tree);
    if (found == NULL)            { return FAIL; }
    if (found->key != max_size-1) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = bs_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    for (i=max_size; i>0; i--) {
        
        found = bs_tree_remove_max(tree);

        // Not enought elements:
        if (found == NULL) { return FAIL; }
        else               { free(found); }

        // It is a bs_tree:
        if (is_bs_tree(tree) == NO) { return FAIL; }
    }
    
    // It is a bs_tree:
    if (is_bs_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (bs_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=max_size; i>0; i--) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = bs_tree_insert_min(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a bs_tree:
        if (is_bs_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (bs_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=max_size; i>0; i--) {
        data->key = i;
        found = bs_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = bs_tree_min(tree);
    if (found == NULL)   { return FAIL; }
    if (found->key != 1) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = bs_tree_max(tree);
    if (found == NULL)          { return FAIL; }
    if (found->key != max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = bs_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    for (i=max_size; i>0; i--) {
        
        found = bs_tree_remove_min(tree);

        // Not enought elements:
        if (found == NULL) { return FAIL; }
        else               { free(found); }

        // It is a bs_tree:
        if (is_bs_tree(tree) == NO) { return FAIL; }
    }
    
    // It is a bs_tree:
    if (is_bs_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (bs_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=1; i<=max_size; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = bs_tree_insert_max(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a bs_tree:
        if (is_bs_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (bs_tree_is_empty(tree) == YES) { return FAIL; }

        data = (MyData *) malloc(sizeof(MyData));
        data->key = -i;
        found = bs_tree_insert_min(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a bs_tree:
        if (is_bs_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (bs_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=1; i<=max_size; i++) {
        data->key = i;
        found = bs_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
        data->key = -i;
        found = bs_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = bs_tree_min(tree);
    if (found == NULL)           { return FAIL; }
    if (found->key != -max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = bs_tree_max(tree);
    if (found == NULL)          { return FAIL; }
    if (found->key != max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = bs_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    for (i=1; i<=max_size; i++) {
        
        found = bs_tree_remove_max(tree);

        // Not enought elements:
        if (found == NULL) { return FAIL; }
        else               { free(found); }

        // It is a bs_tree:
        if (is_bs_tree(tree) == NO) { return FAIL; }

        found = bs_tree_remove_min(tree);

        // Not enought elements:
        if (found == NULL) { return FAIL; }
        else               { free(found); }

        // It is a bs_tree:
        if (is_bs_tree(tree) == NO) { return FAIL; }
    }
    
    // It is a bs_tree:
    if (is_bs_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (bs_tree_is_empty(tree) == NO) { return FAIL; }

    free(tree);
    
    return PASS;
}

// Random insertions & deletions:
int bs_tree_random_test(int max_size) {

    int i;
    bs_tree *tree  = new_bs_tree(MyComp);
    MyData  *data  = NULL;
    MyData  *found = NULL;

    // It is a bs_tree:
    if (is_bs_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (bs_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements randomly in the tree:
    for (i=0; i<max_size*10; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = rand() % max_size;

        // Insert it:
        found = bs_tree_insert(tree, data);

        // If it was already there:
        if (found != NULL) { free(found); }

        // It is a bs_tree:
        if (is_bs_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (bs_tree_is_empty(tree) == YES) { return FAIL; }
    }

    // Check if everything is correctly sorted forward...
    found = bs_tree_min(tree);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = bs_tree_max(tree);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = bs_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Remove elements randomly from the tree:
    for (i=0; i<max_size*5; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = rand() % max_size;

        // Remove element
        found = bs_tree_remove(tree, data);

        // If it was there:
        if (found != NULL) { free(found); }

        // It is a bs_tree:
        if (is_bs_tree(tree) == NO) { return FAIL; }
    }

    // Check if everything is correctly sorted forward...
    found = bs_tree_min(tree);
    while (found != NULL) {
        data  = found;
        found = bs_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = bs_tree_max(tree);
    while (found != NULL) {
        data  = found;
        found = bs_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = bs_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    bs_tree_remove_all(tree, free);

    // It is a bs_tree:
    if (is_bs_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (bs_tree_is_empty(tree) == NO) { return FAIL; }

    free(tree);
    
    return PASS;
}

// Set functions:
int bs_tree_set_test(int max_size) {

    int i;
    MyData  *data  = NULL;
    MyData  *found = NULL;

    // Define some sets:
    bs_tree *empty = new_bs_tree(MyComp);
    bs_tree *odd   = new_bs_tree(MyComp);
    bs_tree *even  = new_bs_tree(MyComp);
    bs_tree *high  = new_bs_tree(MyComp);
    bs_tree *low   = new_bs_tree(MyComp);
    bs_tree *all   = new_bs_tree(MyComp);
    bs_tree *aux   = new_bs_tree(MyComp);
    bs_tree *aux_2 = new_bs_tree(MyComp);

    // Define the threshold between low & high:
    int half = max_size/2;

    // Insert elements in the sets:
    for (i=0; i<max_size; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;

        // Put them all in "all":
        found = bs_tree_insert(all, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a bs_tree:
        if (is_bs_tree(all) == NO)        { return FAIL; }

        // It is not empty:
        if (bs_tree_is_empty(all) == YES) { return FAIL; }

        // Split them between even & odd:
        if (i%2 == 0) { 
            
            found = bs_tree_insert(even, data);

            // It was not already there:
            if (found != NULL) { return FAIL; }

            // It is a bs_tree:
            if (is_bs_tree(even) == NO)        { return FAIL; }

            // It is not empty:
            if (bs_tree_is_empty(even) == YES) { return FAIL; }

        } else {
            
            found = bs_tree_insert(odd, data);

            // It was not already there:
            if (found != NULL) { return FAIL; }

            // It is a bs_tree:
            if (is_bs_tree(odd) == NO)        { return FAIL; }

            // It is not empty:
            if (bs_tree_is_empty(odd) == YES) { return FAIL; }

        }
            
        // Split them between low & high:
        if (i <= half) { 
            
            found = bs_tree_insert(low, data);

            // It was not already there:
            if (found != NULL) { return FAIL; }

            // It is a bs_tree:
            if (is_bs_tree(low) == NO)        { return FAIL; }

            // It is not empty:
            if (bs_tree_is_empty(low) == YES) { return FAIL; }

        } else {
            
            found = bs_tree_insert(high, data);

            // It was not already there:
            if (found != NULL) { return FAIL; }

            // It is a bs_tree:
            if (is_bs_tree(high) == NO)        { return FAIL; }

            // It is not empty:
            if (bs_tree_is_empty(high) == YES) { return FAIL; }

        }
    }

    // TEST COPY: //////////////////////////////////////////////////////////////

    // AUX == ALL
    aux = bs_tree_copy(all);

    // It is a bs_tree:
    if (is_bs_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (bs_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = bs_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = bs_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    bs_tree_remove_all(aux, NULL);

    // TEST UNION: /////////////////////////////////////////////////////////////

    // AUX == UNION(low, high) == ALL
    aux = bs_tree_union(low, high);

    // It is a bs_tree:
    if (is_bs_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (bs_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = bs_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = bs_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    bs_tree_remove_all(aux, NULL);

    // AUX == UNION(odd, all) == ALL
    aux = bs_tree_union(odd, all);

    // It is a bs_tree:
    if (is_bs_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (bs_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = bs_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = bs_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    bs_tree_remove_all(aux, NULL);

    // AUX == UNION(odd, even) == ALL
    aux = bs_tree_union(odd, even);

    // It is a bs_tree:
    if (is_bs_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (bs_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = bs_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = bs_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    bs_tree_remove_all(aux, NULL);

    // AUX == UNION(odd, empty) == ODD
    aux = bs_tree_union(odd, empty);

    // It is a bs_tree:
    if (is_bs_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (bs_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "odd"...
    found = bs_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(odd, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "odd" is also in "aux".
    found = bs_tree_min(odd);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(odd, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(odd);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    bs_tree_remove_all(aux, NULL);

    // AUX == UNION(odd, low)
    aux = bs_tree_union(odd, low);

    // It is a bs_tree:
    if (is_bs_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (bs_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "odd" is also in "aux"...
    found = bs_tree_min(odd);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(odd, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(odd);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "low" is also in "aux".
    found = bs_tree_min(low);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(low, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(low);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    bs_tree_remove_all(aux, NULL);

    // TEST INTERSECTION: //////////////////////////////////////////////////////

    // AUX == INTERSECTION(odd, all) == ODD
    aux = bs_tree_intersection(odd, all);

    // It is a bs_tree:
    if (is_bs_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (bs_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "odd"...
    found = bs_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(odd, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "odd" is also in "aux".
    found = bs_tree_min(odd);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(odd, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(odd);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    bs_tree_remove_all(aux, NULL);

    // AUX == INTERSECTION(odd, high)
    aux = bs_tree_intersection(odd, high);

    // It is a bs_tree:
    if (is_bs_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (bs_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "odd"...
    found = bs_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(odd, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "aux" is also in "high"...
    found = bs_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(high, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    bs_tree_remove_all(aux, NULL);

    // AUX == INTERSECTION(empty, all) == EMPTY
    aux = bs_tree_intersection(empty, all);

    // It is a bs_tree:
    if (is_bs_tree(aux) == NO)       { return FAIL; }

    // It is empty:
    if (bs_tree_is_empty(aux) == NO) { return FAIL; }

    // AUX == INTERSECTION(odd, even) == EMPTY
    aux = bs_tree_intersection(odd, even);

    // It is a bs_tree:
    if (is_bs_tree(aux) == NO)       { return FAIL; }

    // It is empty:
    if (bs_tree_is_empty(aux) == NO) { return FAIL; }

    // TEST DIFFERENCE: ////////////////////////////////////////////////////////

    // AUX == DIFFERENCE(all, odd) == EVEN
    aux = bs_tree_diff(all, odd);

    // It is a bs_tree:
    if (is_bs_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (bs_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "even"...
    found = bs_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(even, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "even" is also in "aux".
    found = bs_tree_min(even);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(even, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(even);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    bs_tree_remove_all(aux, NULL);

    // AUX == DIFFERENCE(odd, high) == INTERSECTION(odd, low) == AUX_2
    aux   = bs_tree_diff(odd, high);
    aux_2 = bs_tree_intersection(odd, low);

    // It is a bs_tree:
    if (is_bs_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (bs_tree_is_empty(aux) == YES) { return FAIL; }

    // It is a bs_tree:
    if (is_bs_tree(aux_2) == NO)        { return FAIL; }

    // It is not empty:
    if (bs_tree_is_empty(aux_2) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "aux_2"...
    found = bs_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(aux_2, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "aux_2" is also in "aux"...
    found = bs_tree_min(aux_2);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(aux_2, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(aux_2);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    bs_tree_remove_all(aux,   NULL);
    bs_tree_remove_all(aux_2, NULL);

    // AUX == DIFFERENCE(all, empty) == ALL
    aux = bs_tree_diff(all, empty);

    // It is a bs_tree:
    if (is_bs_tree(aux) == NO)        { return FAIL; }

    // It is empty:
    if (bs_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = bs_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = bs_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    bs_tree_remove_all(aux, NULL);

    // AUX == DIFFERENCE(empty, all) == EMPTY
    aux = bs_tree_diff(empty, all);

    // It is a bs_tree:
    if (is_bs_tree(aux) == NO)       { return FAIL; }

    // It is empty:
    if (bs_tree_is_empty(aux) == NO) { return FAIL; }

    // TEST SYMMETRIC DIFFERENCE: //////////////////////////////////////////////

    // AUX == SYMMETRIC_DIFFERENCE(all, odd) == EVEN
    aux = bs_tree_sym_diff(all, odd);

    // It is a bs_tree:
    if (is_bs_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (bs_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "even"...
    found = bs_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(even, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "even" is also in "aux".
    found = bs_tree_min(even);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(even, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(even);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    bs_tree_remove_all(aux, NULL);

    // AUX == SYMMETRIC_DIFFERENCE(odd, high)
    aux = bs_tree_sym_diff(odd, high);

    // It is a bs_tree:
    if (is_bs_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (bs_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "odd" or "high" but not in both.
    found = bs_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(odd, data);
        if (found == NULL) {
            found = bs_tree_search(high, data);
            if (found == NULL) { return FAIL; }
        } else {
            found = bs_tree_search(high, data);
            if (found != NULL) { return FAIL; }
        }
        found = bs_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    bs_tree_remove_all(aux, NULL);

    // AUX == SYMMETRIC_DIFFERENCE(all, empty) == ALL
    aux = bs_tree_sym_diff(all, empty);

    // It is a bs_tree:
    if (is_bs_tree(aux) == NO)        { return FAIL; }

    // It is empty:
    if (bs_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = bs_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = bs_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    bs_tree_remove_all(aux, NULL);

    // AUX == SYMMETRIC_DIFFERENCE(empty, all) == ALL
    aux = bs_tree_sym_diff(empty, all);

    // It is not empty:
    if (bs_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = bs_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = bs_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = bs_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = bs_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = bs_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    bs_tree_remove_all(aux, NULL);

    // FINAL CLEAN UP:
    bs_tree_remove_all(aux, NULL);
    bs_tree_remove_all(odd, NULL);
    bs_tree_remove_all(low, NULL);
    bs_tree_remove_all(even, NULL);
    bs_tree_remove_all(high, NULL);
    bs_tree_remove_all(all, free);
    free(aux);
    free(odd);
    free(low);
    free(even);
    free(high);
    free(all);
    
    return PASS;
}


// Sequential insertions & complete deletion:
int rb_tree_sequential_test(int max_size) {

    int i;
    rb_tree *tree  = new_rb_tree(MyComp);
    MyData  *data  = NULL;
    MyData  *found = NULL;

    // It is a rb_tree:
    if (is_rb_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (rb_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=0; i<max_size; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = rb_tree_insert(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a rb_tree:
        if (is_rb_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (rb_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=0; i<max_size; i++) {
        data->key = i;
        found = rb_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = rb_tree_min(tree);
    if (found == NULL)   { return FAIL; }
    if (found->key != 0) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = rb_tree_max(tree);
    if (found == NULL)            { return FAIL; }
    if (found->key != max_size-1) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = rb_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    rb_tree_remove_all(tree, free);

    // It is a rb_tree:
    if (is_rb_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (rb_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=max_size; i>0; i--) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = rb_tree_insert(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a rb_tree:
        if (is_rb_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (rb_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=max_size; i>0; i--) {
        data->key = i;
        found = rb_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = rb_tree_min(tree);
    if (found == NULL)   { return FAIL; }
    if (found->key != 1) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = rb_tree_max(tree);
    if (found == NULL)          { return FAIL; }
    if (found->key != max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = rb_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    rb_tree_remove_all(tree, free);

    // It is a rb_tree:
    if (is_rb_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (rb_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=1; i<=max_size; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = rb_tree_insert(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a rb_tree:
        if (is_rb_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (rb_tree_is_empty(tree) == YES) { return FAIL; }

        data = (MyData *) malloc(sizeof(MyData));
        data->key = -i;
        found = rb_tree_insert(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a rb_tree:
        if (is_rb_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (rb_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=1; i<=max_size; i++) {
        data->key = i;
        found = rb_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
        data->key = -i;
        found = rb_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = rb_tree_min(tree);
    if (found == NULL)           { return FAIL; }
    if (found->key != -max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = rb_tree_max(tree);
    if (found == NULL)          { return FAIL; }
    if (found->key != max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = rb_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    rb_tree_remove_all(tree, free);

    // It is a rb_tree:
    if (is_rb_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (rb_tree_is_empty(tree) == NO) { return FAIL; }

    free(tree);
    
    return PASS;
}

// Fast Sequential insertions & deletions:
int rb_tree_fast_sequential_test(int max_size) {

    int i;
    rb_tree *tree  = new_rb_tree(MyComp);
    MyData  *data  = NULL;
    MyData  *found = NULL;

    // It is a rb_tree:
    if (is_rb_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (rb_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=0; i<max_size; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = rb_tree_insert_max(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a rb_tree:
        if (is_rb_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (rb_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=0; i<max_size; i++) {
        data->key = i;
        found = rb_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = rb_tree_min(tree);
    if (found == NULL)   { return FAIL; }
    if (found->key != 0) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = rb_tree_max(tree);
    if (found == NULL)            { return FAIL; }
    if (found->key != max_size-1) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = rb_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    for (i=max_size; i>0; i--) {
        
        found = rb_tree_remove_max(tree);

        // Not enought elements:
        if (found == NULL) { return FAIL; }
        else               { free(found); }

        // It is a rb_tree:
        if (is_rb_tree(tree) == NO) { return FAIL; }
    }
    
    // It is a rb_tree:
    if (is_rb_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (rb_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=max_size; i>0; i--) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = rb_tree_insert_min(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a rb_tree:
        if (is_rb_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (rb_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=max_size; i>0; i--) {
        data->key = i;
        found = rb_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = rb_tree_min(tree);
    if (found == NULL)   { return FAIL; }
    if (found->key != 1) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = rb_tree_max(tree);
    if (found == NULL)          { return FAIL; }
    if (found->key != max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = rb_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    for (i=max_size; i>0; i--) {
        
        found = rb_tree_remove_min(tree);

        // Not enought elements:
        if (found == NULL) { return FAIL; }
        else               { free(found); }

        // It is a rb_tree:
        if (is_rb_tree(tree) == NO) { return FAIL; }
    }
    
    // It is a rb_tree:
    if (is_rb_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (rb_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=1; i<=max_size; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = rb_tree_insert_max(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a rb_tree:
        if (is_rb_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (rb_tree_is_empty(tree) == YES) { return FAIL; }

        data = (MyData *) malloc(sizeof(MyData));
        data->key = -i;
        found = rb_tree_insert_min(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a rb_tree:
        if (is_rb_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (rb_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=1; i<=max_size; i++) {
        data->key = i;
        found = rb_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
        data->key = -i;
        found = rb_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = rb_tree_min(tree);
    if (found == NULL)           { return FAIL; }
    if (found->key != -max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = rb_tree_max(tree);
    if (found == NULL)          { return FAIL; }
    if (found->key != max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = rb_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    for (i=1; i<=max_size; i++) {
        
        found = rb_tree_remove_max(tree);

        // Not enought elements:
        if (found == NULL) { return FAIL; }
        else               { free(found); }

        // It is a rb_tree:
        if (is_rb_tree(tree) == NO) { return FAIL; }

        found = rb_tree_remove_min(tree);

        // Not enought elements:
        if (found == NULL) { return FAIL; }
        else               { free(found); }

        // It is a rb_tree:
        if (is_rb_tree(tree) == NO) { return FAIL; }
    }
    
    // It is a rb_tree:
    if (is_rb_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (rb_tree_is_empty(tree) == NO) { return FAIL; }

    free(tree);
    
    return PASS;
}

// Random insertions & deletions:
int rb_tree_random_test(int max_size) {

    int i;
    rb_tree *tree  = new_rb_tree(MyComp);
    MyData  *data  = NULL;
    MyData  *found = NULL;

    // It is a rb_tree:
    if (is_rb_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (rb_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements randomly in the tree:
    for (i=0; i<max_size*10; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = rand() % max_size;

        // Insert it:
        found = rb_tree_insert(tree, data);

        // If it was already there:
        if (found != NULL) { free(found); }

        // It is a rb_tree:
        if (is_rb_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (rb_tree_is_empty(tree) == YES) { return FAIL; }
    }

    // Check if everything is correctly sorted forward...
    found = rb_tree_min(tree);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = rb_tree_max(tree);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = rb_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Remove elements randomly from the tree:
    for (i=0; i<max_size*5; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = rand() % max_size;

        // Remove element
        found = rb_tree_remove(tree, data);

        // If it was there:
        if (found != NULL) { free(found); }

        // It is a rb_tree:
        if (is_rb_tree(tree) == NO) { return FAIL; }
    }

    // Check if everything is correctly sorted forward...
    found = rb_tree_min(tree);
    while (found != NULL) {
        data  = found;
        found = rb_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = rb_tree_max(tree);
    while (found != NULL) {
        data  = found;
        found = rb_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = rb_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    rb_tree_remove_all(tree, free);

    // It is a rb_tree:
    if (is_rb_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (rb_tree_is_empty(tree) == NO) { return FAIL; }

    free(tree);
    
    return PASS;
}

// Set functions:
int rb_tree_set_test(int max_size) {

    int i;
    MyData  *data  = NULL;
    MyData  *found = NULL;

    // Define some sets:
    rb_tree *empty = new_rb_tree(MyComp);
    rb_tree *odd   = new_rb_tree(MyComp);
    rb_tree *even  = new_rb_tree(MyComp);
    rb_tree *high  = new_rb_tree(MyComp);
    rb_tree *low   = new_rb_tree(MyComp);
    rb_tree *all   = new_rb_tree(MyComp);
    rb_tree *aux   = new_rb_tree(MyComp);
    rb_tree *aux_2 = new_rb_tree(MyComp);

    // Define the threshold between low & high:
    int half = max_size/2;

    // Insert elements in the sets:
    for (i=0; i<max_size; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;

        // Put them all in "all":
        found = rb_tree_insert(all, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a rb_tree:
        if (is_rb_tree(all) == NO)        { return FAIL; }

        // It is not empty:
        if (rb_tree_is_empty(all) == YES) { return FAIL; }

        // Split them between even & odd:
        if (i%2 == 0) { 
            
            found = rb_tree_insert(even, data);

            // It was not already there:
            if (found != NULL) { return FAIL; }

            // It is a rb_tree:
            if (is_rb_tree(even) == NO)        { return FAIL; }

            // It is not empty:
            if (rb_tree_is_empty(even) == YES) { return FAIL; }

        } else {
            
            found = rb_tree_insert(odd, data);

            // It was not already there:
            if (found != NULL) { return FAIL; }

            // It is a rb_tree:
            if (is_rb_tree(odd) == NO)        { return FAIL; }

            // It is not empty:
            if (rb_tree_is_empty(odd) == YES) { return FAIL; }

        }
            
        // Split them between low & high:
        if (i <= half) { 
            
            found = rb_tree_insert(low, data);

            // It was not already there:
            if (found != NULL) { return FAIL; }

            // It is a rb_tree:
            if (is_rb_tree(low) == NO)        { return FAIL; }

            // It is not empty:
            if (rb_tree_is_empty(low) == YES) { return FAIL; }

        } else {
            
            found = rb_tree_insert(high, data);

            // It was not already there:
            if (found != NULL) { return FAIL; }

            // It is a rb_tree:
            if (is_rb_tree(high) == NO)        { return FAIL; }

            // It is not empty:
            if (rb_tree_is_empty(high) == YES) { return FAIL; }

        }
    }

    // TEST COPY: //////////////////////////////////////////////////////////////

    // AUX == ALL
    aux = rb_tree_copy(all);

    // It is a rb_tree:
    if (is_rb_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (rb_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = rb_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = rb_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    rb_tree_remove_all(aux, NULL);

    // TEST UNION: /////////////////////////////////////////////////////////////

    // AUX == UNION(low, high) == ALL
    aux = rb_tree_union(low, high);

    // It is a rb_tree:
    if (is_rb_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (rb_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = rb_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = rb_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    rb_tree_remove_all(aux, NULL);

    // AUX == UNION(odd, all) == ALL
    aux = rb_tree_union(odd, all);

    // It is a rb_tree:
    if (is_rb_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (rb_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = rb_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = rb_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    rb_tree_remove_all(aux, NULL);

    // AUX == UNION(odd, even) == ALL
    aux = rb_tree_union(odd, even);

    // It is a rb_tree:
    if (is_rb_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (rb_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = rb_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = rb_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    rb_tree_remove_all(aux, NULL);

    // AUX == UNION(odd, empty) == ODD
    aux = rb_tree_union(odd, empty);

    // It is a rb_tree:
    if (is_rb_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (rb_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "odd"...
    found = rb_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(odd, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "odd" is also in "aux".
    found = rb_tree_min(odd);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(odd, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(odd);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    rb_tree_remove_all(aux, NULL);

    // AUX == UNION(odd, low)
    aux = rb_tree_union(odd, low);

    // It is a rb_tree:
    if (is_rb_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (rb_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "odd" is also in "aux"...
    found = rb_tree_min(odd);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(odd, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(odd);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "low" is also in "aux".
    found = rb_tree_min(low);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(low, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(low);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    rb_tree_remove_all(aux, NULL);

    // TEST INTERSECTION: //////////////////////////////////////////////////////

    // AUX == INTERSECTION(odd, all) == ODD
    aux = rb_tree_intersection(odd, all);

    // It is a rb_tree:
    if (is_rb_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (rb_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "odd"...
    found = rb_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(odd, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "odd" is also in "aux".
    found = rb_tree_min(odd);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(odd, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(odd);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    rb_tree_remove_all(aux, NULL);

    // AUX == INTERSECTION(odd, high)
    aux = rb_tree_intersection(odd, high);

    // It is a rb_tree:
    if (is_rb_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (rb_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "odd"...
    found = rb_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(odd, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "aux" is also in "high"...
    found = rb_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(high, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    rb_tree_remove_all(aux, NULL);

    // AUX == INTERSECTION(empty, all) == EMPTY
    aux = rb_tree_intersection(empty, all);

    // It is a rb_tree:
    if (is_rb_tree(aux) == NO)       { return FAIL; }

    // It is empty:
    if (rb_tree_is_empty(aux) == NO) { return FAIL; }

    // AUX == INTERSECTION(odd, even) == EMPTY
    aux = rb_tree_intersection(odd, even);

    // It is a rb_tree:
    if (is_rb_tree(aux) == NO)       { return FAIL; }

    // It is empty:
    if (rb_tree_is_empty(aux) == NO) { return FAIL; }

    // TEST DIFFERENCE: ////////////////////////////////////////////////////////

    // AUX == DIFFERENCE(all, odd) == EVEN
    aux = rb_tree_diff(all, odd);

    // It is a rb_tree:
    if (is_rb_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (rb_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "even"...
    found = rb_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(even, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "even" is also in "aux".
    found = rb_tree_min(even);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(even, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(even);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    rb_tree_remove_all(aux, NULL);
    
    // AUX == DIFFERENCE(odd, high) == INTERSECTION(odd, low) == AUX_2
    aux   = rb_tree_diff(odd, high);
    aux_2 = rb_tree_intersection(odd, low);
    
    // It is a rb_tree:
    if (is_rb_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (rb_tree_is_empty(aux) == YES) { return FAIL; }

    // It is a rb_tree:
    if (is_rb_tree(aux_2) == NO)        { return FAIL; }

    // It is not empty:
    if (rb_tree_is_empty(aux_2) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "aux_2"...
    found = rb_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(aux_2, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "aux_2" is also in "aux"...
    found = rb_tree_min(aux_2);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(aux_2, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(aux_2);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    rb_tree_remove_all(aux,   NULL);
    rb_tree_remove_all(aux_2, NULL);

    // AUX == DIFFERENCE(all, empty) == ALL
    aux = rb_tree_diff(all, empty);

    // It is a rb_tree:
    if (is_rb_tree(aux) == NO)        { return FAIL; }

    // It is empty:
    if (rb_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = rb_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = rb_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    rb_tree_remove_all(aux, NULL);

    // AUX == DIFFERENCE(empty, all) == EMPTY
    aux = rb_tree_diff(empty, all);

    // It is a rb_tree:
    if (is_rb_tree(aux) == NO)       { return FAIL; }

    // It is empty:
    if (rb_tree_is_empty(aux) == NO) { return FAIL; }

    // TEST SYMMETRIC DIFFERENCE: //////////////////////////////////////////////

    // AUX == SYMMETRIC_DIFFERENCE(all, odd) == EVEN
    aux = rb_tree_sym_diff(all, odd);

    // It is a rb_tree:
    if (is_rb_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (rb_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "even"...
    found = rb_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(even, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "even" is also in "aux".
    found = rb_tree_min(even);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(even, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(even);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    rb_tree_remove_all(aux, NULL);

    // AUX == SYMMETRIC_DIFFERENCE(odd, high)
    aux = rb_tree_sym_diff(odd, high);

    // It is a rb_tree:
    if (is_rb_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (rb_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "odd" or "high" but not in both.
    found = rb_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(odd, data);
        if (found == NULL) {
            found = rb_tree_search(high, data);
            if (found == NULL) { return FAIL; }
        } else {
            found = rb_tree_search(high, data);
            if (found != NULL) { return FAIL; }
        }
        found = rb_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    rb_tree_remove_all(aux, NULL);

    // AUX == SYMMETRIC_DIFFERENCE(all, empty) == ALL
    aux = rb_tree_sym_diff(all, empty);

    // It is a rb_tree:
    if (is_rb_tree(aux) == NO)        { return FAIL; }

    // It is empty:
    if (rb_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = rb_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = rb_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    rb_tree_remove_all(aux, NULL);

    // AUX == SYMMETRIC_DIFFERENCE(empty, all) == ALL
    aux = rb_tree_sym_diff(empty, all);

    // It is not empty:
    if (rb_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = rb_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = rb_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = rb_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = rb_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = rb_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    rb_tree_remove_all(aux, NULL);

    // FINAL CLEAN UP:
    rb_tree_remove_all(aux, NULL);
    rb_tree_remove_all(odd, NULL);
    rb_tree_remove_all(low, NULL);
    rb_tree_remove_all(even, NULL);
    rb_tree_remove_all(high, NULL);
    rb_tree_remove_all(all, free);
    free(aux);
    free(odd);
    free(low);
    free(even);
    free(high);
    free(all);
    
    return PASS;
}


// Sequential insertions & complete deletion:
int sp_tree_sequential_test(int max_size) {

    int i;
    sp_tree *tree  = new_sp_tree(MyComp);
    MyData  *data  = NULL;
    MyData  *found = NULL;

    // It is a sp_tree:
    if (is_sp_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (sp_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=0; i<max_size; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = sp_tree_insert(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a sp_tree:
        if (is_sp_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (sp_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=0; i<max_size; i++) {
        data->key = i;
        found = sp_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = sp_tree_min(tree);
    if (found == NULL)   { return FAIL; }
    if (found->key != 0) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = sp_tree_max(tree);
    if (found == NULL)            { return FAIL; }
    if (found->key != max_size-1) { return FAIL; }    
    while (found != NULL) {
        data  = found;
        found = sp_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = sp_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    sp_tree_remove_all(tree, free);

    // It is a sp_tree:
    if (is_sp_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (sp_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=max_size; i>0; i--) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = sp_tree_insert(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a sp_tree:
        if (is_sp_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (sp_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=max_size; i>0; i--) {
        data->key = i;
        found = sp_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = sp_tree_min(tree);
    if (found == NULL)   { return FAIL; }
    if (found->key != 1) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = sp_tree_max(tree);
    if (found == NULL)          { return FAIL; }
    if (found->key != max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = sp_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    sp_tree_remove_all(tree, free);

    // It is a sp_tree:
    if (is_sp_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (sp_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=1; i<=max_size; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = sp_tree_insert(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a sp_tree:
        if (is_sp_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (sp_tree_is_empty(tree) == YES) { return FAIL; }

        data = (MyData *) malloc(sizeof(MyData));
        data->key = -i;
        found = sp_tree_insert(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a sp_tree:
        if (is_sp_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (sp_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=1; i<=max_size; i++) {
        data->key = i;
        found = sp_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
        data->key = -i;
        found = sp_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = sp_tree_min(tree);
    if (found == NULL)           { return FAIL; }
    if (found->key != -max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = sp_tree_max(tree);
    if (found == NULL)          { return FAIL; }
    if (found->key != max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = sp_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    sp_tree_remove_all(tree, free);

    // It is a sp_tree:
    if (is_sp_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (sp_tree_is_empty(tree) == NO) { return FAIL; }

    free(tree);
    
    return PASS;
}

// Fast Sequential insertions & deletions:
int sp_tree_fast_sequential_test(int max_size) {

    int i;
    sp_tree *tree  = new_sp_tree(MyComp);
    MyData  *data  = NULL;
    MyData  *found = NULL;

    // It is a sp_tree:
    if (is_sp_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (sp_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=0; i<max_size; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = sp_tree_insert_max(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a sp_tree:
        if (is_sp_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (sp_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=0; i<max_size; i++) {
        data->key = i;
        found = sp_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = sp_tree_min(tree);
    if (found == NULL)   { return FAIL; }
    if (found->key != 0) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = sp_tree_max(tree);
    if (found == NULL)            { return FAIL; }
    if (found->key != max_size-1) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = sp_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    for (i=max_size; i>0; i--) {
        
        found = sp_tree_remove_max(tree);

        // Not enought elements:
        if (found == NULL) { return FAIL; }
        else               { free(found); }

        // It is a sp_tree:
        if (is_sp_tree(tree) == NO) { return FAIL; }
    }
    
    // It is a sp_tree:
    if (is_sp_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (sp_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=max_size; i>0; i--) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = sp_tree_insert_min(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a sp_tree:
        if (is_sp_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (sp_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=max_size; i>0; i--) {
        data->key = i;
        found = sp_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = sp_tree_min(tree);
    if (found == NULL)   { return FAIL; }
    if (found->key != 1) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = sp_tree_max(tree);
    if (found == NULL)          { return FAIL; }
    if (found->key != max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = sp_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    for (i=max_size; i>0; i--) {
        
        found = sp_tree_remove_min(tree);

        // Not enought elements:
        if (found == NULL) { return FAIL; }
        else               { free(found); }

        // It is a sp_tree:
        if (is_sp_tree(tree) == NO) { return FAIL; }
    }
    
    // It is a sp_tree:
    if (is_sp_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (sp_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements sequentially in the tree:
    for (i=1; i<=max_size; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;
        found = sp_tree_insert_max(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a sp_tree:
        if (is_sp_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (sp_tree_is_empty(tree) == YES) { return FAIL; }

        data = (MyData *) malloc(sizeof(MyData));
        data->key = -i;
        found = sp_tree_insert_min(tree, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a sp_tree:
        if (is_sp_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (sp_tree_is_empty(tree) == YES) { return FAIL; }

    }

    // Check if all elements are there:
    data = (MyData *) malloc(sizeof(MyData));
    for (i=1; i<=max_size; i++) {
        data->key = i;
        found = sp_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
        data->key = -i;
        found = sp_tree_search(tree, data);
        if (found == NULL)            { return FAIL; }
        if (MyComp(data, found) != 0) { return FAIL; }
    }
    free(data);

    // Check if everything is correctly sorted forward...
    found = sp_tree_min(tree);
    if (found == NULL)           { return FAIL; }
    if (found->key != -max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = sp_tree_max(tree);
    if (found == NULL)          { return FAIL; }
    if (found->key != max_size) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = sp_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    for (i=1; i<=max_size; i++) {
        
        found = sp_tree_remove_max(tree);

        // Not enought elements:
        if (found == NULL) { return FAIL; }
        else               { free(found); }

        // It is a sp_tree:
        if (is_sp_tree(tree) == NO) { return FAIL; }

        found = sp_tree_remove_min(tree);

        // Not enought elements:
        if (found == NULL) { return FAIL; }
        else               { free(found); }

        // It is a sp_tree:
        if (is_sp_tree(tree) == NO) { return FAIL; }
    }
    
    // It is a sp_tree:
    if (is_sp_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (sp_tree_is_empty(tree) == NO) { return FAIL; }

    free(tree);
    
    return PASS;
}

// Random insertions & deletions:
int sp_tree_random_test(int max_size) {

    int i;
    sp_tree *tree  = new_sp_tree(MyComp);
    MyData  *data  = NULL;
    MyData  *found = NULL;

    // It is a sp_tree:
    if (is_sp_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (sp_tree_is_empty(tree) == NO) { return FAIL; }

    // Insert elements randomly in the tree:
    for (i=0; i<max_size*10; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = rand() % max_size;

        // Insert it:
        found = sp_tree_insert(tree, data);
        
        // If it was already there:
        if (found != NULL) { free(found); }

        // It is a sp_tree:
        if (is_sp_tree(tree) == NO)        { return FAIL; }

        // It is not empty:
        if (sp_tree_is_empty(tree) == YES) { return FAIL; }
    }

    // Check if everything is correctly sorted forward...
    found = sp_tree_min(tree);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and backwards.
    found = sp_tree_max(tree);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = sp_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Remove elements randomly from the tree:
    for (i=0; i<max_size*5; i++) {

        data = (MyData *) malloc(sizeof(MyData));
        data->key = rand() % max_size;

        // Remove element
        found = sp_tree_remove(tree, data);

        // If it was there:
        if (found != NULL) { free(found); }
        else               { free(data); data = NULL; }

        // It is a sp_tree:
        if (is_sp_tree(tree) == NO) { return FAIL; }

    }

    // Check if everything is correctly sorted forward...
    found = sp_tree_min(tree);
    while (found != NULL) {
        data  = found;
        found = sp_tree_next(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // ...and backwards.
    found = sp_tree_max(tree);
    while (found != NULL) {
        data  = found;
        found = sp_tree_prev(tree, data);
        if (found != NULL) {
            if (MyComp(data, found) <= 0) { return FAIL; }
        } else { 
            found = sp_tree_min(tree);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break; 
        }
    }

    // Now, clean everything!
    sp_tree_remove_all(tree, free);

    // It is a sp_tree:
    if (is_sp_tree(tree) == NO) { return FAIL; }

    // It is empty:
    if (sp_tree_is_empty(tree) == NO) { return FAIL; }

    free(tree);
    
    return PASS;
}

// Set functions:
int sp_tree_set_test(int max_size) {

    int i;
    MyData  *data  = NULL;
    MyData  *found = NULL;

    // Define some sets:
    sp_tree *empty = new_sp_tree(MyComp);
    sp_tree *odd   = new_sp_tree(MyComp);
    sp_tree *even  = new_sp_tree(MyComp);
    sp_tree *high  = new_sp_tree(MyComp);
    sp_tree *low   = new_sp_tree(MyComp);
    sp_tree *all   = new_sp_tree(MyComp);
    sp_tree *aux   = new_sp_tree(MyComp);
    sp_tree *aux_2 = new_sp_tree(MyComp);

    // Define the threshold between low & high:
    int half = max_size/2;

    // Insert elements in the sets:
    for (i=0; i<max_size; i++) {
        
        data = (MyData *) malloc(sizeof(MyData));
        data->key = i;

        // Put them all in "all":
        found = sp_tree_insert(all, data);

        // It was not already there:
        if (found != NULL) { return FAIL; }

        // It is a sp_tree:
        if (is_sp_tree(all) == NO)        { return FAIL; }

        // It is not empty:
        if (sp_tree_is_empty(all) == YES) { return FAIL; }

        // Split them between even & odd:
        if (i%2 == 0) { 
            
            found = sp_tree_insert(even, data);

            // It was not already there:
            if (found != NULL) { return FAIL; }

            // It is a sp_tree:
            if (is_sp_tree(even) == NO)        { return FAIL; }

            // It is not empty:
            if (sp_tree_is_empty(even) == YES) { return FAIL; }

        } else {
            
            found = sp_tree_insert(odd, data);

            // It was not already there:
            if (found != NULL) { return FAIL; }

            // It is a sp_tree:
            if (is_sp_tree(odd) == NO)        { return FAIL; }

            // It is not empty:
            if (sp_tree_is_empty(odd) == YES) { return FAIL; }

        }
            
        // Split them between low & high:
        if (i <= half) { 
            
            found = sp_tree_insert(low, data);

            // It was not already there:
            if (found != NULL) { return FAIL; }

            // It is a sp_tree:
            if (is_sp_tree(low) == NO)        { return FAIL; }

            // It is not empty:
            if (sp_tree_is_empty(low) == YES) { return FAIL; }

        } else {
            
            found = sp_tree_insert(high, data);

            // It was not already there:
            if (found != NULL) { return FAIL; }

            // It is a sp_tree:
            if (is_sp_tree(high) == NO)        { return FAIL; }

            // It is not empty:
            if (sp_tree_is_empty(high) == YES) { return FAIL; }

        }
    }

    // TEST COPY: //////////////////////////////////////////////////////////////

    // AUX == ALL
    aux = sp_tree_copy(all);

    // It is a sp_tree:
    if (is_sp_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (sp_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = sp_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = sp_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    sp_tree_remove_all(aux, NULL);

    // TEST UNION: /////////////////////////////////////////////////////////////

    // AUX == UNION(low, high) == ALL
    aux = sp_tree_union(low, high);

    // It is a sp_tree:
    if (is_sp_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (sp_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = sp_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = sp_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    sp_tree_remove_all(aux, NULL);

    // AUX == UNION(odd, all) == ALL
    aux = sp_tree_union(odd, all);

    // It is a sp_tree:
    if (is_sp_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (sp_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = sp_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = sp_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    sp_tree_remove_all(aux, NULL);

    // AUX == UNION(odd, even) == ALL
    aux = sp_tree_union(odd, even);

    // It is a sp_tree:
    if (is_sp_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (sp_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = sp_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = sp_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    sp_tree_remove_all(aux, NULL);

    // AUX == UNION(odd, empty) == ODD
    aux = sp_tree_union(odd, empty);

    // It is a sp_tree:
    if (is_sp_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (sp_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "odd"...
    found = sp_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(odd, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "odd" is also in "aux".
    found = sp_tree_min(odd);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(odd, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(odd);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    sp_tree_remove_all(aux, NULL);

    // AUX == UNION(odd, low)
    aux = sp_tree_union(odd, low);

    // It is a sp_tree:
    if (is_sp_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (sp_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "odd" is also in "aux"...
    found = sp_tree_min(odd);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(odd, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(odd);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "low" is also in "aux".
    found = sp_tree_min(low);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(low, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(low);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    sp_tree_remove_all(aux, NULL);

    // TEST INTERSECTION: //////////////////////////////////////////////////////

    // AUX == INTERSECTION(odd, all) == ODD
    aux = sp_tree_intersection(odd, all);

    // It is a sp_tree:
    if (is_sp_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (sp_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "odd"...
    found = sp_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(odd, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "odd" is also in "aux".
    found = sp_tree_min(odd);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(odd, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(odd);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    sp_tree_remove_all(aux, NULL);

    // AUX == INTERSECTION(odd, high)
    aux = sp_tree_intersection(odd, high);

    // It is a sp_tree:
    if (is_sp_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (sp_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "odd"...
    found = sp_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(odd, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "aux" is also in "high"...
    found = sp_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(high, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    sp_tree_remove_all(aux, NULL);

    // AUX == INTERSECTION(empty, all) == EMPTY
    aux = sp_tree_intersection(empty, all);

    // It is a sp_tree:
    if (is_sp_tree(aux) == NO)       { return FAIL; }

    // It is empty:
    if (sp_tree_is_empty(aux) == NO) { return FAIL; }

    // AUX == INTERSECTION(odd, even) == EMPTY
    aux = sp_tree_intersection(odd, even);

    // It is a sp_tree:
    if (is_sp_tree(aux) == NO)       { return FAIL; }

    // It is empty:
    if (sp_tree_is_empty(aux) == NO) { return FAIL; }

    // TEST DIFFERENCE: ////////////////////////////////////////////////////////

    // AUX == DIFFERENCE(all, odd) == EVEN
    aux = sp_tree_diff(all, odd);

    // It is a sp_tree:
    if (is_sp_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (sp_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "even"...
    found = sp_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(even, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "even" is also in "aux".
    found = sp_tree_min(even);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(even, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(even);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    sp_tree_remove_all(aux, NULL);

    // AUX == DIFFERENCE(odd, high) == INTERSECTION(odd, low) == AUX_2
    aux   = sp_tree_diff(odd, high);
    aux_2 = sp_tree_intersection(odd, low);

    // It is a sp_tree:
    if (is_sp_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (sp_tree_is_empty(aux) == YES) { return FAIL; }

    // It is a sp_tree:
    if (is_sp_tree(aux_2) == NO)        { return FAIL; }

    // It is not empty:
    if (sp_tree_is_empty(aux_2) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "aux_2"...
    found = sp_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(aux_2, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "aux_2" is also in "aux"...
    found = sp_tree_min(aux_2);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(aux_2, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(aux_2);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    sp_tree_remove_all(aux,   NULL);
    sp_tree_remove_all(aux_2, NULL);

    // AUX == DIFFERENCE(all, empty) == ALL
    aux = sp_tree_diff(all, empty);

    // It is a sp_tree:
    if (is_sp_tree(aux) == NO)        { return FAIL; }

    // It is empty:
    if (sp_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = sp_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = sp_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    sp_tree_remove_all(aux, NULL);

    // AUX == DIFFERENCE(empty, all) == EMPTY
    aux = sp_tree_diff(empty, all);

    // It is a sp_tree:
    if (is_sp_tree(aux) == NO)       { return FAIL; }

    // It is empty:
    if (sp_tree_is_empty(aux) == NO) { return FAIL; }

    // TEST SYMMETRIC DIFFERENCE: //////////////////////////////////////////////

    // AUX == SYMMETRIC_DIFFERENCE(all, odd) == EVEN
    aux = sp_tree_sym_diff(all, odd);

    // It is a sp_tree:
    if (is_sp_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (sp_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "even"...
    found = sp_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(even, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "even" is also in "aux".
    found = sp_tree_min(even);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(even, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(even);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    sp_tree_remove_all(aux, NULL);

    // AUX == SYMMETRIC_DIFFERENCE(odd, high)
    aux = sp_tree_sym_diff(odd, high);

    // It is a sp_tree:
    if (is_sp_tree(aux) == NO)        { return FAIL; }

    // It is not empty:
    if (sp_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "odd" or "high" but not in both.
    found = sp_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(odd, data);
        if (found == NULL) {
            found = sp_tree_search(high, data);
            if (found == NULL) { return FAIL; }
        } else {
            found = sp_tree_search(high, data);
            if (found != NULL) { return FAIL; }
        }
        found = sp_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    sp_tree_remove_all(aux, NULL);

    // AUX == SYMMETRIC_DIFFERENCE(all, empty) == ALL
    aux = sp_tree_sym_diff(all, empty);

    // It is a sp_tree:
    if (is_sp_tree(aux) == NO)        { return FAIL; }

    // It is empty:
    if (sp_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = sp_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = sp_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    sp_tree_remove_all(aux, NULL);

    // AUX == SYMMETRIC_DIFFERENCE(empty, all) == ALL
    aux = sp_tree_sym_diff(empty, all);

    // It is not empty:
    if (sp_tree_is_empty(aux) == YES) { return FAIL; }

    // Check if all data in "aux" is also in "all"...
    found = sp_tree_min(aux);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(all, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(aux, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(aux);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }
    
    // ...and all data in "all" is also in "aux".
    found = sp_tree_min(all);
    if (found == NULL) { return FAIL; }
    while (found != NULL) {
        data  = found;
        found = sp_tree_search(aux, data);
        if (found == NULL) { return FAIL; }
        found = sp_tree_next(all, data);
        if (found != NULL) {
            if (MyComp(data, found) >= 0) { return FAIL; }
        } else { 
            found = sp_tree_max(all);
            if (found == NULL)            { return FAIL; }
            if (MyComp(data, found) != 0) { return FAIL; }
            break;
        }
    }

    // Clean:
    sp_tree_remove_all(aux, NULL);

    // FINAL CLEAN UP:
    sp_tree_remove_all(aux, NULL);
    sp_tree_remove_all(odd, NULL);
    sp_tree_remove_all(low, NULL);
    sp_tree_remove_all(even, NULL);
    sp_tree_remove_all(high, NULL);
    sp_tree_remove_all(all, free);
    free(aux);
    free(odd);
    free(low);
    free(even);
    free(high);
    free(all);

    return PASS;
}

////////////////////////////////////////////////////////////////////////////////





// MAIN: ///////////////////////////////////////////////////////////////////////

int main () {

    // Test size:
    int max_size = 1001;//131063;// = 2^17 - 9, which is prime.

    // Timer:
    clock_t timer;

    // Init the random seed:
    srand(time(NULL));

    // BS_Testing:
    timer = clock();
    if      (bs_tree_sequential_test(max_size) == FAIL)      { printf("bs_tree_sequential_test FAILS\n\n"); }
    else if (bs_tree_fast_sequential_test(max_size) == FAIL) { printf("bs_tree_fast_sequential_test FAILS\n\n"); }
    else if (bs_tree_random_test(max_size) == FAIL)          { printf("bs_tree_random_test FAILS\n\n"); }
    else if (bs_tree_set_test(max_size) == FAIL)             { printf("bs_tree_set_test FAILS\n\n"); }
    else { printf("\nALL BS_TESTS PASSING in %.2f sec\n\n", ((double) (clock() - timer)) / CLOCKS_PER_SEC); }

    // RB_Testing:
    timer = clock();
    if      (rb_tree_sequential_test(max_size) == FAIL)      { printf("rb_tree_sequential_test FAILS\n\n"); }
    else if (rb_tree_fast_sequential_test(max_size) == FAIL) { printf("rb_tree_fast_sequential_test FAILS\n\n"); }
    else if (rb_tree_random_test(max_size) == FAIL)          { printf("rb_tree_random_test FAILS\n\n"); }
    else if (rb_tree_set_test(max_size) == FAIL)             { printf("rb_tree_set_test FAILS\n\n"); }
    else { printf("\nALL RB_TESTS PASSING in %.2f sec\n\n", ((double) (clock() - timer)) / CLOCKS_PER_SEC); }

    // SP_Testing:
    timer = clock();
    if      (sp_tree_sequential_test(max_size) == FAIL)      { printf("sp_tree_sequential_test FAILS\n\n"); }
    else if (sp_tree_fast_sequential_test(max_size) == FAIL) { printf("sp_tree_fast_sequential_test FAILS\n\n"); }
    else if (sp_tree_random_test(max_size) == FAIL)          { printf("sp_tree_random_test FAILS\n\n"); }
    else if (sp_tree_set_test(max_size) == FAIL)             { printf("sp_tree_set_test FAILS\n\n"); }
    else { printf("\nALL SP_TESTS PASSING in %.2f sec\n\n", ((double) (clock() - timer)) / CLOCKS_PER_SEC); }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
