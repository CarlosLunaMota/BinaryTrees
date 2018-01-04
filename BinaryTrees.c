////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//      BinaryTrees.c                                                         //
//      -------------                                                         //
//                                                                            //
// Content: The methods of the BinaryTrees library.                           //
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

#include <stdlib.h>         // malloc, free
#include <assert.h>         // assert
#include <stdio.h>          // fprintf, fflush, sprintf, stderr, stdout
#include <string.h>         // strlen
#include "BinaryTrees.h"    // BinaryTrees library headers

////////////////////////////////////////////////////////////////////////////////



// BINARY SEARCH TREES /////////////////////////////////////////////////////////


// CREATION & INSERTION:

// Returns a pointer to a newly created bs_tree.
// You must provide a comparing function "comp" such that:
//  * comp(A,B) = 0  if and only if  A = B
//  * comp(A,B) > 0  if and only if  A > B
//  * comp(A,B) < 0  if and only if  A < B
//
// I recommend to limit the output of the "comp" function to {-1, 0, 1} as in:
//
//  int MyComp(const void *ptr1, const void *ptr2) {
//      MyData *d1 = (MyData *) ptr1;
//      MyData *d2 = (MyData *) ptr2;
//      if      (d1->key < d2->key) { return -1; }
//      else if (d1->key > d2->key) { return +1; }
//      else                        { return  0; }
//  }
//
// However, the code does not assume that so, if you are working with simple
// numeric keys, you can just use something like:
//
//  int MyComp(const void *ptr1, const void *ptr2) {
//      MyData *d1 = (MyData *) ptr1;
//      MyData *d2 = (MyData *) ptr2;
//      return (int) ((d1->key) - (d2->key));
//  }
//
bs_tree *new_bs_tree(int (* comp) (const void *, const void *)) {

    // Sanity check:
    assert(comp != NULL);

    // Allocate memory:
    bs_tree *tree = (bs_tree *) malloc(sizeof(bs_tree));
    if (tree == NULL) {
        fprintf(stderr, "ERROR: Unable to allocate memory for bs_tree\n");
    }

    // Initialize the empty tree:
    else {
        tree->root = NULL;
        tree->comp = comp;
    }

    return tree;
}

// Returns a (really degenerated) binary tree containing a copy of tree.
//
// Since it takes O(|tree|) to build the new tree you can perform:
//
//      bs_tree_rebalance(new_tree);
//
// afterwards "for free", but I prefer to return the degenerate tree because
// it will speed up a little bit future aplications of other "bulk functions".
// Please, take a moment to decide if you want to post-process the tree with
// a total rebalance operation.
//
bs_tree *bs_tree_copy(const bs_tree *tree) {

    bs_tree *new_tree = NULL;
    bs_node *new_node = NULL;
    bs_node *pred;
    bs_node *node;
    
    // Sanity check:
    assert(tree != NULL);
    
    // Create a new tree:
    new_tree = new_bs_tree(tree->comp);
    if (new_tree == NULL) { return NULL; }

    // Go to the smallest element of tree while right-threading your path:
    node = tree->root;
    if (node != NULL) {
        while (node->left != NULL) {
            pred = node->left;
            while (pred->right != NULL) { pred = pred->right; }
            pred->right = node;
            node        = node->left;
        }
    }

    // Insert all data from tree into new_tree:
    while (node != NULL) {

        // insert node->data in new_tree ///////////////////////////////////////
        if (new_node == NULL)  {
            new_tree->root = (bs_node *) malloc(sizeof(bs_node));
            new_node = new_tree->root;
        } else {
            new_node->right = (bs_node *) malloc(sizeof(bs_node));
            new_node = new_node->right;
        }
        if (new_node == NULL) {
            fprintf(stderr, "ERROR: Unable to allocate bs_node\n");
            return new_tree;
        } else {
            new_node->data  = node->data;
            new_node->left  = NULL;
            new_node->right = NULL;
        }
        ////////////////////////////////////////////////////////////////////////

        // advance node ////////////////////////////////////////////////////////
        node = node->right;
        if (node != NULL) {
            while (node->left != NULL) {
                pred = node->left;
                while (pred->right != NULL && pred->right != node) {
                    pred = pred->right;
                }
                if (pred->right == NULL) {
                    pred->right = node;
                    node        = node->left;
                } else {
                    pred->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }

    // Return the resulting tree:
    return new_tree;
}

// Inserts data in tree.
//
// If a node of the tree compares "equal" to data it will get replaced and a
// pointer to the previously stored data will be returned (so you can free it),
// otherwise it will simply return a NULL pointer.
//
void *bs_tree_insert(bs_tree *tree, void *data) {

    bs_node *node;
    bs_node *new_node;
    void    *old_data;
    int      comp;

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Avoid the trivial case: empty tree
    node = NULL;
    if (tree->root != NULL) {

        // Search for the correct place to insert data:
        node = tree->root;
        for (;;) {

            // Compare data:
            comp = (tree->comp)(data, node->data);

            // Data is smaller:
            if (comp < 0) {
                if (node->left == NULL) { break; }
                else { node = node->left; }
            }

            // Data is bigger:
            else if (comp > 0) {
                if (node->right == NULL) { break; }
                else { node = node->right; }
            }

            // Data is already there: overwrite it!
            else {
                old_data   = node->data;
                node->data = data;
                return old_data;
            }
        }
    }

    // Insert the new node here:
    new_node = (bs_node *) malloc(sizeof(bs_node));
    if (new_node == NULL) {
        fprintf(stderr, "ERROR: Unable to allocate bs_node\n");
    } else {
        new_node->data  = data;
        new_node->left  = NULL;
        new_node->right = NULL;
    }

    if (node == NULL)  { tree->root  = new_node; }
    else if (comp < 0) { node->left  = new_node; }
    else               { node->right = new_node; }

    // Data was not here!
    return NULL;
}

// Inserts "data" such that "data" is smaller or equal to any other "data"
// already in the tree. It is slightly faster than a regular insert because it
// makes at most one comparison.
//
// WARNING: If you use this function to insert data that is strictly bigger than
//          something already in the tree you will break the tree!
//
// If a node of the tree compares "equal" to data it will get replaced and a
// pointer to the previously stored data will be returned (so you can free it),
// otherwise it will simply return a NULL pointer.
//
void *bs_tree_insert_min(bs_tree *tree, void *data) {

    bs_node *node;
    bs_node *new_node;
    void    *old_data;

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Trivial case:
    if (tree->root == NULL) { node = NULL; }

    // General case:
    else {
        node = tree->root;
        while (node->left != NULL) { node = node->left; }

        // If "data" is already there: overwrite it & return!
        if ((tree->comp)(data, node->data) == 0) {
            old_data   = node->data;
            node->data = data;
            return old_data;
        }
    }

    // Finally: Insert the new node here
    new_node = (bs_node *) malloc(sizeof(bs_node));
    if (new_node == NULL) {
        fprintf(stderr, "ERROR: Unable to allocate bs_node\n");
    } else {
        new_node->data  = data;
        new_node->left  = NULL;
        new_node->right = NULL;
    }
    if (node == NULL) { tree->root = new_node; }
    else              { node->left = new_node; }

    // Data was not here!
    return NULL;
}

// Inserts "data" such that "data" is bigger or equal to any other "data"
// already in the tree. It is slightly faster than a regular insert because it
// makes at most one comparison.
//
// WARNING: If you use this function to insert data that is strictly smaller
//          than something already in the tree you will break the tree!
//
// If a node of the tree compares "equal" to data it will get replaced and a
// pointer to the previously stored data will be returned (so you can free it),
// otherwise it will simply return a NULL pointer.
//
void *bs_tree_insert_max(bs_tree *tree, void *data) {

    bs_node *node;
    bs_node *new_node;
    void    *old_data;

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Trivial case:
    if (tree->root == NULL) { node = NULL; }

    // General case:
    else {
        node = tree->root;
        while (node->right != NULL) { node = node->right; }

        // If "data" is already there: overwrite it & return!
        if ((tree->comp)(data, node->data) == 0) {
            old_data   = node->data;
            node->data = data;
            return old_data;
        }
    }

    // Finally: Insert the new node here
    new_node = (bs_node *) malloc(sizeof(bs_node));
    if (new_node == NULL) {
        fprintf(stderr, "ERROR: Unable to allocate bs_node\n");
    } else {
        new_node->data  = data;
        new_node->left  = NULL;
        new_node->right = NULL;
    }
    if (node == NULL) { tree->root  = new_node; }
    else              { node->right = new_node; }

    // Data was not here!
    return NULL;
}



// SEARCH:

// Returns YES if the tree is empty and NO otherwise.
//
int bs_tree_is_empty(const bs_tree *tree) {

    // Sanity check:
    assert(tree != NULL);

    // Check if there is at least 1 node in the tree:
    if (tree->root == NULL) { return YES; }
    else                    { return NO;  }
}

// Finds a node that compares "equal" to data. Returns NULL if not found.
//
void *bs_tree_search(const bs_tree *tree, const void *data) {

    bs_node *node;
    int      comp;

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Search:
    node = tree->root;
    while (node != NULL) {
        comp = (tree->comp)(data, node->data);      // compare data
        if      (comp < 0) { node = node->left;  }  // data is smaller
        else if (comp > 0) { node = node->right; }  // data is bigger
        else               { return node->data;  }  // found!
    }

    // Not found:
    return NULL;
}

// Returns a pointer to the smallest element stored in the tree.
// Returns NULL if the tree is empty.
//
void *bs_tree_min(const bs_tree *tree) {

    bs_node *node;

    // Sanity check:
    assert(tree != NULL);

    // Trivial case: empty tree
    if (tree->root == NULL) { return NULL; }

    // General case: Find the smallest node
    node = tree->root;
    while (node->left != NULL) { node = node->left; }

    // Return a pointer to the data:
    return node->data;
}

// Returns a pointer to the biggest element stored in "tree".
// Returns NULL if the tree is empty.
//
void *bs_tree_max(const bs_tree *tree) {

    bs_node *node;

    // Sanity check:
    assert(tree != NULL);

    // Trivial case: empty tree
    if (tree->root == NULL) { return NULL; }

    // General case: Find the biggest node
    node = tree->root;
    while (node->right != NULL) { node = node->right; }

    // Return a pointer to the data:
    return node->data;
}

// Find the in-order predesor of data in the tree.
//
// If data is not in tree returns the biggest element of tree smaller than data.
// If data is smaller or equal to all elements of tree returns NULL.
//
void *bs_tree_prev(const bs_tree *tree, const void *data) {

    bs_node *node;
    bs_node *pred;
    int      comp;

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Search:
    pred = NULL;
    node = tree->root;
    while (node != NULL) {

        // Compare data:
        comp = (tree->comp)(data, node->data);

        // Data is smaller:
        if (comp < 0) { node = node->left; }

        // Data is bigger:
        else if (comp > 0) {
            pred = node;
            node = node->right;
        }

        // We have found the node:
        else if (node->left != NULL) {
            pred = node->left;
            while (pred->right != NULL) { pred = pred->right; }
            break;
        } else { break; }
    }

    // Return predecesor:
    if (pred == NULL) { return NULL; }
    return pred->data;
}

// Find the in-order successor of data in the tree.
//
// If data is not in tree returns the smallest element of tree bigger than data.
// If data is bigger or equal to all elements of tree returns NULL.
//
void *bs_tree_next(const bs_tree *tree, const void *data) {

    bs_node *node;
    bs_node *succ;
    int      comp;

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Search:
    succ = NULL;
    node = tree->root;
    while (node != NULL) {

        // Compare data:
        comp = (tree->comp)(data, node->data);

        // Data is smaller:
        if (comp < 0) {
            succ = node;
            node = node->left;
        }

        // Data is bigger:
        else if (comp > 0) { node = node->right; }

        // We have found the node:
        else if (node->right != NULL) {
            succ = node->right;
            while (succ->left != NULL) { succ = succ->left; }
            break;
        } else { break; }
    }

    // Return succesor:
    if (succ == NULL) { return NULL; }
    return succ->data;
}



// REMOVE:

// Removes a node of tree that compares "equal" to data and returns a pointer
// to the previously stored data (so you can free it).
// If such a node is not found, it returns a NULL pointer.
//
// Uses the trick of swapping node->data and successor->data pointers and then
// removes successor. This is safe because the final user has no acces to any
// tree node (only "tree" and "data" pointers are used in all interfaces) and
// no traverser is coded either (so no "internal state" is stored in them).
//
void *bs_tree_remove(bs_tree *tree, const void *data) {

    bs_node *parent;
    bs_node *node;
    bs_node *old_node;
    void    *old_data;
    int      comp;

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Search the node to delete:
    parent   = NULL;
    node     = tree->root;
    while (node != NULL) {

        // Compare data:
        comp = (tree->comp)(data, node->data);

        // Data is smaller:
        if (comp < 0) {
            parent = node;
            node   = node->left;
        }

        // Data is bigger:
        else if (comp > 0) {
            parent = node;
            node   = node->right;
        }

        // We have found the node:
        else {

            old_data = node->data;

            // If node has both subtrees we must do some extra work:
            if (node->left != NULL && node->right != NULL) {

                // Remember the place and keep going down to find the successor
                old_node = node;                // Here we are looking for the
                parent   = node;                // inorder successor of node
                node     = node->right;         // but if we exchange left for
                while (node->left != NULL) {    // right (and viceversa) on
                    parent = node;              // thisblock of code we could
                    node   = node->left;        // implement the equaly valid
                }                               // predecesor version of remove

                // Once we find the "succesor" of "old_node"
                // we just move the data pointer and remove
                // the current "node":
                old_node->data = node->data;
            }

            // At this point we are sure that "node" has, at least,
            // one empty subtree, which greatly simplifies the removal:

            // Easy case 1: node->left is NULL
            if (node->left == NULL) {
                if      (parent       == NULL) { tree->root    = node->right; }
                else if (parent->left == node) { parent->left  = node->right; }
                else                           { parent->right = node->right; }
                free(node);
                return old_data;
            }

            // Easy case 2: node->right is NULL
            else {
                if      (parent       == NULL) { tree->root    = node->left; }
                else if (parent->left == node) { parent->left  = node->left; }
                else                           { parent->right = node->left; }
                free(node);
                return old_data;
            }
        }
    }
    return NULL;
}

// Removes the smallest element from "tree" and returns a pointer to its data
// (so you can free it). If the tree is empty returns a NULL pointer.
//
// More efficient than "tree_remove(tree, min_elem)" even if you already know
// the value of "min_elem".
//
void *bs_tree_remove_min(bs_tree *tree) {

    bs_node *node;
    bs_node *parent;
    void    *old_data;

    // Sanity check:
    assert(tree != NULL);

    // Avoid trivial case: empty tree
    old_data = NULL;
    if (tree->root != NULL) {

        // General case: Find the smallest node
        parent = NULL;
        node   = tree->root;
        while (node->left != NULL) {
            parent = node;
            node   = node->left;
        }
        old_data = node->data;

        // Remove Node:
        if (parent != NULL) { parent->left = node->right; }
        else                { tree->root   = node->right; }
        free(node);
    }
    return old_data;
}

// Removes the biggest element from "tree" and returns a pointer to its data
// (so you can free it). If the tree is empty returns a NULL pointer.
//
// More efficient than "tree_remove(tree, max_data)" even if you already know
// the value of "max_data".
//
void *bs_tree_remove_max(bs_tree *tree) {

    bs_node *node;
    bs_node *parent;
    void        *old_data;

    // Sanity check:
    assert(tree != NULL);

    // Avoid trivial case: empty tree
    old_data = NULL;
    if (tree->root != NULL) {

        // General case: Find the biggest node
        parent = NULL;
        node   = tree->root;
        while (node->right != NULL) {
            parent = node;
            node   = node->right;
        }
        old_data = node->data;

        // Remove Node:
        if (parent != NULL) { parent->right = node->left; }
        else                { tree->root    = node->left; }
        free(node);
    }
    return old_data;
}

// Removes all the elements from the tree in linear time. This is faster than
// calling "tree_remove_min(tree)" until "tree_is_empty(tree)" returns "YES".
//
// If you provide a "free_data" function it will be used to free the "data"
// inside each node. If "free_data" is NULL, no "data" will be freed. Use this
// later option when "data" is shared between several data structures but be
// aware that this may cause some memory leaks if you are not carefull.
//
// Most of the time, you can use just "free" as "free_data" function.
// However if your "data" struct contains dinamically allocated data
// you may need to provide a more complex "free_data" function like:
//
//  void free_data(void *ptr) {
//      MyData *data = (MyData *) ptr;
//      free(data->some_array);
//      free(data->some_other_array);
//      free(data);
//  }
//
void bs_tree_remove_all(bs_tree *tree, void (* free_data) (void *)) {

    bs_node *root;
    bs_node *left;
    bs_node *right;

    // Sanity check:
    assert(tree != NULL);

    // Initialize:
    root = tree->root;
    tree->root = NULL;

    // While the tree is not empty:
    while (root != NULL) {

        // Unravel the tree: Rotate right "root" & "left"
        if (root->left != NULL) {
            left        = root->left;
            right       = left->right;
            left->right = root;
            root->left  = right;
            root        = left;

        // Erase the current "root" node:
        } else {
            right = root->right;
            if (free_data != NULL) { free_data(root->data); }
            free(root);
            root = right;
        }
    }
}



// SET FUNCTIONS:

// Returns a (really degenerated) binary tree containing a copy of the union of
// tree_1 and tree_2. It does NOT modify tree_1 or tree_2.
//
// If a given "element" is in both trees it takes the pointer from tree_1.
// Likewise, the new tree stores a pointer to the comparing function of tree_1.
//
// Since it takes O(|tree_1| + |tree_2|) to build the new tree you can perform:
//
//      bs_tree_rebalance(tree);
//
// afterwards "for free", but I prefer to return the degenerate tree because
// it will speed up a little bit future aplications of other "set functions".
// Please, take a moment to decide if you want to post-process the tree with
// a total rebalance operation.
//
bs_tree *bs_tree_union(const bs_tree *tree_1, const bs_tree *tree_2) {

    bs_tree *tree = NULL;
    bs_node *node = NULL;
    bs_node *pred_1;
    bs_node *node_1;
    bs_node *pred_2;
    bs_node *node_2;
    int      comp;

    // Sanity check:
    assert(tree_1 != NULL);
    assert(tree_2 != NULL);

    // Special case: Both trees are the same
    if (tree_1 == tree_2) { return bs_tree_copy(tree_1); }

    // Create a new tree:
    tree = new_bs_tree(tree_1->comp);
    if (tree == NULL) { return NULL; }

    // Go to the smallest element of tree_1 while right-threading your path:
    node_1 = tree_1->root;
    if (node_1 != NULL) {
        while (node_1->left != NULL) {
            pred_1 = node_1->left;
            while (pred_1->right != NULL) { pred_1 = pred_1->right; }
            pred_1->right = node_1;
            node_1        = node_1->left;
        }
    }

    // Go to the smallest element of tree_2 while right-threading your path:
    node_2 = tree_2->root;
    if (node_2 != NULL) {
        while (node_2->left != NULL) {
            pred_2 = node_2->left;
            while (pred_2->right != NULL) { pred_2 = pred_2->right; }
            pred_2->right = node_2;
            node_2        = node_2->left;
        }
    }

    // Until we have exhausted at least one of the trees:
    while (node_1 != NULL && node_2 != NULL) {

        // compare both nodes:
        comp = (tree->comp)(node_1->data, node_2->data);

        if (comp < 0) {

            // insert node_1->data in tree /////////////////////////////////////
            if (node == NULL)  {
                tree->root = (bs_node *) malloc(sizeof(bs_node));
                node = tree->root;
            } else {
                node->right = (bs_node *) malloc(sizeof(bs_node));
                node = node->right;
            }
            if (node == NULL) {
                fprintf(stderr, "ERROR: Unable to allocate bs_node\n");
                return tree;
            } else {
                node->data  = node_1->data;
                node->left  = NULL;
                node->right = NULL;
            }
            ////////////////////////////////////////////////////////////////////

            // advance node_1 //////////////////////////////////////////////////
            node_1 = node_1->right;
            if (node_1 != NULL) {
                while (node_1->left != NULL) {
                    pred_1 = node_1->left;
                    while (pred_1->right != NULL && pred_1->right != node_1) {
                        pred_1 = pred_1->right;
                    }
                    if (pred_1->right == NULL) {
                        pred_1->right = node_1;
                        node_1        = node_1->left;
                    } else {
                        pred_1->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

        } else if (comp > 0) {

            // insert node_2->data in tree /////////////////////////////////////
            if (node == NULL)  {
                tree->root = (bs_node *) malloc(sizeof(bs_node));
                node = tree->root;
            } else {
                node->right = (bs_node *) malloc(sizeof(bs_node));
                node = node->right;
            }
            if (node == NULL) {
                fprintf(stderr, "ERROR: Unable to allocate bs_node\n");
                return tree;
            } else {
                node->data  = node_2->data;
                node->left  = NULL;
                node->right = NULL;
            }
            ////////////////////////////////////////////////////////////////////

            // advance node_2 //////////////////////////////////////////////////
            node_2 = node_2->right;
            if (node_2 != NULL) {
                while (node_2->left != NULL) {
                    pred_2 = node_2->left;
                    while (pred_2->right != NULL && pred_2->right != node_2) {
                        pred_2 = pred_2->right;
                    }
                    if (pred_2->right == NULL) {
                        pred_2->right = node_2;
                        node_2        = node_2->left;
                    } else {
                        pred_2->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

        } else {

            // insert node_1->data in tree /////////////////////////////////////
            if (node == NULL)  {
                tree->root = (bs_node *) malloc(sizeof(bs_node));
                node = tree->root;
            } else {
                node->right = (bs_node *) malloc(sizeof(bs_node));
                node = node->right;
            }
            if (node == NULL) {
                fprintf(stderr, "ERROR: Unable to allocate bs_node\n");
                return tree;
            } else {
                node->data  = node_1->data;
                node->left  = NULL;
                node->right = NULL;
            }
            ////////////////////////////////////////////////////////////////////

            // advance node_1 //////////////////////////////////////////////////
            node_1 = node_1->right;
            if (node_1 != NULL) {
                while (node_1->left != NULL) {
                    pred_1 = node_1->left;
                    while (pred_1->right != NULL && pred_1->right != node_1) {
                        pred_1 = pred_1->right;
                    }
                    if (pred_1->right == NULL) {
                        pred_1->right = node_1;
                        node_1        = node_1->left;
                    } else {
                        pred_1->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

            // advance node_2 //////////////////////////////////////////////////
            node_2 = node_2->right;
            if (node_2 != NULL) {
                while (node_2->left != NULL) {
                    pred_2 = node_2->left;
                    while (pred_2->right != NULL && pred_2->right != node_2) {
                        pred_2 = pred_2->right;
                    }
                    if (pred_2->right == NULL) {
                        pred_2->right = node_2;
                        node_2        = node_2->left;
                    } else {
                        pred_2->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////
        }
    }

    // Insert all remaining data from tree_1:
    while (node_1 != NULL) {

        // insert node_1->data in tree /////////////////////////////////////////
        if (node == NULL)  {
            tree->root = (bs_node *) malloc(sizeof(bs_node));
            node = tree->root;
        } else {
            node->right = (bs_node *) malloc(sizeof(bs_node));
            node = node->right;
        }
        if (node == NULL) {
            fprintf(stderr, "ERROR: Unable to allocate bs_node\n");
            return tree;
        } else {
            node->data  = node_1->data;
            node->left  = NULL;
            node->right = NULL;
        }
        ////////////////////////////////////////////////////////////////////////

        // advance node_1 //////////////////////////////////////////////////////
        node_1 = node_1->right;
        if (node_1 != NULL) {
            while (node_1->left != NULL) {
                pred_1 = node_1->left;
                while (pred_1->right != NULL && pred_1->right != node_1) {
                    pred_1 = pred_1->right;
                }
                if (pred_1->right == NULL) {
                    pred_1->right = node_1;
                    node_1        = node_1->left;
                } else {
                    pred_1->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }

    // Insert all remaining data from tree_2:
    while (node_2 != NULL) {

        // insert node_2->data in tree /////////////////////////////////////////
        if (node == NULL)  {
            tree->root = (bs_node *) malloc(sizeof(bs_node));
            node = tree->root;
        } else {
            node->right = (bs_node *) malloc(sizeof(bs_node));
            node = node->right;
        }
        if (node == NULL) {
            fprintf(stderr, "ERROR: Unable to allocate bs_node\n");
            return tree;
        } else {
            node->data  = node_2->data;
            node->left  = NULL;
            node->right = NULL;
        }
        ////////////////////////////////////////////////////////////////////////

        // advance node_2 //////////////////////////////////////////////////////
        node_2 = node_2->right;
        if (node_2 != NULL) {
            while (node_2->left != NULL) {
                pred_2 = node_2->left;
                while (pred_2->right != NULL && pred_2->right != node_2) {
                    pred_2 = pred_2->right;
                }
                if (pred_2->right == NULL) {
                    pred_2->right = node_2;
                    node_2        = node_2->left;
                } else {
                    pred_2->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }

    // Return the resulting tree:
    return tree;
}

// Returns a (really degenerated) binary tree containing a copy of the
// intersection of tree_1 and tree_2. It does NOT modify tree_1 or tree_2.
//
// The comparing function and all data pointers are taken from tree_1.
//
// Since it takes O(|tree_1| + |tree_2|) to build the new tree you can perform:
//
//      bs_tree_rebalance(tree);
//
// afterwards "for free", but I prefer to return the degenerate tree because
// it will speed up a little bit future aplications of other "set functions".
// Please, take a moment to decide if you want to post-process the tree with
// a total rebalance operation.
//
bs_tree *bs_tree_intersection(const bs_tree *tree_1, const bs_tree *tree_2) {

    bs_tree *tree = NULL;
    bs_node *node = NULL;
    bs_node *pred_1;
    bs_node *node_1;
    bs_node *pred_2;
    bs_node *node_2;
    int      comp;

    // Sanity check:
    assert(tree_1 != NULL);
    assert(tree_2 != NULL);

    // Special case: Both trees are the same
    if (tree_1 == tree_2) { return bs_tree_copy(tree_1); }

    // Create a new tree:
    tree = new_bs_tree(tree_1->comp);
    if (tree == NULL) { return NULL; }

    // Special case: Some of them is empty
    if (bs_tree_is_empty(tree_1) == YES) { return tree; }
    if (bs_tree_is_empty(tree_2) == YES) { return tree; }

    // Go to the smallest element of tree_1 while right-threading your path:
    node_1 = tree_1->root;
    if (node_1 != NULL) {
        while (node_1->left != NULL) {
            pred_1 = node_1->left;
            while (pred_1->right != NULL) { pred_1 = pred_1->right; }
            pred_1->right = node_1;
            node_1        = node_1->left;
        }
    }

    // Go to the smallest element of tree_2 while right-threading your path:
    node_2 = tree_2->root;
    if (node_2 != NULL) {
        while (node_2->left != NULL) {
            pred_2 = node_2->left;
            while (pred_2->right != NULL) { pred_2 = pred_2->right; }
            pred_2->right = node_2;
            node_2        = node_2->left;
        }
    }

    // Until we have exhausted at least one of the trees:
    while (node_1 != NULL && node_2 != NULL) {

        // compare both nodes:
        comp = (tree->comp)(node_1->data, node_2->data);

        if (comp < 0) {

            // advance node_1 //////////////////////////////////////////////////
            node_1 = node_1->right;
            if (node_1 != NULL) {
                while (node_1->left != NULL) {
                    pred_1 = node_1->left;
                    while (pred_1->right != NULL && pred_1->right != node_1) {
                        pred_1 = pred_1->right;
                    }
                    if (pred_1->right == NULL) {
                        pred_1->right = node_1;
                        node_1        = node_1->left;
                    } else {
                        pred_1->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

        } else if (comp > 0) {

            // advance node_2 //////////////////////////////////////////////////
            node_2 = node_2->right;
            if (node_2 != NULL) {
                while (node_2->left != NULL) {
                    pred_2 = node_2->left;
                    while (pred_2->right != NULL && pred_2->right != node_2) {
                        pred_2 = pred_2->right;
                    }
                    if (pred_2->right == NULL) {
                        pred_2->right = node_2;
                        node_2        = node_2->left;
                    } else {
                        pred_2->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

        } else {

            // insert node_1->data in tree /////////////////////////////////////
            if (node == NULL)  {
                tree->root = (bs_node *) malloc(sizeof(bs_node));
                node = tree->root;
            } else {
                node->right = (bs_node *) malloc(sizeof(bs_node));
                node = node->right;
            }
            if (node == NULL) {
                fprintf(stderr, "ERROR: Unable to allocate bs_node\n");
                return tree;
            } else {
                node->data  = node_1->data;
                node->left  = NULL;
                node->right = NULL;
            }
            ////////////////////////////////////////////////////////////////////

            // advance node_1 //////////////////////////////////////////////////
            node_1 = node_1->right;
            if (node_1 != NULL) {
                while (node_1->left != NULL) {
                    pred_1 = node_1->left;
                    while (pred_1->right != NULL && pred_1->right != node_1) {
                        pred_1 = pred_1->right;
                    }
                    if (pred_1->right == NULL) {
                        pred_1->right = node_1;
                        node_1        = node_1->left;
                    } else {
                        pred_1->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

            // advance node_2 //////////////////////////////////////////////////
            node_2 = node_2->right;
            if (node_2 != NULL) {
                while (node_2->left != NULL) {
                    pred_2 = node_2->left;
                    while (pred_2->right != NULL && pred_2->right != node_2) {
                        pred_2 = pred_2->right;
                    }
                    if (pred_2->right == NULL) {
                        pred_2->right = node_2;
                        node_2        = node_2->left;
                    } else {
                        pred_2->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////
        }
    }

    // We need to traverse tree_1 completely to restore the modified pointers:
    while (node_1 != NULL) {

        // advance node_1 //////////////////////////////////////////////////////
        node_1 = node_1->right;
        if (node_1 != NULL) {
            while (node_1->left != NULL) {
                pred_1 = node_1->left;
                while (pred_1->right != NULL && pred_1->right != node_1) {
                    pred_1 = pred_1->right;
                }
                if (pred_1->right == NULL) {
                    pred_1->right = node_1;
                    node_1        = node_1->left;
                } else {
                    pred_1->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }

    // We need to traverse tree_2 completely to restore the modified pointers:
    while (node_2 != NULL) {

        // advance node_2 //////////////////////////////////////////////////////
        node_2 = node_2->right;
        if (node_2 != NULL) {
            while (node_2->left != NULL) {
                pred_2 = node_2->left;
                while (pred_2->right != NULL && pred_2->right != node_2) {
                    pred_2 = pred_2->right;
                }
                if (pred_2->right == NULL) {
                    pred_2->right = node_2;
                    node_2        = node_2->left;
                } else {
                    pred_2->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }

    // Return the resulting tree:
    return tree;
}

// Returns a (really degenerated) binary tree containing a copy of the
// difference: tree_1 - tree_2. It does NOT modify tree_1 or tree_2.
//
// The comparing function and all data pointers are taken from tree_1.
//
// Since it takes O(|tree_1| + |tree_2|) to build the new tree you can perform:
//
//      bs_tree_rebalance(tree);
//
// afterwards "for free", but I prefer to return the degenerate tree because
// it will speed up a little bit future aplications of other "set functions".
// Please, take a moment to decide if you want to post-process the tree with
// a total rebalance operation.
//
bs_tree *bs_tree_diff(const bs_tree *tree_1, const bs_tree *tree_2) {

    bs_tree *tree = NULL;
    bs_node *node = NULL;
    bs_node *pred_1;
    bs_node *node_1;
    bs_node *pred_2;
    bs_node *node_2;
    int      comp;

    // Sanity check:
    assert(tree_1 != NULL);
    assert(tree_2 != NULL);

    // Create a new tree:
    tree = new_bs_tree(tree_1->comp);
    if (tree == NULL) { return NULL; }

    // Special case: Both trees are the same
    if (tree_1 == tree_2) { return tree; }

    // Special case: Some of them is empty
    if (bs_tree_is_empty(tree_1) == YES) { return tree; }

    // Go to the smallest element of tree_1 while right-threading your path:
    node_1 = tree_1->root;
    if (node_1 != NULL) {
        while (node_1->left != NULL) {
            pred_1 = node_1->left;
            while (pred_1->right != NULL) { pred_1 = pred_1->right; }
            pred_1->right = node_1;
            node_1        = node_1->left;
        }
    }

    // Go to the smallest element of tree_2 while right-threading your path:
    node_2 = tree_2->root;
    if (node_2 != NULL) {
        while (node_2->left != NULL) {
            pred_2 = node_2->left;
            while (pred_2->right != NULL) { pred_2 = pred_2->right; }
            pred_2->right = node_2;
            node_2        = node_2->left;
        }
    }

    // Until we have exhausted at least one of the trees:
    while (node_1 != NULL && node_2 != NULL) {

        // compare both nodes:
        comp = (tree->comp)(node_1->data, node_2->data);

        if (comp < 0) {

            // insert node_1->data in tree /////////////////////////////////////
            if (node == NULL)  {
                tree->root = (bs_node *) malloc(sizeof(bs_node));
                node = tree->root;
            } else {
                node->right = (bs_node *) malloc(sizeof(bs_node));
                node = node->right;
            }
            if (node == NULL) {
                fprintf(stderr, "ERROR: Unable to allocate bs_node\n");
                return tree;
            } else {
                node->data  = node_1->data;
                node->left  = NULL;
                node->right = NULL;
            }
            ////////////////////////////////////////////////////////////////////

            // advance node_1 //////////////////////////////////////////////////
            node_1 = node_1->right;
            if (node_1 != NULL) {
                while (node_1->left != NULL) {
                    pred_1 = node_1->left;
                    while (pred_1->right != NULL && pred_1->right != node_1) {
                        pred_1 = pred_1->right;
                    }
                    if (pred_1->right == NULL) {
                        pred_1->right = node_1;
                        node_1        = node_1->left;
                    } else {
                        pred_1->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

        } else if (comp > 0) {

            // advance node_2 //////////////////////////////////////////////////
            node_2 = node_2->right;
            if (node_2 != NULL) {
                while (node_2->left != NULL) {
                    pred_2 = node_2->left;
                    while (pred_2->right != NULL && pred_2->right != node_2) {
                        pred_2 = pred_2->right;
                    }
                    if (pred_2->right == NULL) {
                        pred_2->right = node_2;
                        node_2        = node_2->left;
                    } else {
                        pred_2->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

        } else {

            // advance node_1 //////////////////////////////////////////////////
            node_1 = node_1->right;
            if (node_1 != NULL) {
                while (node_1->left != NULL) {
                    pred_1 = node_1->left;
                    while (pred_1->right != NULL && pred_1->right != node_1) {
                        pred_1 = pred_1->right;
                    }
                    if (pred_1->right == NULL) {
                        pred_1->right = node_1;
                        node_1        = node_1->left;
                    } else {
                        pred_1->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

            // advance node_2 //////////////////////////////////////////////////
            node_2 = node_2->right;
            if (node_2 != NULL) {
                while (node_2->left != NULL) {
                    pred_2 = node_2->left;
                    while (pred_2->right != NULL && pred_2->right != node_2) {
                        pred_2 = pred_2->right;
                    }
                    if (pred_2->right == NULL) {
                        pred_2->right = node_2;
                        node_2        = node_2->left;
                    } else {
                        pred_2->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////
        }
    }

    // Insert all remaining data from tree_1:
    while (node_1 != NULL) {

        // insert node_1->data in tree /////////////////////////////////////////
        if (node == NULL)  {
            tree->root = (bs_node *) malloc(sizeof(bs_node));
            node = tree->root;
        } else {
            node->right = (bs_node *) malloc(sizeof(bs_node));
            node = node->right;
        }
        if (node == NULL) {
            fprintf(stderr, "ERROR: Unable to allocate bs_node\n");
            return tree;
        } else {
            node->data  = node_1->data;
            node->left  = NULL;
            node->right = NULL;
        }
        ////////////////////////////////////////////////////////////////////////

        // advance node_1 //////////////////////////////////////////////////////
        node_1 = node_1->right;
        if (node_1 != NULL) {
            while (node_1->left != NULL) {
                pred_1 = node_1->left;
                while (pred_1->right != NULL && pred_1->right != node_1) {
                    pred_1 = pred_1->right;
                }
                if (pred_1->right == NULL) {
                    pred_1->right = node_1;
                    node_1        = node_1->left;
                } else {
                    pred_1->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }

    // We need to traverse tree_2 completely to restore the modified pointers:
    while (node_2 != NULL) {

        // advance node_2 //////////////////////////////////////////////////////
        node_2 = node_2->right;
        if (node_2 != NULL) {
            while (node_2->left != NULL) {
                pred_2 = node_2->left;
                while (pred_2->right != NULL && pred_2->right != node_2) {
                    pred_2 = pred_2->right;
                }
                if (pred_2->right == NULL) {
                    pred_2->right = node_2;
                    node_2        = node_2->left;
                } else {
                    pred_2->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }

    // Return the resulting tree:
    return tree;
}

// Returns a (really degenerated) binary tree containing a copy of the symmetric
// difference of tree_1 and tree_2. It does NOT modify tree_1 or tree_2.
//
// The comparing function is taken from tree_1.
//
// Since it takes O(|tree_1| + |tree_2|) to build the new tree you can perform:
//
//      bs_tree_rebalance(tree);
//
// afterwards "for free", but I prefer to return the degenerate tree because
// it will speed up a little bit future aplications of other "set functions".
// Please, take a moment to decide if you want to post-process the tree with
// a total rebalance operation.
//
bs_tree *bs_tree_sym_diff(const bs_tree *tree_1, const bs_tree *tree_2) {

    bs_tree *tree = NULL;
    bs_node *node = NULL;
    bs_node *pred_1;
    bs_node *node_1;
    bs_node *pred_2;
    bs_node *node_2;
    int      comp;

    // Sanity check:
    assert(tree_1 != NULL);
    assert(tree_2 != NULL);

    // Create a new tree:
    tree = new_bs_tree(tree_1->comp);
    if (tree == NULL) { return NULL; }

    // Special case: Both trees are the same
    if (tree_1 == tree_2) { return tree; }

    // Go to the smallest element of tree_1 while right-threading your path:
    node_1 = tree_1->root;
    if (node_1 != NULL) {
        while (node_1->left != NULL) {
            pred_1 = node_1->left;
            while (pred_1->right != NULL) { pred_1 = pred_1->right; }
            pred_1->right = node_1;
            node_1        = node_1->left;
        }
    }

    // Go to the smallest element of tree_2 while right-threading your path:
    node_2 = tree_2->root;
    if (node_2 != NULL) {
        while (node_2->left != NULL) {
            pred_2 = node_2->left;
            while (pred_2->right != NULL) { pred_2 = pred_2->right; }
            pred_2->right = node_2;
            node_2        = node_2->left;
        }
    }

    // Until we have exhausted at least one of the trees:
    while (node_1 != NULL && node_2 != NULL) {

        // compare both nodes:
        comp = (tree->comp)(node_1->data, node_2->data);

        if (comp < 0) {

            // insert node_1->data in tree /////////////////////////////////////
            if (node == NULL)  {
                tree->root = (bs_node *) malloc(sizeof(bs_node));
                node = tree->root;
            } else {
                node->right = (bs_node *) malloc(sizeof(bs_node));
                node = node->right;
            }
            if (node == NULL) {
                fprintf(stderr, "ERROR: Unable to allocate bs_node\n");
                return tree;
            } else {
                node->data  = node_1->data;
                node->left  = NULL;
                node->right = NULL;
            }
            ////////////////////////////////////////////////////////////////////

            // advance node_1 //////////////////////////////////////////////////
            node_1 = node_1->right;
            if (node_1 != NULL) {
                while (node_1->left != NULL) {
                    pred_1 = node_1->left;
                    while (pred_1->right != NULL && pred_1->right != node_1) {
                        pred_1 = pred_1->right;
                    }
                    if (pred_1->right == NULL) {
                        pred_1->right = node_1;
                        node_1        = node_1->left;
                    } else {
                        pred_1->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

        } else if (comp > 0) {

            // insert node_2->data in tree /////////////////////////////////////
            if (node == NULL)  {
                tree->root = (bs_node *) malloc(sizeof(bs_node));
                node = tree->root;
            } else {
                node->right = (bs_node *) malloc(sizeof(bs_node));
                node = node->right;
            }
            if (node == NULL) {
                fprintf(stderr, "ERROR: Unable to allocate bs_node\n");
                return tree;
            } else {
                node->data  = node_2->data;
                node->left  = NULL;
                node->right = NULL;
            }
            ////////////////////////////////////////////////////////////////////

            // advance node_2 //////////////////////////////////////////////////
            node_2 = node_2->right;
            if (node_2 != NULL) {
                while (node_2->left != NULL) {
                    pred_2 = node_2->left;
                    while (pred_2->right != NULL && pred_2->right != node_2) {
                        pred_2 = pred_2->right;
                    }
                    if (pred_2->right == NULL) {
                        pred_2->right = node_2;
                        node_2        = node_2->left;
                    } else {
                        pred_2->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

        } else {

            // advance node_1 //////////////////////////////////////////////////
            node_1 = node_1->right;
            if (node_1 != NULL) {
                while (node_1->left != NULL) {
                    pred_1 = node_1->left;
                    while (pred_1->right != NULL && pred_1->right != node_1) {
                        pred_1 = pred_1->right;
                    }
                    if (pred_1->right == NULL) {
                        pred_1->right = node_1;
                        node_1        = node_1->left;
                    } else {
                        pred_1->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

            // advance node_2 //////////////////////////////////////////////////
            node_2 = node_2->right;
            if (node_2 != NULL) {
                while (node_2->left != NULL) {
                    pred_2 = node_2->left;
                    while (pred_2->right != NULL && pred_2->right != node_2) {
                        pred_2 = pred_2->right;
                    }
                    if (pred_2->right == NULL) {
                        pred_2->right = node_2;
                        node_2        = node_2->left;
                    } else {
                        pred_2->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////
        }
    }

    // Insert all remaining data from tree_1:
    while (node_1 != NULL) {

        // insert node_1->data in tree /////////////////////////////////////////
        if (node == NULL)  {
            tree->root = (bs_node *) malloc(sizeof(bs_node));
            node = tree->root;
        } else {
            node->right = (bs_node *) malloc(sizeof(bs_node));
            node = node->right;
        }
        if (node == NULL) {
            fprintf(stderr, "ERROR: Unable to allocate bs_node\n");
            return tree;
        } else {
            node->data  = node_1->data;
            node->left  = NULL;
            node->right = NULL;
        }
        ////////////////////////////////////////////////////////////////////////

        // advance node_1 //////////////////////////////////////////////////////
        node_1 = node_1->right;
        if (node_1 != NULL) {
            while (node_1->left != NULL) {
                pred_1 = node_1->left;
                while (pred_1->right != NULL && pred_1->right != node_1) {
                    pred_1 = pred_1->right;
                }
                if (pred_1->right == NULL) {
                    pred_1->right = node_1;
                    node_1        = node_1->left;
                } else {
                    pred_1->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }

    // Insert all remaining data from tree_2:
    while (node_2 != NULL) {

        // insert node_2->data in tree /////////////////////////////////////////
        if (node == NULL)  {
            tree->root = (bs_node *) malloc(sizeof(bs_node));
            node = tree->root;
        } else {
            node->right = (bs_node *) malloc(sizeof(bs_node));
            node = node->right;
        }
        if (node == NULL) {
            fprintf(stderr, "ERROR: Unable to allocate bs_node\n");
            return tree;
        } else {
            node->data  = node_2->data;
            node->left  = NULL;
            node->right = NULL;
        }
        ////////////////////////////////////////////////////////////////////////

        // advance node_2 //////////////////////////////////////////////////////
        node_2 = node_2->right;
        if (node_2 != NULL) {
            while (node_2->left != NULL) {
                pred_2 = node_2->left;
                while (pred_2->right != NULL && pred_2->right != node_2) {
                    pred_2 = pred_2->right;
                }
                if (pred_2->right == NULL) {
                    pred_2->right = node_2;
                    node_2        = node_2->left;
                } else {
                    pred_2->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }

    // Return the resulting tree:
    return tree;
}



// REBALANCE OPERATIONS:

// Transforms any bs_tree in a highly degenerated bs_tree where tree->root
// points to the smallest element and all nodes have no left sub-tree.
//
// In general it is a really BAD idea to work with highly degenerated trees but
// I provide this tree_to_list method because it may be useful.
//
void bs_tree_to_list(bs_tree *tree) {

    bs_node *node;
    bs_node *left;
    bs_node *right;
    bs_node *parent;

    // Sanity check:
    assert(tree != NULL);

    // Avoid trivial case: empty tree
    if (tree->root != NULL) {

        node = tree->root;

        // Unravel the left side of root:
        while (node->left != NULL) {

            // Rotate right:
            left        = node->left;
            right       = left->right;
            left->right = node;
            node->left  = right;

            // Move to the new root:
            node = left;
        }

        // Fix the root:
        tree->root = node;
        parent     = NULL;

        // Unravel the rest of the tree:
        while (node != NULL) {

            if (node->left != NULL) {

                // Rotate right:
                left          = node->left;
                right         = left->right;
                left->right   = node;
                node->left    = right;
                parent->right = left;

                // Move up:
                node = left;

            } else {

                // Move down:
                parent = node;
                node   = node->right;
            }
        }
    }
}

// Transforms any bs_tree in a highly degenerated bs_tree where tree->root
// points to the biggest element and all nodes have no right sub-tree.
//
// In general it is a really BAD idea to work with highly degenerated trees but
// I provide this tree_to_reversed_list method because it may be useful.
//
void bs_tree_to_reversed_list(bs_tree *tree) {

    bs_node *node;
    bs_node *left;
    bs_node *right;
    bs_node *parent;

    // Sanity check:
    assert(tree != NULL);

    // Avoid trivial case: empty tree
    if (tree->root != NULL) {

        node = tree->root;

        // Unravel the right side of root:
        while (node->right != NULL) {

            // Rotate left:
            right       = node->right;
            left        = right->left;
            right->left = node;
            node->right = left;

            // Move to the new root:
            node = right;
        }

        // Fix the root:
        tree->root = node;
        parent     = NULL;

        // Unravel the rest of the tree:
        while (node != NULL) {

            if (node->right != NULL) {

                // Rotate left:
                right        = node->right;
                left         = right->left;
                right->left  = node;
                node->right  = left;
                parent->left = right;

                // Move up:
                node = right;

            } else {

                // Move down:
                parent = node;
                node   = node->left;
            }
        }
    }
}

// Since classic Binary Search Trees can become quite degenerated it is useful
// to have a "rebalancing" method to redress them. The Day–Stout–Warren
// algorithm does exactly this but it requires to count the number of nodes
// stored on the tree and I want to avoid this.
//
// Fortunatelly, applying the Day–Stout–Warren algorithm blindly works quite
// well and you get a "perfectly" balanced tree (in the sense of ensuring that
// the resulting tree height will be equal to ceil(log2(N)), which is optimal)
// in linear time without explicitly counting the number of nodes.
//
// As far as I know, this version of Day–Stout–Warren Algorithm is new.
// This being said, I will be surprised if nobody has implemented this before.
//
// You can use this operation to rebalance a binary search tree once you are
// sure that no further insertions will take place (or that they will be rare).
// Alternatively, if you have a counter that stores the number of elements of
// the tree, you can use this function every time the tree doubles in size to
// get some of the benefits of a balanced tree in O(1) amortized time.
//
void bs_tree_rebalance(bs_tree *tree) {

    bs_node *node;
    bs_node *left;
    bs_node *right;
    bs_node *child;
    bs_node *parent;

    // Sanity check:
    assert(tree != NULL);

    // Avoid trivial case: empty tree
    if (tree->root != NULL) {

        // LINEARIZE THE TREE: ///////////////////////////////////
                                                                //
        node = tree->root;                                      //
                                                                //
        // Unravel the left side of root:                       //
        while (node->left != NULL) {                            //
                                                                //
            // Rotate right:                                    //
            left        = node->left;                           //
            right       = left->right;                          //
            left->right = node;                                 //
            node->left  = right;                                //
                                                                //
            // Move to the new root:                            //
            node = left;                                        //
        }                                                       //
                                                                //
        // Fix the root:                                        //
        tree->root = node;                                      //
        parent     = NULL;                                      //
                                                                //
        // Unravel the rest of the tree:                        //
        while (node != NULL) {                                  //
                                                                //
            if (node->left != NULL) {                           //
                                                                //
                // Rotate right:                                //
                left          = node->left;                     //
                right         = left->right;                    //
                left->right   = node;                           //
                node->left    = right;                          //
                parent->right = left;                           //
                                                                //
                // Move up:                                     //
                node = left;                                    //
                                                                //
            } else {                                            //
                                                                //
                // Move down:                                   //
                parent = node;                                  //
                node   = node->right;                           //
            }                                                   //
        }                                                       //
        //////////////////////////////////////////////////////////


        // RE-BALANCE THE TREE ///////////////////////////////////
                                                                //
        for (;;) {                                              //
                                                                //
            parent = NULL;                                      //
            node   = tree->root;                                //
            child  = node->right;                               //
                                                                //
            if (child == NULL) { break; }                       //
                                                                //
            // Move along the right spine:                      //
            while (child != NULL) {                             //
                                                                //
                // Rotate left:                                 //
                if (parent == NULL) { tree->root    = child; }  //
                else                { parent->right = child; }  //
                node->right = child->left;                      //
                child->left = node;                             //
                                                                //
                // Advance:                                     //
                parent = child;                                 //
                node   = parent->right;                         //
                if (node == NULL) { break; }                    //
                else { child = node->right; }                   //
            }                                                   //
        }                                                       //
        //////////////////////////////////////////////////////////


        // A FINAL IMPROVEMENT ///////////////////////////////////
        //
        // When you arrive here the root is the biggest element
        // of the tree and has no right child.
        //
        // So, if we remove the root node and insert it as the
        // right pointer of the second biggest element, the new
        // root element will be way closer to be the "median"
        // element (rather than being the biggest element) and
        // everyone else will be 1 step closer to the root.
        //
        // If you do not understand what I mean, comment the
        // following lines and use the print_bs_tree
        // function to take a look.
        //
        node = tree->root;
        if (node->left != NULL) {
            tree->root = node->left;
            child      = node->left;
            while (child->right != NULL) { child = child->right; }
            child->right = node;
            node->left = NULL;
        }
        //////////////////////////////////////////////////////////
    }
}


// DEBUG & VISUALIZATION:

// This is an auxiliary function to check the symmetric order property
// recursively. You should not use it directly, use "is_bs_tree" instead.
//
// Note that, unlike other functions of this library, it is safe to use
// recursive definitions here since they will only be used to test the
// code while debugging this library.
//
static int is_bs_subtree(const bs_tree *tree, const bs_node *node,
                         const void *min, const void *max) {

    // Make sure that node is (strictly) between specified limits:
    if (min != NULL && (tree->comp)(min, node->data) >= 0) {
        fprintf(stderr,"ERROR: Symmetric order not satisfied in bs_tree\n");
        return NO;
    }
    if (max != NULL && (tree->comp)(node->data, max) >= 0) {
        fprintf(stderr,"ERROR: Symmetric order not satisfied in bs_tree\n");
        return NO;
    }

    // Check recursively the left subtree of node:
    if (node->left != NULL) {
        if (is_bs_subtree(tree, node->left, min, node->data) == NO) {
            return NO;
        }
    }

    // Check recursively the right subtree of node:
    if (node->right != NULL) {
        if (is_bs_subtree(tree, node->right, node->data, max) == NO) {
            return NO;
        }
    }

    // All test are fine:
    return YES;
}

// This is an auxiliary function to check the symmetric order property of a
// binary tree. Returns YES if everything is correct and NO otherwise.
//
// This function should not be used in production code. I recommend to use:
//
//      assert(is_bs_tree(tree) == YES);
//
// To automatically remove all calls to this function when the flag NDEBUG
// is defined in the header files (deactivating all assertions).
//
int is_bs_tree(const bs_tree *tree) {

    // Basic Sanity Checks:
    if (tree == NULL) {
        fprintf(stderr, "ERROR: NULL pointer to bs_tree\n");
        return NO;
    }
    if (tree->comp == NULL) {
        fprintf(stderr, "ERROR: NULL comparing function in bs_tree\n");
        return NO;
    }

    // Trivial Case: empty tree
    if (tree->root == NULL) { return YES; }

    // General Case:
    return is_bs_subtree(tree, tree->root, NULL, NULL);
}

// This is an auxiliary function to print the tree recursively.
// You should not use it directly, use "print_bs_tree" instead.
//
// Note that, unlike the rest of the functions of this library, it is safe
// to use recursive definitions here since any tree large enough to hit the
// recursion limit will be too big to be printed anyway.
//
static void print_bs_subtree(const bs_node *node, const int is_right,
                             char *indent, void (* print_node) (const void *)) {

    // Allocate memory:
    char *new_indent = (char *) malloc((6+strlen(indent)*sizeof(char)));
    assert(new_indent != NULL);

    // Print right subtree recursively:
    if (node->right != NULL) {
        if (is_right == YES) { sprintf(new_indent, "%s%s", indent, "      "); }
        else                 { sprintf(new_indent, "%s%s", indent, "|     "); }
        print_bs_subtree(node->right, YES, new_indent, print_node);
    }

    // Print current node:
    if (is_right) { fprintf(stdout, "%s,----",indent); }
    else          { fprintf(stdout, "%s`----",indent); }
    if (print_node == NULL)  { fprintf(stdout, "(#)"); }
    else                     { print_node(node->data); }
    printf("\n");

    // Print left subtree recursively:
    if (node->left != NULL) {
        if (is_right == YES) { sprintf(new_indent, "%s%s", indent, "|     "); }
        else                 { sprintf(new_indent, "%s%s", indent, "      "); }
        print_bs_subtree(node->left, NO, new_indent, print_node);
    }

    // Free memory:
    free(new_indent);
}

// This function is used to print a binary tree on the screen.
//
//  * If a "print_node" function is provided it will be used to print the
//    content of each node.
//  * Otherwise (if print_node is NULL) all nodes will be printed as (#).
//
// Use this latter option if you only want to take a look at the tree shape.
//
// A correct print_node function only prints a single line of text using
// the "fprintf" command. It should NOT include any "\n" in that line.
//
// For example:
//
//  void MyPrintNodeFunction(const void *ptr) {
//      MyDataStruct *d = (MyDataStruct *) ptr;     // Pointer casting
//      fprintf(stdout, "(%d)", d->key);            // Printing integer key
//  }
//
// Finally, note that this is a visualization tool for debugging purposes only.
//
void print_bs_tree(const bs_tree *tree, void (* print_node) (const void *)) {

    // Avoid the trivial cases:
    if (tree != NULL && tree->root != NULL) {

        // Print right subtree recursively:
        if (tree->root->right != NULL) {
            print_bs_subtree(tree->root->right, YES, "     ", print_node);
        }

        // Print current node:
        fprintf(stdout, "----");
        if (print_node == NULL) { fprintf(stdout, "(#)"); }
        else     { print_node(tree->root->data); }
        fprintf(stdout, "\n");

        // Print left subtree recursively:
        if (tree->root->left != NULL) {
            print_bs_subtree(tree->root->left, NO, "     ", print_node);
        }
    }

    // Empty the "stdout" buffer:
    fflush(stdout);
}

// END OF BINARY SEARCH TREES //////////////////////////////////////////////////





// RED BLACK TREES /////////////////////////////////////////////////////////////


// CREATION & INSERTION:

// Returns a pointer to a newly created rb_tree.
// You must provide a comparing function "comp" such that:
//  * comp(A,B) = 0  if and only if  A = B
//  * comp(A,B) > 0  if and only if  A > B
//  * comp(A,B) < 0  if and only if  A < B
//
// I recommend to limit the output of the "comp" function to {-1, 0, 1} as in:
//
//  int MyComp(const void *ptr1, const void *ptr2) {
//      MyData *d1 = (MyData *) ptr1;
//      MyData *d2 = (MyData *) ptr2;
//      if      (d1->key < d2->key) { return -1; }
//      else if (d1->key > d2->key) { return +1; }
//      else                        { return  0; }
//  }
//
// However, the code does not assume that so, if you are working with simple
// numeric keys, you can just use something like:
//
//  int MyComp(const void *ptr1, const void *ptr2) {
//      MyData *d1 = (MyData *) ptr1;
//      MyData *d2 = (MyData *) ptr2;
//      return (int) ((d1->key) - (d2->key));
//  }
//
rb_tree *new_rb_tree(int (* comp) (const void *, const void *)) {

    // Sanity check:
    assert(comp != NULL);

    // Allocate memory:
    rb_tree *tree = (rb_tree *) malloc(sizeof(rb_tree));
    if (tree == NULL) {
        fprintf(stderr, "ERROR: Unable to allocate memory for rb_tree\n");
    }

    // Initialize the empty tree:
    else {
        tree->root = NULL;
        tree->comp = comp;
    }

    return tree;
}

// Returns a new rb_tree containing a copy of the tree.
//
// It takes O( |Tree|·Log(|Tree|) } ) time.
//
rb_tree *rb_tree_copy(const rb_tree *tree) {

    rb_tree *new_tree = NULL;
    rb_node *pred;
    rb_node *node;
    void    *data;
    
    // Sanity check:
    assert(tree != NULL);
    
    // Create a new tree:
    new_tree = new_rb_tree(tree->comp);
    if (new_tree == NULL) { return NULL; }

    // Go to the smallest element of tree while right-threading your path:
    node = tree->root;
    if (node != NULL) {
        while (node->left != NULL) {
            pred = node->left;
            while (pred->right != NULL) { pred = pred->right; }
            pred->right = node;
            node        = node->left;
        }
    }

    // Insert all data from tree into new_tree:
    while (node != NULL) {

        // insert node->data in new_tree ///////////////////////////////////////
        data = node->data;
        data = rb_tree_insert_max(new_tree, data);
        assert(data == NULL);
        ////////////////////////////////////////////////////////////////////////

        // advance node ////////////////////////////////////////////////////////
        node = node->right;
        if (node != NULL) {
            while (node->left != NULL) {
                pred = node->left;
                while (pred->right != NULL && pred->right != node) {
                    pred = pred->right;
                }
                if (pred->right == NULL) {
                    pred->right = node;
                    node        = node->left;
                } else {
                    pred->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }
    
    // Return the new_tree:
    return new_tree;
}

// Inserts data in tree.
//
// If a node of the tree compares "equal" to data it will get replaced and a
// pointer to the previously stored data will be returned (so you can free it),
// otherwise it will simply return a NULL pointer.
//
void *rb_tree_insert(rb_tree *tree, void *data) {

    rb_node *anchor   = NULL;   // We need to store the last 4 levels:
    rb_node *granpa   = NULL;   //
    rb_node *parent   = NULL;   //            anchor
    rb_node *node     = NULL;   //              |    <- comp_g
    void    *old_data = NULL;   //            granpa
    int      comp_g   = 0;      //              |    <- comp_p
    int      comp_p   = 0;      //            parent
    int      comp_n   = 0;      //              |    <- comp_n
    int      comp     = 0;      //             node

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Search for the correct place to insert data:
    node = tree->root;
    for (;;) {

        // If we reach a leaf we must insert "data" here:
        if (node == NULL) {

            // Create a new node:
            node = (rb_node *) malloc(sizeof(rb_node));
            if (node == NULL) {
                fprintf(stderr, "ERROR: Unable to allocate rb_node\n");
                break;
            } else {
                node->data  = data;
                node->left  = NULL;
                node->right = NULL;
                node->color = RED;
                comp        = 0;
            }

            // And attach it bellow "parent":
            if (parent == NULL)  { tree->root    = node; }
            else if (comp_n < 0) { parent->left  = node; }
            else                 { parent->right = node; }

        // Otherwise "node" is an interior node:
        } else {

            // Compare "data" with "node->data":
            comp = (tree->comp)(data, node->data);

            // If the data is already there: Update and remember "old_data"
            if (comp == 0) {
                old_data   = node->data;
                node->data = data;
            }

            // If "node" has two RED children: Make a color flip
            if (IS_RED(node->left) && IS_RED(node->right)) {
                node->color        = RED;
                node->left->color  = BLACK;
                node->right->color = BLACK;
            }
        }

        // Repair any violation of the RED property:
        if (IS_RED(node) && IS_RED(parent)) {

            // Case 1: Single "granpa-parent" left rotation
            if (comp_p > 0 && comp_n > 0) {

                granpa->right = parent->left;
                granpa->color = RED;
                parent->left  = granpa;
                parent->color = BLACK;
                
                if  (anchor == NULL) { tree->root    = parent; }
                else if (comp_g < 0) { anchor->left  = parent; }
                else if (comp_g > 0) { anchor->right = parent; }
                granpa = anchor;
                comp_p = comp_g;

            // Case 2: Single "granpa-parent" right rotation
            } else if (comp_p < 0 && comp_n < 0) {

                granpa->left  = parent->right;
                granpa->color = RED;
                parent->right = granpa;
                parent->color = BLACK;
                
                if  (anchor == NULL) { tree->root    = parent; }
                else if (comp_g < 0) { anchor->left  = parent; }
                else if (comp_g > 0) { anchor->right = parent; }
                granpa = anchor;
                comp_p = comp_g;

            // Case 3: Double "granpa-parent-node" rotation
            } else {

                // Case 3.1: Left-Right
                if (comp_n < 0) {
                    granpa->right = node->left;
                    granpa->color = RED;
                    parent->left  = node->right;
                    node->left    = granpa;
                    node->right   = parent;
                    node->color   = BLACK;
                    if (comp > 0) { granpa = parent; }
                    parent = node;
                    node   = granpa;
                    if (comp > 0) { comp_n *= -1;      }
                    if (comp < 0) { comp_n  = -comp_p; }
                    
                // Case 3.2: Right-Left
                } else {
                    granpa->left  = node->right;
                    granpa->color = RED;
                    parent->right = node->left;
                    node->right   = granpa;
                    node->left    = parent;
                    node->color   = BLACK;
                    if (comp < 0) { granpa = parent; }
                    parent = node;
                    node   = granpa;
                    if (comp < 0) { comp_n *= -1;      }
                    if (comp > 0) { comp_n  = -comp_p; }
                }

                if  (anchor == NULL) { tree->root    = parent; }
                else if (comp_g < 0) { anchor->left  = parent; }
                else if (comp_g > 0) { anchor->right = parent; }
                granpa = anchor;
                comp_p = comp_g;
                comp  *= -1;
            }
        }

        // Advance one step:
        anchor = granpa;
        granpa = parent;
        parent = node;
        if      (comp < 0) { node = node->left;  }  // Data is smaller
        else if (comp > 0) { node = node->right; }  // Data is bigger
        else               { break;              }  // We are done!

        // And remember were you come from:
        comp_g = comp_p;
        comp_p = comp_n;
        comp_n = comp;
    }

    // Before leaving: Make sure that the root is BLACK!
    if (tree->root != NULL) { tree->root->color = BLACK; }

    // And return old_data (which will be NULL unless data was already here)
    return old_data;
}

// Inserts "data" such that "data" is smaller or equal to any other "data"
// already in the tree. It is slightly faster than a regular insert because it
// makes at most one comparison.
//
// WARNING: If you use this function to insert data that is strictly bigger than
//          something already in the tree you will break the tree!
//
// If a node of the tree compares "equal" to data it will get replaced and a
// pointer to the previously stored data will be returned (so you can free it),
// otherwise it will simply return a NULL pointer.
//
void *rb_tree_insert_min(rb_tree *tree, void *data) {

    rb_node *anchor   = NULL;
    rb_node *granpa   = NULL;
    rb_node *parent   = NULL;
    rb_node *node     = NULL;
    void    *old_data = NULL;
    int      inserted = NO;

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Search for the correct place to insert data:
    node = tree->root;
    for (;;) {

        // If we reach a leaf we must insert "data" here:
        if (node == NULL) {

            // Perhaps "parent" already contains "data":
            if (parent != NULL && (tree->comp)(data, parent->data) == 0) {
                old_data   = node->data;
                node->data = data;
                break;

            // Otherwise: Create a new node 
            } else {            
                node = (rb_node *) malloc(sizeof(rb_node));
                if (node == NULL) {
                    fprintf(stderr, "ERROR: Unable to allocate rb_node\n");
                    break;
                } else {
                    node->data  = data;
                    node->left  = NULL;
                    node->right = NULL;
                    node->color = RED;
                    inserted    = YES;
                }
                if (parent == NULL)  { tree->root   = node; }
                else                 { parent->left = node; }
            }            
            
        // Otherwise: "node" may require a color flip
        } else if (IS_RED(node->left) && IS_RED(node->right)) {
            node->color        = RED;
            node->left->color  = BLACK;
            node->right->color = BLACK;
        }

        // Repair any violation of the RED property: Single right rotation
        if (IS_RED(node) && IS_RED(parent)) {            
            granpa->left  = parent->right;
            granpa->color = RED;
            parent->right = granpa;
            parent->color = BLACK;            
            if  (anchor == NULL) { tree->root   = parent; }
            else                 { anchor->left = parent; }
            granpa = anchor;
        }

        // Advance one step or exit:
        if (inserted == YES) { break; }
        anchor = granpa;
        granpa = parent;
        parent = node;
        node   = node->left;        
    }

    // Before leaving: Make sure that the root is BLACK!
    if (tree->root != NULL) { tree->root->color = BLACK; }

    // And return old_data (which will be NULL unless data was already here)
    return old_data;
}

// Inserts "data" such that "data" is bigger or equal to any other "data"
// already in the tree. It is slightly faster than a regular insert because it
// makes at most one comparison.
//
// WARNING: If you use this function to insert data that is strictly smaller
//          than something already in the tree you will break the tree!
//
// If a node of the tree compares "equal" to data it will get replaced and a
// pointer to the previously stored data will be returned (so you can free it),
// otherwise it will simply return a NULL pointer.
//
void *rb_tree_insert_max(rb_tree *tree, void *data) {

    rb_node *anchor   = NULL;
    rb_node *granpa   = NULL;
    rb_node *parent   = NULL;
    rb_node *node     = NULL;
    void    *old_data = NULL;
    int      inserted = NO;

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Search for the correct place to insert data:
    node = tree->root;
    for (;;) {

        // If we reach a leaf we must insert "data" here:
        if (node == NULL) {

            // Perhaps "parent" already contains "data":
            if (parent != NULL && (tree->comp)(data, parent->data) == 0) {
                old_data   = node->data;
                node->data = data;
                break;

            // Otherwise: Create a new node 
            } else {            
                node = (rb_node *) malloc(sizeof(rb_node));
                if (node == NULL) {
                    fprintf(stderr, "ERROR: Unable to allocate rb_node\n");
                    break;
                } else {
                    node->data  = data;
                    node->left  = NULL;
                    node->right = NULL;
                    node->color = RED;
                    inserted    = YES;
                }
                if (parent == NULL)  { tree->root    = node; }
                else                 { parent->right = node; }
            }            
            
        // Otherwise: "node" may require a color flip
        } else if (IS_RED(node->left) && IS_RED(node->right)) {
            node->color        = RED;
            node->left->color  = BLACK;
            node->right->color = BLACK;
        }

        // Repair any violation of the RED property: Single left rotation
        if (IS_RED(node) && IS_RED(parent)) {
            granpa->right = parent->left;
            granpa->color = RED;
            parent->left  = granpa;
            parent->color = BLACK;
            if  (anchor == NULL) { tree->root    = parent; }
            else                 { anchor->right = parent; }
            granpa = anchor;
        }

        // Advance one step or exit:
        if (inserted == YES) { break; }
        anchor = granpa;
        granpa = parent;
        parent = node;
        node   = node->right;        
    }

    // Before leaving: Make sure that the root is BLACK!
    if (tree->root != NULL) { tree->root->color = BLACK; }

    // And return old_data (which will be NULL unless data was already here)
    return old_data;
}



// SEARCH:

// Returns YES if the tree is empty and NO otherwise.
//
int rb_tree_is_empty(const rb_tree *tree) {

    // Sanity check:
    assert(tree != NULL);

    // Check if there is at least 1 node in the tree:
    if (tree->root == NULL) { return YES; }
    else                    { return NO;  }
}

// Finds a node that compares "equal" to data. Returns NULL if not found.
//
void *rb_tree_search(const rb_tree *tree, const void *data) {

    rb_node *node;
    int      comp;

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Search:
    node = tree->root;
    while (node != NULL) {
        comp = (tree->comp)(data, node->data);      // compare data
        if      (comp < 0) { node = node->left;  }  // data is smaller
        else if (comp > 0) { node = node->right; }  // data is bigger
        else               { return node->data;  }  // found!
    }

    // Not found:
    return NULL;
}

// Returns a pointer to the smallest element stored in the tree.
// Returns NULL if the tree is empty.
//
void *rb_tree_min(const rb_tree *tree) {

    rb_node *node;

    // Sanity check:
    assert(tree != NULL);

    // Trivial case: empty tree
    if (tree->root == NULL) { return NULL; }

    // General case: Find the smallest node
    node = tree->root;
    while (node->left != NULL) { node = node->left; }

    // Return a pointer to the data:
    return node->data;
}

// Returns a pointer to the biggest element stored in "tree".
// Returns NULL if the tree is empty.
//
void *rb_tree_max(const rb_tree *tree) {

    rb_node *node;

    // Sanity check:
    assert(tree != NULL);

    // Trivial case: empty tree
    if (tree->root == NULL) { return NULL; }

    // General case: Find the biggest node
    node = tree->root;
    while (node->right != NULL) { node = node->right; }

    // Return a pointer to the data:
    return node->data;
}

// Find the in-order predesor of data in the tree.
//
// If data is not in tree returns the biggest element of tree smaller than data.
// If data is smaller or equal to all elements of tree returns NULL.
//
void *rb_tree_prev(const rb_tree *tree, const void *data) {

    rb_node *node;
    rb_node *pred;
    int      comp;

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Search:
    pred = NULL;
    node = tree->root;
    while (node != NULL) {

        // Compare data:
        comp = (tree->comp)(data, node->data);

        // Data is smaller:
        if (comp < 0) { node = node->left; }

        // Data is bigger:
        else if (comp > 0) {
            pred = node;
            node = node->right;
        }

        // We have found the node:
        else if (node->left != NULL) {
            pred = node->left;
            while (pred->right != NULL) { pred = pred->right; }
            break;
        } else { break; }
    }

    // Return predecesor:
    if (pred == NULL) { return NULL; }
    return pred->data;
}

// Find the in-order successor of data in the tree.
//
// If data is not in tree returns the smallest element of tree bigger than data.
// If data is bigger or equal to all elements of tree returns NULL.
//
void *rb_tree_next(const rb_tree *tree, const void *data) {

    rb_node *node;
    rb_node *succ;
    int      comp;

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Search:
    succ = NULL;
    node = tree->root;
    while (node != NULL) {

        // Compare data:
        comp = (tree->comp)(data, node->data);

        // Data is smaller:
        if (comp < 0) {
            succ = node;
            node = node->left;
        }

        // Data is bigger:
        else if (comp > 0) { node = node->right; }

        // We have found the node:
        else if (node->right != NULL) {
            succ = node->right;
            while (succ->left != NULL) { succ = succ->left; }
            break;
        } else { break; }
    }

    // Return succesor:
    if (succ == NULL) { return NULL; }
    return succ->data;
}


// REMOVE:

// Removes a node of tree that compares "equal" to data and returns a pointer
// to the previously stored data (so you can free it).
// If such a node is not found, it returns a NULL pointer.
//
// Uses the trick of swapping node->data and successor->data pointers and then
// removes successor. This is safe because the final user has no acces to any
// tree node (only "tree" and "data" pointers are used in all interfaces) and
// no traverser is coded either (so no "internal state" is stored in them).
//
void *rb_tree_remove(rb_tree *tree, const void *data) {

    rb_node *granpa   = NULL;   // We need to store the last 3 levels //
    rb_node *parent   = NULL;   //                                    //
    rb_node *sister   = NULL;   //            granpa                  //
    rb_node *node     = NULL;   //              |                     //
    rb_node *old_node = NULL;   //            parent                  //
    void    *old_data = NULL;   //            /    \   <- comp_n      //
    int      comp_n   = 0;      //        sister  node                //
    int      comp     = 0;      //                / \  <- comp        //

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Initialize the search at the root node:
    node = tree->root;
    if (node == NULL) { return NULL; }
    
    // Look for a leaf:
    while (node != NULL) {

        // At this point node is BLACK, if sister exists is BLACK and if parent 
        // exists is RED. We want to paint node RED and repair any violation.

        // Case 1: Node has two BLACK children
        if (IS_BLACK(node->left) && IS_BLACK(node->right)) {

            // Easy case: the node is the root node
            if (parent == NULL) { node->color = RED; }

            // General case:
            else {

                // Case 1.0: Node has no sister
                if (sister == NULL) {
                    node->color   = RED;
                    parent->color = BLACK;
                    
                // Case 1.1: Sister has 2 BLACK children
                } else if (IS_BLACK(sister->left) && IS_BLACK(sister->right)){
                    node->color   = RED;
                    sister->color = RED;
                    parent->color = BLACK;

                // Case 1.2: Sister has at least 1 RED children
                } else {

                    // If sister->left is RED:
                    if (IS_RED(sister->left)) {

                        // If sister == parent->right: Double rotation
                        if (comp < 0) {

                            if  (granpa == NULL) { tree->root    = sister->left; }
                            else if (comp_n < 0) { granpa->left  = sister->left; }
                            else                 { granpa->right = sister->left; }
                            granpa = sister->left;
                            
                            parent->right = granpa->left;
                            granpa->left  = parent;

                            sister->left  = granpa->right;
                            granpa->right = sister;
                            sister        = parent->right;
                            
                            node->color   = RED;
                            parent->color = BLACK;
                        }

                        // If sister == parent->left: Single rotation
                        else {

                            if  (granpa == NULL) { tree->root    = sister; }
                            else if (comp_n < 0) { granpa->left  = sister; }
                            else                 { granpa->right = sister; }
                            granpa = sister;
                            
                            parent->left  = granpa->right;
                            granpa->right = parent;
                            sister        = parent->left;
                            
                            node->color   = RED;
                            granpa->color = RED;
                            parent->color = BLACK;
                            granpa->left->color = BLACK;
                        }
                    }

                    // If sister->right is RED:
                    else {

                        // If sister == parent->left: Double rotation
                        if (comp > 0) {

                            if  (granpa == NULL) { tree->root    = sister->right; }
                            else if (comp_n < 0) { granpa->left  = sister->right; }
                            else                 { granpa->right = sister->right; }
                            granpa = sister->right;
                            
                            parent->left  = granpa->right;
                            granpa->right = parent;

                            sister->right = granpa->left;
                            granpa->left  = sister;
                            sister        = parent->left;
                            
                            node->color   = RED;
                            parent->color = BLACK;
                        }

                        // If sister == parent->right: Single rotation
                        else {

                            if  (granpa == NULL) { tree->root    = sister; }
                            else if (comp_n < 0) { granpa->left  = sister; }
                            else                 { granpa->right = sister; }
                            granpa = sister;
                            
                            parent->right = granpa->left;
                            granpa->left  = parent;
                            sister        = parent->right;

                            node->color   = RED;
                            granpa->color = RED;
                            parent->color = BLACK;
                            granpa->right->color = BLACK;
                        }
                    }
                }
            }
        }

        // We compare the data now because we need the information for "Case 2"

        // Compare data unless you already know where to go:
        comp_n = comp;
        comp   = (old_data == NULL) ? (tree->comp)(data, node->data) : (-1);

        // If we have found the node to remove: Remember it!
        if (comp == 0) {
            old_data = node->data;
            old_node = node;
            comp     = +1;   // ...and search the successor
        }
        
        // Case 2: Node has at least one RED children
        if (IS_RED(node->left) || IS_RED(node->right)) {

            // Again node is BLACK, if sister exists is BLACK and if parent
            // exists is RED. We paint node RED and repair any violation.

            // Case 2.1: We are moving to the RED node
            if ((comp < 0 && IS_RED(node->left )) ||
                (comp > 0 && IS_RED(node->right)) ){

                // Move and compare again for free!
                granpa = parent;
                parent = node;
                if (comp < 0) {
                    node   = parent->left;
                    sister = parent->right;
                } else if (comp > 0) {
                    node   = parent->right;
                    sister = parent->left;
                }
                comp_n = comp;
                comp   = (old_data == NULL) ? (tree->comp)(data, node->data) : (-1);
                if (comp == 0) {
                    old_data = node->data;
                    old_node = node;
                    comp     = +1;
                }
            }
            
            // Case 2.2: We are moving to the BLACK node
            else {
                // If we are moving to the left: Single left-rotation
                if (comp < 0) {
                    if  (parent == NULL) { tree->root    = node->right; }
                    else if (comp_n < 0) { parent->left  = node->right; }
                    else                 { parent->right = node->right; }
                    granpa        = parent;
                    parent        = node->right;
                    sister        = parent->right;
                    node->right   = parent->left;
                    parent->left  = node;
                                        
                    node->color   = RED;
                    parent->color = BLACK;

                    comp_n = -1;
                }

                // If we are moving to the right: Single right-rotation
                else {
                    if  (parent == NULL) { tree->root    = node->left; }
                    else if (comp_n < 0) { parent->left  = node->left; }
                    else                 { parent->right = node->left; }
                    granpa        = parent;
                    parent        = node->left;
                    sister        = parent->left;
                    node->left    = parent->right;
                    parent->right = node;
                                        
                    node->color   = RED;
                    parent->color = BLACK;

                    comp_n = 1;
                }                
            }
        }

        // ...and finally move!
        granpa = parent;
        parent = node;
        if (comp < 0) {
            node   = parent->left;
            sister = parent->right;
        } else if (comp > 0) {
            node   = parent->right;
            sister = parent->left;
        }
    }

    // Erase "parent", which should be RED:
    if (old_node != NULL) {
        old_node->data = parent->data;
        if      (granpa       == NULL)   { tree->root    = parent->right; }
        else if (granpa->left == parent) { granpa->left  = parent->right; }
        else                             { granpa->right = parent->right; }
        free(parent);
    }
    
    // Before leaving: Make sure that the root is BLACK!
    if (tree->root != NULL) { tree->root->color = BLACK; }

    // And return old_data (which will be NULL unless data was already here)
    return old_data;
}

// Removes the smallest element from "tree" and returns a pointer to its data
// (so you can free it). If the tree is empty returns a NULL pointer.
//
// More efficient than "tree_remove(tree, min_elem)" even if you already know
// the value of "min_elem".
//
void *rb_tree_remove_min(rb_tree *tree) {

    rb_node *granpa   = NULL;   //      granpa      //
    rb_node *parent   = NULL;   //        |         //
    rb_node *sister   = NULL;   //      parent      //
    rb_node *node     = NULL;   //      /    \      // 
    void    *old_data = NULL;   //   node  sister   //

    // Sanity Check:
    assert(tree != NULL);

    // Initialize the search at the root node:
    node = tree->root;
    if (node == NULL) { return NULL; }
    
    // Look for a leaf:
    while (node != NULL) {

        // At this point node is BLACK, if sister exists is BLACK and if parent 
        // exists is RED. We want to paint node RED and repair any violation.

        // Case 1: Node has two BLACK children
        if (IS_BLACK(node->left) && IS_BLACK(node->right)) {

            // Easy case: the node is the root node
            if (parent == NULL) { node->color = RED; }

            // General case:
            else {

                // Case 1.0: Node has no sister
                if (sister == NULL) {
                    node->color   = RED;
                    parent->color = BLACK;
                    
                // Case 1.1: Sister has 2 BLACK children
                } else if (IS_BLACK(sister->left) && IS_BLACK(sister->right)){
                    node->color   = RED;
                    sister->color = RED;
                    parent->color = BLACK;

                // Case 1.2: Sister has at least 1 RED children
                } else {

                    // If sister->left is RED:
                    if (IS_RED(sister->left)) {

                        // Sister == parent->right: Double rotation
                        if  (granpa == NULL) { tree->root    = sister->left; }
                        else                 { granpa->left  = sister->left; }
                        granpa = sister->left;
                        
                        parent->right = granpa->left;
                        granpa->left  = parent;

                        sister->left  = granpa->right;
                        granpa->right = sister;
                        sister        = parent->right;
                        
                        node->color   = RED;
                        parent->color = BLACK;
                    }

                    // If sister->right is RED:
                    else {

                        // Sister == parent->right: Single rotation
                        if  (granpa == NULL) { tree->root    = sister; }
                        else                 { granpa->left  = sister; }
                        granpa = sister;

                        parent->right = granpa->left;
                        granpa->left  = parent;
                        sister        = parent->right;

                        node->color   = RED;
                        granpa->color = RED;
                        parent->color = BLACK;
                        granpa->right->color = BLACK;
                    }
                }
            }
        }

        // Case 2: Node has at least one RED children
        if (IS_RED(node->left) || IS_RED(node->right)) {

            // Again node is BLACK, if sister exists is BLACK and if parent
            // exists is RED. We paint node RED and repair any violation.

            // Case 2.1: We are moving to the RED node
            if (IS_RED(node->left)){

                // Move and compare again for free!
                granpa = parent;
                parent = node;
                node   = parent->left;
                sister = parent->right;
            }
            
            // Case 2.2: We are moving to the BLACK node
            else {
                // We are moving to the left: Single left-rotation
                if  (parent == NULL) { tree->root    = node->right; }
                else                 { parent->left  = node->right; }
                granpa        = parent;
                parent        = node->right;
                sister        = parent->right;
                node->right   = parent->left;
                parent->left  = node;
                                    
                node->color   = RED;
                parent->color = BLACK;
            }
        }

        // ...and finally move!
        granpa = parent;
        parent = node;
        node   = parent->left;
        sister = parent->right;
    }

    // Erase "parent", which should be RED:
    old_data = parent->data;
    if (granpa == NULL) { tree->root   = parent->right; }
    else                { granpa->left = parent->right; }
    free(parent);
    
    // Before leaving: Make sure that the root is BLACK!
    if (tree->root != NULL) { tree->root->color = BLACK; }

    // And return old_data (which will be NULL unless data was already here)
    return old_data;
}

// Removes the biggest element from "tree" and returns a pointer to its data
// (so you can free it). If the tree is empty returns a NULL pointer.
//
// More efficient than "tree_remove(tree, max_data)" even if you already know
// the value of "max_data".
//
void *rb_tree_remove_max(rb_tree *tree) {

    rb_node *granpa   = NULL;   //      granpa      //
    rb_node *parent   = NULL;   //        |         //
    rb_node *sister   = NULL;   //      parent      //
    rb_node *node     = NULL;   //      /    \      // 
    void    *old_data = NULL;   //   sister node    //

    // Sanity Check:
    assert(tree != NULL);
    
    // Initialize the search at the root node:
    node = tree->root;
    if (node == NULL) { return NULL; }
    
    // Look for a leaf:
    while (node != NULL) {

        // At this point node is BLACK, if sister exists is BLACK and if parent 
        // exists is RED. We want to paint node RED and repair any violation.

        // Case 1: Node has two BLACK children
        if (IS_BLACK(node->left) && IS_BLACK(node->right)) {

            // Easy case: the node is the root node
            if (parent == NULL) { node->color = RED; }

            // General case:
            else {

                // Case 1.0: Node has no sister
                if (sister == NULL) {
                    node->color   = RED;
                    parent->color = BLACK;
                    
                // Case 1.1: Sister has 2 BLACK children
                } else if (IS_BLACK(sister->left) && IS_BLACK(sister->right)){
                    node->color   = RED;
                    sister->color = RED;
                    parent->color = BLACK;

                // Case 1.2: Sister has at least 1 RED children
                } else {

                    // If sister->left is RED:
                    if (IS_RED(sister->left)) {

                        // Sister == parent->left: Single rotation
                        if  (granpa == NULL) { tree->root    = sister; }
                        else                 { granpa->right = sister; }
                        granpa = sister;
                        
                        parent->left  = granpa->right;
                        granpa->right = parent;
                        sister        = parent->left;
                        
                        node->color   = RED;
                        granpa->color = RED;
                        parent->color = BLACK;
                        granpa->left->color = BLACK;
                    }

                    // If sister->right is RED:
                    else {

                        // If sister == parent->left: Double rotation
                        if  (granpa == NULL) { tree->root    = sister->right; }
                        else                 { granpa->right = sister->right; }
                        granpa = sister->right;
                        
                        parent->left  = granpa->right;
                        granpa->right = parent;

                        sister->right = granpa->left;
                        granpa->left  = sister;
                        sister        = parent->left;
                        
                        node->color   = RED;
                        parent->color = BLACK;
                    }
                }
            }
        }

        // Case 2: Node has at least one RED children
        if (IS_RED(node->left) || IS_RED(node->right)) {

            // Again node is BLACK, if sister exists is BLACK and if parent
            // exists is RED. We paint node RED and repair any violation.

            // Case 2.1: We are moving to the RED node
            if (IS_RED(node->right)){

                // Move and compare again for free!
                granpa = parent;
                parent = node;
                node   = parent->right;
                sister = parent->left;
            }
            
            // Case 2.2: We are moving to the BLACK node
            else {
                // we are moving to the right: Single right-rotation
                if  (parent == NULL) { tree->root    = node->left; }
                else                 { parent->right = node->left; }
                granpa        = parent;
                parent        = node->left;
                sister        = parent->left;
                node->left    = parent->right;
                parent->right = node;
                                    
                node->color   = RED;
                parent->color = BLACK;
            }
        }

        // ...and finally move!
        granpa = parent;
        parent = node;
        node   = parent->right;
        sister = parent->left;
    }

    // Erase "parent", which should be RED:
    old_data = parent->data;
    if (granpa == NULL) { tree->root    = parent->left; }
    else                { granpa->right = parent->left; }
    free(parent);
    
    // Before leaving: Make sure that the root is BLACK!
    if (tree->root != NULL) { tree->root->color = BLACK; }

    // And return old_data (which will be NULL unless data was already here)
    return old_data;
}

// Removes all the elements from the tree in linear time. This is faster than
// calling "tree_remove_min(tree)" until "tree_is_empty(tree)" returns "YES".
//
// If you provide a "free_data" function it will be used to free the "data"
// inside each node. If "free_data" is NULL, no "data" will be freed. Use this
// later option when "data" is shared between several data structures but be
// aware that this may cause some memory leaks if you are not carefull.
//
// Most of the time, you can use just "free" as "free_data" function.
// However if your "data" struct contains dinamically allocated data
// you may need to provide a more complex "free_data" function like:
//
//  void free_data(void *ptr) {
//      MyData *data = (MyData *) ptr;
//      free(data->some_array);
//      free(data->some_other_array);
//      free(data);
//  }
//
void rb_tree_remove_all(rb_tree *tree, void (* free_data) (void *)) {

    rb_node *root;
    rb_node *left;
    rb_node *right;

    // Sanity check:
    assert(tree != NULL);

    // Initialize:
    root = tree->root;
    tree->root = NULL;

    // While the tree is not empty:
    while (root != NULL) {

        // Unravel the tree: Rotate right "root" & "left"
        if (root->left != NULL) {
            left        = root->left;
            right       = left->right;
            left->right = root;
            root->left  = right;
            root        = left;

        // Erase the current "root" node:
        } else {
            right = root->right;
            if (free_data != NULL) { free_data(root->data); }
            free(root);
            root = right;
        }
    }
}



// SET FUNCTIONS:

// Returns a rb_tree containing a copy of the union of tree_1 and tree_2.
// It does NOT modify tree_1 or tree_2.
//
// If a given "element" is in both trees it takes the pointer from tree_1.
// Likewise, the new tree stores a pointer to the comparing function of tree_1.
//
// It takes O( MAX{ |tree_1|+|tree_2|, |Union|·Log(|Union|) } ) time.
//
rb_tree *rb_tree_union(const rb_tree *tree_1, const rb_tree *tree_2) {

    rb_tree *tree = NULL;
    rb_node *pred_1;
    rb_node *node_1;
    rb_node *pred_2;
    rb_node *node_2;
    void    *data;
    int      comp;

    // Sanity check:
    assert(tree_1 != NULL);
    assert(tree_2 != NULL);

    // Special case: Both trees are the same
    if (tree_1 == tree_2) { return rb_tree_copy(tree_1); }

    // Create a new tree:
    tree = new_rb_tree(tree_1->comp);
    if (tree == NULL) { return NULL; }

    // Go to the smallest element of tree_1 while right-threading your path:
    node_1 = tree_1->root;
    if (node_1 != NULL) {
        while (node_1->left != NULL) {
            pred_1 = node_1->left;
            while (pred_1->right != NULL) { pred_1 = pred_1->right; }
            pred_1->right = node_1;
            node_1        = node_1->left;
        }
    }

    // Go to the smallest element of tree_2 while right-threading your path:
    node_2 = tree_2->root;
    if (node_2 != NULL) {
        while (node_2->left != NULL) {
            pred_2 = node_2->left;
            while (pred_2->right != NULL) { pred_2 = pred_2->right; }
            pred_2->right = node_2;
            node_2        = node_2->left;
        }
    }

    // Until we have exhausted at least one of the trees:
    while (node_1 != NULL && node_2 != NULL) {

        // compare both nodes:
        comp = (tree->comp)(node_1->data, node_2->data);

        if (comp < 0) {

            // insert node_1->data in tree /////////////////////////////////////
            data = node_1->data;
            data = rb_tree_insert_max(tree, data);
            assert(data == NULL);
            ////////////////////////////////////////////////////////////////////

            // advance node_1 //////////////////////////////////////////////////
            node_1 = node_1->right;
            if (node_1 != NULL) {
                while (node_1->left != NULL) {
                    pred_1 = node_1->left;
                    while (pred_1->right != NULL && pred_1->right != node_1) {
                        pred_1 = pred_1->right;
                    }
                    if (pred_1->right == NULL) {
                        pred_1->right = node_1;
                        node_1        = node_1->left;
                    } else {
                        pred_1->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

        } else if (comp > 0) {

            // insert node_2->data in tree /////////////////////////////////////
            data = node_2->data;
            data = rb_tree_insert_max(tree, data);
            assert(data == NULL);
            ////////////////////////////////////////////////////////////////////

            // advance node_2 //////////////////////////////////////////////////
            node_2 = node_2->right;
            if (node_2 != NULL) {
                while (node_2->left != NULL) {
                    pred_2 = node_2->left;
                    while (pred_2->right != NULL && pred_2->right != node_2) {
                        pred_2 = pred_2->right;
                    }
                    if (pred_2->right == NULL) {
                        pred_2->right = node_2;
                        node_2        = node_2->left;
                    } else {
                        pred_2->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

        } else {

            // insert node_1->data in tree /////////////////////////////////////
            data = node_1->data;
            data = rb_tree_insert_max(tree, data);
            assert(data == NULL);
            ////////////////////////////////////////////////////////////////////

            // advance node_1 //////////////////////////////////////////////////
            node_1 = node_1->right;
            if (node_1 != NULL) {
                while (node_1->left != NULL) {
                    pred_1 = node_1->left;
                    while (pred_1->right != NULL && pred_1->right != node_1) {
                        pred_1 = pred_1->right;
                    }
                    if (pred_1->right == NULL) {
                        pred_1->right = node_1;
                        node_1        = node_1->left;
                    } else {
                        pred_1->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

            // advance node_2 //////////////////////////////////////////////////
            node_2 = node_2->right;
            if (node_2 != NULL) {
                while (node_2->left != NULL) {
                    pred_2 = node_2->left;
                    while (pred_2->right != NULL && pred_2->right != node_2) {
                        pred_2 = pred_2->right;
                    }
                    if (pred_2->right == NULL) {
                        pred_2->right = node_2;
                        node_2        = node_2->left;
                    } else {
                        pred_2->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////
        }
    }

    // Insert all remaining data from tree_1:
    while (node_1 != NULL) {

        // insert node_1->data in tree /////////////////////////////////////////
        data = node_1->data;
        data = rb_tree_insert_max(tree, data);
        assert(data == NULL);
        ////////////////////////////////////////////////////////////////////////

        // advance node_1 //////////////////////////////////////////////////////
        node_1 = node_1->right;
        if (node_1 != NULL) {
            while (node_1->left != NULL) {
                pred_1 = node_1->left;
                while (pred_1->right != NULL && pred_1->right != node_1) {
                    pred_1 = pred_1->right;
                }
                if (pred_1->right == NULL) {
                    pred_1->right = node_1;
                    node_1        = node_1->left;
                } else {
                    pred_1->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }

    // Insert all remaining data from tree_2:
    while (node_2 != NULL) {

        // insert node_2->data in tree /////////////////////////////////////////
        data = node_2->data;
        data = rb_tree_insert_max(tree, data);
        assert(data == NULL);
        ////////////////////////////////////////////////////////////////////////

        // advance node_2 //////////////////////////////////////////////////////
        node_2 = node_2->right;
        if (node_2 != NULL) {
            while (node_2->left != NULL) {
                pred_2 = node_2->left;
                while (pred_2->right != NULL && pred_2->right != node_2) {
                    pred_2 = pred_2->right;
                }
                if (pred_2->right == NULL) {
                    pred_2->right = node_2;
                    node_2        = node_2->left;
                } else {
                    pred_2->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }

    // Return the resulting tree:
    return tree;
}

// Returns a rb_tree containing a copy of the intersection of tree_1 and tree_2.
// It does NOT modify tree_1 or tree_2.
//
// The comparing function and all data pointers are taken from tree_1.
//
// It takes O( MAX{ |tree_1|+|tree_2|, |Intersec|·Log(|Intersec|) } ) time.
//
rb_tree *rb_tree_intersection(const rb_tree *tree_1, const rb_tree *tree_2) {

    rb_tree *tree = NULL;
    rb_node *pred_1;
    rb_node *node_1;
    rb_node *pred_2;
    rb_node *node_2;
    void    *data;
    int      comp;

    // Sanity check:
    assert(tree_1 != NULL);
    assert(tree_2 != NULL);

    // Special case: Both trees are the same
    if (tree_1 == tree_2) { return rb_tree_copy(tree_1); }

    // Create a new tree:    
    tree = new_rb_tree(tree_1->comp);
    if (tree == NULL) { return NULL; }

    // Special case: Some of them is empty
    if (rb_tree_is_empty(tree_1) == YES) { return tree; }
    if (rb_tree_is_empty(tree_2) == YES) { return tree; }

    // Go to the smallest element of tree_1 while right-threading your path:
    node_1 = tree_1->root;
    if (node_1 != NULL) {
        while (node_1->left != NULL) {
            pred_1 = node_1->left;
            while (pred_1->right != NULL) { pred_1 = pred_1->right; }
            pred_1->right = node_1;
            node_1        = node_1->left;
        }
    }

    // Go to the smallest element of tree_2 while right-threading your path:
    node_2 = tree_2->root;
    if (node_2 != NULL) {
        while (node_2->left != NULL) {
            pred_2 = node_2->left;
            while (pred_2->right != NULL) { pred_2 = pred_2->right; }
            pred_2->right = node_2;
            node_2        = node_2->left;
        }
    }

    // Until we have exhausted at least one of the trees:
    while (node_1 != NULL && node_2 != NULL) {

        // compare both nodes:
        comp = (tree->comp)(node_1->data, node_2->data);

        if (comp < 0) {

            // advance node_1 //////////////////////////////////////////////////
            node_1 = node_1->right;
            if (node_1 != NULL) {
                while (node_1->left != NULL) {
                    pred_1 = node_1->left;
                    while (pred_1->right != NULL && pred_1->right != node_1) {
                        pred_1 = pred_1->right;
                    }
                    if (pred_1->right == NULL) {
                        pred_1->right = node_1;
                        node_1        = node_1->left;
                    } else {
                        pred_1->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

        } else if (comp > 0) {

            // advance node_2 //////////////////////////////////////////////////
            node_2 = node_2->right;
            if (node_2 != NULL) {
                while (node_2->left != NULL) {
                    pred_2 = node_2->left;
                    while (pred_2->right != NULL && pred_2->right != node_2) {
                        pred_2 = pred_2->right;
                    }
                    if (pred_2->right == NULL) {
                        pred_2->right = node_2;
                        node_2        = node_2->left;
                    } else {
                        pred_2->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

        } else {

            // insert node_1->data in tree /////////////////////////////////////
            data = node_1->data;
            data = rb_tree_insert_max(tree, data);
            assert(data == NULL);
            ////////////////////////////////////////////////////////////////////

            // advance node_1 //////////////////////////////////////////////////
            node_1 = node_1->right;
            if (node_1 != NULL) {
                while (node_1->left != NULL) {
                    pred_1 = node_1->left;
                    while (pred_1->right != NULL && pred_1->right != node_1) {
                        pred_1 = pred_1->right;
                    }
                    if (pred_1->right == NULL) {
                        pred_1->right = node_1;
                        node_1        = node_1->left;
                    } else {
                        pred_1->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

            // advance node_2 //////////////////////////////////////////////////
            node_2 = node_2->right;
            if (node_2 != NULL) {
                while (node_2->left != NULL) {
                    pred_2 = node_2->left;
                    while (pred_2->right != NULL && pred_2->right != node_2) {
                        pred_2 = pred_2->right;
                    }
                    if (pred_2->right == NULL) {
                        pred_2->right = node_2;
                        node_2        = node_2->left;
                    } else {
                        pred_2->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////
        }
    }

    // We need to traverse tree_1 completely to restore the modified pointers:
    while (node_1 != NULL) {

        // advance node_1 //////////////////////////////////////////////////////
        node_1 = node_1->right;
        if (node_1 != NULL) {
            while (node_1->left != NULL) {
                pred_1 = node_1->left;
                while (pred_1->right != NULL && pred_1->right != node_1) {
                    pred_1 = pred_1->right;
                }
                if (pred_1->right == NULL) {
                    pred_1->right = node_1;
                    node_1        = node_1->left;
                } else {
                    pred_1->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }

    // We need to traverse tree_2 completely to restore the modified pointers:
    while (node_2 != NULL) {

        // advance node_2 //////////////////////////////////////////////////////
        node_2 = node_2->right;
        if (node_2 != NULL) {
            while (node_2->left != NULL) {
                pred_2 = node_2->left;
                while (pred_2->right != NULL && pred_2->right != node_2) {
                    pred_2 = pred_2->right;
                }
                if (pred_2->right == NULL) {
                    pred_2->right = node_2;
                    node_2        = node_2->left;
                } else {
                    pred_2->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }

    // Return the resulting tree:
    return tree;
}

// Returns a rb_tree containing a copy of the difference: tree_1 - tree_2.
// It does NOT modify tree_1 or tree_2.
//
// The comparing function and all data pointers are taken from tree_1.
//
// It takes O( MAX{ |tree_1|+|tree_2|, |Diff|·Log(|Diff|) } ) time.
//
rb_tree *rb_tree_diff(const rb_tree *tree_1, const rb_tree *tree_2) {

    rb_tree *tree = NULL;
    rb_node *pred_1;
    rb_node *node_1;
    rb_node *pred_2;
    rb_node *node_2;
    void    *data;
    int      comp;

    // Sanity check:
    assert(tree_1 != NULL);
    assert(tree_2 != NULL);

    // Create a new tree:
    tree = new_rb_tree(tree_1->comp);
    if (tree == NULL) { return NULL; }

    // Special case: Both trees are the same
    if (tree_1 == tree_2) { return tree; }

    // Special case: tree_1 is empty
    if (rb_tree_is_empty(tree_1) == YES) { return tree; }

    // Go to the smallest element of tree_1 while right-threading your path:
    node_1 = tree_1->root;
    if (node_1 != NULL) {
        while (node_1->left != NULL) {
            pred_1 = node_1->left;
            while (pred_1->right != NULL) { pred_1 = pred_1->right; }
            pred_1->right = node_1;
            node_1        = node_1->left;
        }
    }

    // Go to the smallest element of tree_2 while right-threading your path:
    node_2 = tree_2->root;
    if (node_2 != NULL) {
        while (node_2->left != NULL) {
            pred_2 = node_2->left;
            while (pred_2->right != NULL) { pred_2 = pred_2->right; }
            pred_2->right = node_2;
            node_2        = node_2->left;
        }
    }
    
    // Until we have exhausted at least one of the trees:
    while (node_1 != NULL && node_2 != NULL) {

        // compare both nodes:
        comp = (tree->comp)(node_1->data, node_2->data);

        if (comp < 0) {

            // insert node_1->data in tree /////////////////////////////////////
            data = node_1->data;
            data = rb_tree_insert_max(tree, data);
            assert(data == NULL);
            ////////////////////////////////////////////////////////////////////

            // advance node_1 //////////////////////////////////////////////////
            node_1 = node_1->right;
            if (node_1 != NULL) {
                while (node_1->left != NULL) {
                    pred_1 = node_1->left;
                    while (pred_1->right != NULL && pred_1->right != node_1) {
                        pred_1 = pred_1->right;
                    }
                    if (pred_1->right == NULL) {
                        pred_1->right = node_1;
                        node_1        = node_1->left;
                    } else {
                        pred_1->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

        } else if (comp > 0) {

            // advance node_2 //////////////////////////////////////////////////
            node_2 = node_2->right;
            if (node_2 != NULL) {
                while (node_2->left != NULL) {
                    pred_2 = node_2->left;
                    while (pred_2->right != NULL && pred_2->right != node_2) {
                        pred_2 = pred_2->right;
                    }
                    if (pred_2->right == NULL) {
                        pred_2->right = node_2;
                        node_2        = node_2->left;
                    } else {
                        pred_2->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

        } else {

            // advance node_1 //////////////////////////////////////////////////
            node_1 = node_1->right;
            if (node_1 != NULL) {
                while (node_1->left != NULL) {
                    pred_1 = node_1->left;
                    while (pred_1->right != NULL && pred_1->right != node_1) {
                        pred_1 = pred_1->right;
                    }
                    if (pred_1->right == NULL) {
                        pred_1->right = node_1;
                        node_1        = node_1->left;
                    } else {
                        pred_1->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

            // advance node_2 //////////////////////////////////////////////////
            node_2 = node_2->right;
            if (node_2 != NULL) {
                while (node_2->left != NULL) {
                    pred_2 = node_2->left;
                    while (pred_2->right != NULL && pred_2->right != node_2) {
                        pred_2 = pred_2->right;
                    }
                    if (pred_2->right == NULL) {
                        pred_2->right = node_2;
                        node_2        = node_2->left;
                    } else {
                        pred_2->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////
        }
    }

    // Insert all remaining data from tree_1:
    while (node_1 != NULL) {

        // insert node_1->data in tree /////////////////////////////////////////
        data = node_1->data;
        data = rb_tree_insert_max(tree, data);
        assert(data == NULL);
        ////////////////////////////////////////////////////////////////////////

        // advance node_1 //////////////////////////////////////////////////////
        node_1 = node_1->right;
        if (node_1 != NULL) {
            while (node_1->left != NULL) {
                pred_1 = node_1->left;
                while (pred_1->right != NULL && pred_1->right != node_1) {
                    pred_1 = pred_1->right;
                }
                if (pred_1->right == NULL) {
                    pred_1->right = node_1;
                    node_1        = node_1->left;
                } else {
                    pred_1->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }

    // We need to traverse tree_2 completely to restore the modified pointers:
    while (node_2 != NULL) {

        // advance node_2 //////////////////////////////////////////////////////
        node_2 = node_2->right;
        if (node_2 != NULL) {
            while (node_2->left != NULL) {
                pred_2 = node_2->left;
                while (pred_2->right != NULL && pred_2->right != node_2) {
                    pred_2 = pred_2->right;
                }
                if (pred_2->right == NULL) {
                    pred_2->right = node_2;
                    node_2        = node_2->left;
                } else {
                    pred_2->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }

    // Return the resulting tree:
    return tree;
}

// Returns a rb_tree containing a copy of the symmetric difference of tree_1
// and tree_2. It does NOT modify tree_1 or tree_2.
//
// The comparing function is taken from tree_1.
//
// It takes O( MAX{ |tree_1|+|tree_2|, |SymDiff|·Log(|SymDiff|) } ) time.
//
rb_tree *rb_tree_sym_diff(const rb_tree *tree_1, const rb_tree *tree_2) {

    rb_tree *tree = NULL;
    rb_node *pred_1;
    rb_node *node_1;
    rb_node *pred_2;
    rb_node *node_2;
    void    *data;
    int      comp;

    // Sanity check:
    assert(tree_1 != NULL);
    assert(tree_2 != NULL);

    // Create a new tree:
    tree = new_rb_tree(tree_1->comp);
    if (tree == NULL) { return NULL; }

    // Special case: Both trees are the same
    if (tree_1 == tree_2) { return tree; }

    // Go to the smallest element of tree_1 while right-threading your path:
    node_1 = tree_1->root;
    if (node_1 != NULL) {
        while (node_1->left != NULL) {
            pred_1 = node_1->left;
            while (pred_1->right != NULL) { pred_1 = pred_1->right; }
            pred_1->right = node_1;
            node_1        = node_1->left;
        }
    }

    // Go to the smallest element of tree_2 while right-threading your path:
    node_2 = tree_2->root;
    if (node_2 != NULL) {
        while (node_2->left != NULL) {
            pred_2 = node_2->left;
            while (pred_2->right != NULL) { pred_2 = pred_2->right; }
            pred_2->right = node_2;
            node_2        = node_2->left;
        }
    }

    // Until we have exhausted at least one of the trees:
    while (node_1 != NULL && node_2 != NULL) {

        // compare both nodes:
        comp = (tree->comp)(node_1->data, node_2->data);

        if (comp < 0) {

            // insert node_1->data in tree /////////////////////////////////////
            data = node_1->data;
            data = rb_tree_insert_max(tree, data);
            assert(data == NULL);
            ////////////////////////////////////////////////////////////////////

            // advance node_1 //////////////////////////////////////////////////
            node_1 = node_1->right;
            if (node_1 != NULL) {
                while (node_1->left != NULL) {
                    pred_1 = node_1->left;
                    while (pred_1->right != NULL && pred_1->right != node_1) {
                        pred_1 = pred_1->right;
                    }
                    if (pred_1->right == NULL) {
                        pred_1->right = node_1;
                        node_1        = node_1->left;
                    } else {
                        pred_1->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

        } else if (comp > 0) {

            // insert node_2->data in tree /////////////////////////////////////
            data = node_2->data;
            data = rb_tree_insert_max(tree, data);
            assert(data == NULL);
            ////////////////////////////////////////////////////////////////////

            // advance node_2 //////////////////////////////////////////////////
            node_2 = node_2->right;
            if (node_2 != NULL) {
                while (node_2->left != NULL) {
                    pred_2 = node_2->left;
                    while (pred_2->right != NULL && pred_2->right != node_2) {
                        pred_2 = pred_2->right;
                    }
                    if (pred_2->right == NULL) {
                        pred_2->right = node_2;
                        node_2        = node_2->left;
                    } else {
                        pred_2->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

        } else {

            // advance node_1 //////////////////////////////////////////////////
            node_1 = node_1->right;
            if (node_1 != NULL) {
                while (node_1->left != NULL) {
                    pred_1 = node_1->left;
                    while (pred_1->right != NULL && pred_1->right != node_1) {
                        pred_1 = pred_1->right;
                    }
                    if (pred_1->right == NULL) {
                        pred_1->right = node_1;
                        node_1        = node_1->left;
                    } else {
                        pred_1->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////

            // advance node_2 //////////////////////////////////////////////////
            node_2 = node_2->right;
            if (node_2 != NULL) {
                while (node_2->left != NULL) {
                    pred_2 = node_2->left;
                    while (pred_2->right != NULL && pred_2->right != node_2) {
                        pred_2 = pred_2->right;
                    }
                    if (pred_2->right == NULL) {
                        pred_2->right = node_2;
                        node_2        = node_2->left;
                    } else {
                        pred_2->right = NULL;
                        break;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////
        }
    }

    // Insert all remaining data from tree_1:
    while (node_1 != NULL) {

        // insert node_1->data in tree /////////////////////////////////////////
        data = node_1->data;
        data = rb_tree_insert_max(tree, data);
        assert(data == NULL);
        ////////////////////////////////////////////////////////////////////////

        // advance node_1 //////////////////////////////////////////////////////
        node_1 = node_1->right;
        if (node_1 != NULL) {
            while (node_1->left != NULL) {
                pred_1 = node_1->left;
                while (pred_1->right != NULL && pred_1->right != node_1) {
                    pred_1 = pred_1->right;
                }
                if (pred_1->right == NULL) {
                    pred_1->right = node_1;
                    node_1        = node_1->left;
                } else {
                    pred_1->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }

    // Insert all remaining data from tree_2:
    while (node_2 != NULL) {

        // insert node_2->data in tree /////////////////////////////////////////
        data = node_2->data;
        data = rb_tree_insert_max(tree, data);
        assert(data == NULL);
        ////////////////////////////////////////////////////////////////////////

        // advance node_2 //////////////////////////////////////////////////////
        node_2 = node_2->right;
        if (node_2 != NULL) {
            while (node_2->left != NULL) {
                pred_2 = node_2->left;
                while (pred_2->right != NULL && pred_2->right != node_2) {
                    pred_2 = pred_2->right;
                }
                if (pred_2->right == NULL) {
                    pred_2->right = node_2;
                    node_2        = node_2->left;
                } else {
                    pred_2->right = NULL;
                    break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
    }

    // Return the resulting tree:
    return tree;
}


// DEBUG & VISUALIZATION:

// This is an auxiliary function to check the symmetric order property
// recursively. You should not use it directly, use "is_rb_tree" instead.
//
// Note that, unlike other functions of this library, it is safe to use
// recursive definitions here since they will only be used to test the
// code while debugging this library.
//
static int is_rb_subtree(const rb_tree *tree, const rb_node *node,
                         const void *min, const void *max) {

    int left_height  = 0;
    int right_height = 0;

    // Make sure that node is (strictly) between specified limits:
    if (min != NULL && (tree->comp)(min, node->data) >= 0) {
        fprintf(stderr, "ERROR: Symmetric order not satisfied in rb_tree\n");
        return -1;
    }
    if (max != NULL && (tree->comp)(node->data, max) >= 0) {
        fprintf(stderr, "ERROR: Symmetric order not satisfied in rb_tree\n");
        return -1;
    }

    // Check for RED violations:
    if (node->color == RED) {
        if (node->left  != NULL && node->left->color  == RED) {
            fprintf(stderr, "ERROR: Two RED nodes in a row in rb_tree\n");
            return -1;
        }
        if (node->right != NULL && node->right->color == RED) {
            fprintf(stderr, "ERROR: Two RED nodes in a row in rb_tree\n");
            return -1;
        }
    }

    // Check recursively the left subtree of node:
    if (node->left != NULL) {
        left_height = is_rb_subtree(tree, node->left, min, node->data);
        if (left_height == -1) { return -1; }
    }

    // Check recursively the right subtree of node:
    if (node->right != NULL) {
        right_height = is_rb_subtree(tree, node->right, node->data, max);
        if (right_height == -1) { return -1; }
    }

    // Check for BLACK violations:
    if (left_height != right_height) {
        fprintf(stderr, "ERROR: Different BLACK height in rb_tree\n");
        return -1;
    }
    
    // Return Black-Height of "node":
    if (node->color == RED){ return left_height; }
    return left_height+1;
}

// This is an auxiliary function to check the symmetric order property, the
// RED property and the BLACK property of a red-black tree.
// Returns YES if everything is correct and NO otherwise.
//
// This function should not be used in production code. I recommend to use:
//
//      assert(is_rb_tree(tree) == YES);
//
// To automatically remove all calls to this function when the flag NDEBUG
// is defined in the header files (deactivating all assertions).
//
int is_rb_tree(const rb_tree *tree) {

    // Basic Sanity Checks:
    if (tree == NULL) {
        fprintf(stderr, "ERROR: NULL pointer to rb_tree\n");
        return NO;
    }
    if (tree->comp == NULL) {
        fprintf(stderr, "ERROR: NULL comparing function in rb_tree\n");
        return NO;
    }

    // Trivial Case: empty tree
    if (tree->root == NULL) { return YES; }

    // General Case:
    if (is_rb_subtree(tree, tree->root, NULL, NULL) == -1) { return NO;  }
    else                                                   { return YES; }
}

// This is an auxiliary function to print the tree recursively.
// You should not use it directly, use "print_rb_tree" instead.
//
// Note that, unlike the rest of the functions of this library, it is safe
// to use recursive definitions here since any tree large enough to hit the
// recursion limit will be too big to be printed anyway.
//
static void print_rb_subtree(const rb_node *node, const int is_right,
                             char *indent, void (* print_node) (const void *)) {

    // Allocate memory:
    char *new_indent = (char *) malloc((6+strlen(indent)*sizeof(char)));
    assert(new_indent != NULL);

    // Print right subtree recursively:
    if (node->right != NULL) {
        if (is_right == YES) { sprintf(new_indent, "%s%s", indent, "      "); }
        else {
            if (node->color == RED) {
                sprintf(new_indent, "%s%s", indent, "||    ");
            } else {
                sprintf(new_indent, "%s%s", indent, "|     ");
            }
        }
        print_rb_subtree(node->right, YES, new_indent, print_node);
    }

    // Print current node:
    if (node->color == RED) {
        if (is_right) { fprintf(stdout, "%s/====",indent); }
        else          { fprintf(stdout, "%s\\====",indent); }
    } else {
        if (is_right) { fprintf(stdout, "%s,----",indent); }
        else          { fprintf(stdout, "%s`----",indent); }
    }
    if (print_node == NULL)  {
        if (node->color == RED) { fprintf(stdout, "(#)"); }
        else                    { fprintf(stdout, "( )"); }
    } else { print_node(node->data); }
    fprintf(stdout, "\n");

    // Print left subtree recursively:
    if (node->left != NULL) {
        if (is_right == YES) {
            if (node->color == RED) {
                sprintf(new_indent, "%s%s", indent, "||    ");
            } else {
                sprintf(new_indent, "%s%s", indent, "|     ");
            }
        } else { sprintf(new_indent, "%s%s", indent, "      "); }
        print_rb_subtree(node->left, NO, new_indent, print_node);
    }

    // Free memory:
    free(new_indent);
}

// This function is used to print a red-black tree on the screen.
//
//  * If a "print_node" function is provided it will be used to print the
//    content of each node.
//  * Otherwise (if print_node is NULL) RED nodes will be printed as (#) and
//    BLACK nodes will be printed as ( ).
//
// Use this latter option if you only want to take a look at the tree shape.
//
// A correct print_node function only prints a single line of text using
// the "fprintf" command. It should NOT include any "\n" in that line.
//
// For example:
//
//  void MyPrintNodeFunction(const void *ptr) {
//      MyDataStruct *d = (MyDataStruct *) ptr;     // Pointer casting
//      fprintf(stdout, "(%d)", d->key);            // Printing integer key
//  }
//
// Finally, note that this is a visualization tool for debugging purposes only.
//
void print_rb_tree(const rb_tree *tree, void (* print_node) (const void *)) {

    // Avoid the trivial cases:
    if (tree != NULL && tree->root != NULL) {

        // Print right subtree recursively:
        if (tree->root->right != NULL) {
            print_rb_subtree(tree->root->right, YES, "     ", print_node);
        }

        // Print current node:
        if (tree->root->color == RED) { fprintf(stdout, "===="); }
        else                          { fprintf(stdout, "----"); }
        if (print_node == NULL) {
            if (tree->root->color == RED) { fprintf(stdout, "(#)"); }
            else                          { fprintf(stdout, "( )"); }
        } else { print_node(tree->root->data); }
        fprintf(stdout, "\n");

        // Print left subtree recursively:
        if (tree->root->left != NULL) {
            print_rb_subtree(tree->root->left, NO, "     ", print_node);
        }
    }

    // Empty the "stdout" buffer:
    fflush(stdout);
}

// END OF RED BLACK TREES //////////////////////////////////////////////////////





// SPLAY TREES /////////////////////////////////////////////////////////////////


// SPLAYING FUNCTIONS:

// Moves the node with "data" to the root of the splay tree.
//
// If data is not found moves the last node found in the search path to "data".
//
static inline void splay(sp_tree *tree, const void *data) {

    sp_node  root;
    sp_node *left;
    sp_node *right;
    sp_node *node;
    sp_node *temp;
    int      comp;

    // Sanity checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Initialize:
    root.right = root.left = NULL;
    left = right = &root;
    node = tree->root;
    if (node == NULL) { return; }

    for (;;) {

        // Compare "data":
        comp = (tree->comp)(data, node->data);  

        // If "data" is smaller:
        if (comp < 0) {                         
            if (node->left == NULL) { break; }

            // Rotate right if needed:
            if ((tree->comp)(data, node->left->data) < 0) {
                temp        = node->left;
                node->left  = temp->right;
                temp->right = node;
                node        = temp;
                if (node->left == NULL) { break; }
            }

            // Link right:
            right->left = node;
            right       = node;
            node        = node->left;

        // If "data" is bigger:
        } else if (comp > 0) {
            if (node->right == NULL) { break; }

            // Rotate left if needed:
            if ((tree->comp)(data, node->right->data) > 0) {
                temp        = node->right;
                node->right = temp->left;
                temp->left  = node;
                node        = temp;
                if (node->right == NULL) { break; }
            }

            // Link left:
            left->right = node;
            left        = node;
            node        = node->right;

        // If we have found "data":
        } else { break; }
    }

    // Assemble:
    left->right = node->left;
    right->left = node->right;
    node->left  = root.right;
    node->right = root.left;
    tree->root  = node;
}

// Moves the node with the smallest element to the root of the splay tree.
//
static inline void splay_left(sp_tree *tree) {

    sp_node  root;
    sp_node *right;
    sp_node *node;
    sp_node *temp;

    // Sanity check:
    assert(tree != NULL);

    // Initialize:
    root.right = root.left = NULL;
    right = &root;
    node = tree->root;
    if (node == NULL) { return; }

    for (;;) {
        
        // Act as if splaying "data" which is always smaller
        if (node->left == NULL) { break; }

        // Rotate right:
        temp        = node->left;
        node->left  = temp->right;
        temp->right = node;
        node        = temp;
        if (node->left == NULL) { break; }

        // Link right:
        right->left = node;
        right       = node;
        node        = node->left;
    }

    // Assemble:
    right->left = node->right;
    node->right = root.left;
    tree->root  = node;
}

// Moves the node with the biggest element to the root of the splay tree.
//
static inline void splay_right(sp_tree *tree) {

    sp_node  root;
    sp_node *left;
    sp_node *node;
    sp_node *temp;

    // Sanity check:
    assert(tree != NULL);

    // Initialize:
    root.right = root.left = NULL;
    left = &root;
    node = tree->root;
    if (node == NULL) { return; }

    for (;;) {
        
        // Act as if splaying "data" which is always bigger
        if (node->right == NULL) { break; }

        // Rotate left:
        temp        = node->right;
        node->right = temp->left;
        temp->left  = node;
        node        = temp;
        if (node->right == NULL) { break; }

        // Link left:
        left->right = node;
        left        = node;
        node        = node->right;
    }

    // Assemble:
    left->right = node->left;
    node->left  = root.right;
    tree->root  = node;
}



// CREATION & INSERTION:

// Returns a pointer to a newly created sp_tree.
// You must provide a comparing function "comp" such that:
//  * comp(A,B) = 0  if and only if  A = B
//  * comp(A,B) > 0  if and only if  A > B
//  * comp(A,B) < 0  if and only if  A < B
//
// I recommend to limit the output of the "comp" function to {-1, 0, 1} as in:
//
//  int MyComp(const void *ptr1, const void *ptr2) {
//      MyData *d1 = (MyData *) ptr1;
//      MyData *d2 = (MyData *) ptr2;
//      if      (d1->key < d2->key) { return -1; }
//      else if (d1->key > d2->key) { return +1; }
//      else                        { return  0; }
//  }
//
// However, the code does not assume that so, if you are working with simple
// numeric keys, you can just use something like:
//
//  int MyComp(const void *ptr1, const void *ptr2) {
//      MyData *d1 = (MyData *) ptr1;
//      MyData *d2 = (MyData *) ptr2;
//      return (int) ((d1->key) - (d2->key));
//  }
//
sp_tree *new_sp_tree(int (* comp) (const void *, const void *)) {

    // Sanity check:
    assert(comp != NULL);

    // Allocate memory:
    sp_tree *tree = (sp_tree *) malloc(sizeof(sp_tree));
    if (tree == NULL) {
        fprintf(stderr, "ERROR: Unable to allocate memory for sp_tree\n");
    }

    // Initialize the empty tree:
    else {
        tree->root = NULL;
        tree->comp = comp;
    }

    return tree;
}

// Returns a (really degenerated) splay tree containing a copy of tree.
// It does NOT modify the content of tree but can modify its shape.
//
// Takes O(|Tree|) time.
//
sp_tree *sp_tree_copy(sp_tree *tree) {

    sp_tree *new_tree;
    void    *data;
    
    // Sanity check:
    assert(tree != NULL);
    
    // Create a new tree:
    new_tree = new_sp_tree(tree->comp);
    if (tree == NULL) { return NULL; }

    // Get the smallest element of tree:
    data = sp_tree_min(tree);

    // Insert all data from tree into new_tree:
    while (data != NULL) {

        // insert data in new_tree:
        sp_tree_insert_max(new_tree, data);

        // advance data:
        data = sp_tree_next(tree, data);
    }

    // Return the resulting tree:
    return new_tree;
}

// Inserts data in tree.
//
// If a node of the tree compares "equal" to data it will get replaced and a
// pointer to the previously stored data will be returned (so you can free it),
// otherwise it will simply return a NULL pointer.
//
void *sp_tree_insert(sp_tree *tree, void *data) {

    sp_node *old_root;
    void    *old_data;
    int      comp = 1;

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Trivial case: Empty tree
    if (tree->root == NULL) {
        tree->root = (sp_node *) malloc(sizeof(sp_node));
        if (tree->root == NULL) {
            fprintf(stderr, "ERROR: Unable to allocate sp_node\n");
        } else {
            tree->root->data  = data;
            tree->root->left  = NULL;
            tree->root->right = NULL;
        }
        return NULL;
    }

    // General case: Splay data to the root
    splay(tree, data);
    old_root = tree->root;

    // Compare the current root with data:
    if (old_root != NULL) { comp = (tree->comp)(data, old_root->data); }

    // If data is in the tree: overwrite it!
    if (comp == 0) {
        old_data = tree->root->data;
        tree->root->data = data;
        return old_data;
    }

    // Otherwise insert a new node as a root and link the previous root to it:
    tree->root = (sp_node *) malloc(sizeof(sp_node));
    if (tree->root == NULL) {
        fprintf(stderr, "ERROR: Unable to allocate sp_node\n");
        tree->root = old_root;
    } else {
        tree->root->data  = data;
        if (comp > 0) {
            tree->root->left  = old_root;
            tree->root->right = old_root->right;
            old_root->right   = NULL;
        } else {
            tree->root->right = old_root;
            tree->root->left  = old_root->left;
            old_root->left    = NULL;
        }
    }

    // Data was not here!
    return NULL;
}

// Inserts "data" such that "data" is smaller or equal to any other "data"
// already in the tree. It is slightly faster than a regular insert because it
// makes at most one comparison.
//
// WARNING: If you use this function to insert data that is strictly bigger than
//          something already in the tree you will break the tree!
//
// If a node of the tree compares "equal" to data it will get replaced and a
// pointer to the previously stored data will be returned (so you can free it),
// otherwise it will simply return a NULL pointer.
//
void *sp_tree_insert_min(sp_tree *tree, void *data) {

    sp_node *old_root;
    void    *old_data;
    int      comp = 1;
    
    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // General case: Splay data to the root
    splay_left(tree);
    old_root = tree->root;

    // Compare the current root with data:
    if (old_root != NULL) { comp = (tree->comp)(data, old_root->data); }

    // If data is in the tree: overwrite it!
    if (comp == 0) {
        old_data = tree->root->data;
        tree->root->data = data;
        return old_data;
    }

    // Otherwise insert a new node as a root and link the previous root to it:
    tree->root = (sp_node *) malloc(sizeof(sp_node));
    if (tree->root == NULL) {
        fprintf(stderr, "ERROR: Unable to allocate sp_node\n");
        tree->root = old_root;
    } else {
        tree->root->data  = data;
        tree->root->left  = NULL;
        tree->root->right = old_root;
    }

    // Data was not here!
    return NULL;
}

// Inserts "data" such that "data" is bigger or equal to any other "data"
// already in the tree. It is slightly faster than a regular insert because it
// makes at most one comparison.
//
// WARNING: If you use this function to insert data that is strictly smaller
//          than something already in the tree you will break the tree!
//
// If a node of the tree compares "equal" to data it will get replaced and a
// pointer to the previously stored data will be returned (so you can free it),
// otherwise it will simply return a NULL pointer.
//
void *sp_tree_insert_max(sp_tree *tree, void *data) {

    sp_node *old_root;
    void    *old_data;
    int      comp = 1;
    
    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // General case: Splay data to the root
    splay_right(tree);
    old_root = tree->root;

    // Compare the current root with data:
    if (old_root != NULL) { comp = (tree->comp)(data, old_root->data); }

    // If data is in the tree: overwrite it!
    if (comp == 0) {
        old_data = tree->root->data;
        tree->root->data = data;
        return old_data;
    }

    // Otherwise insert a new node as a root and link the previous root to it:
    tree->root = (sp_node *) malloc(sizeof(sp_node));
    if (tree->root == NULL) {
        fprintf(stderr, "ERROR: Unable to allocate sp_node\n");
        tree->root = old_root;
    } else {
        tree->root->data  = data;
        tree->root->left  = old_root;
        tree->root->right = NULL;
    }

    // Data was not here!
    return NULL;
}



// SEARCH:

// Returns YES if the tree is empty and NO otherwise.
//
int sp_tree_is_empty(const sp_tree *tree) {

    // Sanity check:
    assert(tree != NULL);

    // Check if there is at least 1 node in the tree:
    if (tree->root == NULL) { return YES; }
    else                    { return NO;  }
}

// Finds a node that compares "equal" to data. Returns NULL if not found.
//
void *sp_tree_search(sp_tree *tree, const void *data) {

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Trivial case: empty tree
    if (tree->root == NULL) { return NULL; }

    // General case: Splay data to the root
    splay(tree, data);

    // If data is in the tree return a pointer to it:
    if ((tree->comp)(data, tree->root->data) == 0) { return tree->root->data; }

    // Not found:
    return NULL;
}

// Returns a pointer to the smallest element stored in the tree.
// Returns NULL if the tree is empty.
//
void *sp_tree_min(sp_tree *tree) {

    // Sanity Check:
    assert(tree != NULL);

    // Trivial case: empty tree
    if (tree->root == NULL) { return NULL; }

    // General case: Move the smallest element to the root
    splay_left(tree);

    // And return a pointer to its data:
    return tree->root->data;
}

// Returns a pointer to the biggest element stored in "tree".
// Returns NULL if the tree is empty.
//
void *sp_tree_max(sp_tree *tree) {

    // Sanity Check:
    assert(tree != NULL);

    // Trivial case: empty tree
    if (tree->root == NULL) { return NULL; }

    // General case: Move the biggest element to the root
    splay_right(tree);

    // And return a pointer to its data:
    return tree->root->data;
}

// Find the in-order predesor of data in the tree.
//
// If data is not in tree returns the biggest element of tree smaller than data.
// If data is smaller or equal to all elements of tree returns NULL.
//
void *sp_tree_prev(sp_tree *tree, const void *data) {

    sp_node *old_root;
    int      comp;

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Trivial case: empty tree
    if (tree->root == NULL) { return NULL; }

    // Trivial case: null data == return maximum
    if (data == NULL) {
        splay_right(tree);
        return tree->root->data;
    }

    // General case: Splay data to the root
    splay(tree, data);

    // Take a look at the current root:
    comp = (tree->comp)(tree->root->data, data);

    // If its bigger or equal we must find the predecessor in the left subtree:    
    if (comp >= 0) {
        if (tree->root->left == NULL) { return NULL; }
        else {
            old_root   = tree->root;
            tree->root = tree->root->left;
            old_root->left = NULL;
            splay_right(tree);
            tree->root->right = old_root;
        }
    }

    // Predecessor is at root now:
    return tree->root->data;
}

// Find the in-order successor of data in the tree.
//
// If data is not in tree returns the smallest element of tree bigger than data.
// If data is bigger or equal to all elements of tree returns NULL.
//
void *sp_tree_next(sp_tree *tree, const void *data) {

    sp_node *old_root;
    int      comp;

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Trivial case: empty tree
    if (tree->root == NULL) { return NULL; }

    // Trivial case: null data == return minimum
    if (data == NULL) {
        splay_left(tree);
        return tree->root->data;
    }

    // General case: Splay data to the root
    splay(tree, data);

    // Take a look at the current root:
    comp = (tree->comp)(tree->root->data, data);

    // If its smaller or equal we must find the successor in the right subtree:
    if (comp <= 0) {
        if (tree->root->right == NULL) { return NULL; }
        else {
            old_root   = tree->root;
            tree->root = tree->root->right;
            old_root->right = NULL;
            splay_left(tree);
            tree->root->left = old_root;
        }
    }

    // Predecessor is at root now:
    return tree->root->data;
}



// REMOVE:

// Removes a node of tree that compares "equal" to data and returns a pointer
// to the previously stored data (so you can free it).
// If such a node is not found, it returns a NULL pointer.
//
void *sp_tree_remove(sp_tree *tree, const void *data) {

    sp_node *old_root;
    void    *old_data = NULL;

    // Sanity Checks:
    assert(tree != NULL);
    assert(data != NULL);

    // Avoid trivial case: empty tree
    if (tree->root != NULL) {

        // Splay the data to the root:
        splay(tree, data);

        // If it is here: remove it!
        if ((tree->comp)(tree->root->data, data) == 0) {
            old_root = tree->root;
            old_data = tree->root->data;
            if (tree->root->right == NULL) { tree->root = tree->root->left; }
            else {
                tree->root = tree->root->right;
                splay_left(tree);
                tree->root->left = old_root->left;
            }
            free(old_root);
        }
    }

    // Return
    return old_data;
}

// Removes the smallest element from "tree" and returns a pointer to its data
// (so you can free it). If the tree is empty returns a NULL pointer.
//
void *sp_tree_remove_min(sp_tree *tree) {

    sp_node *old_root;
    void    *old_data;

    // Sanity check:
    assert(tree != NULL);

    // Avoid trivial case: empty tree
    old_data = NULL;
    if (tree->root != NULL) {

        // Splay the smallest node to the root:
        splay_left(tree);

        // Remove it!
        old_root   = tree->root;
        old_data   = tree->root->data;
        tree->root = tree->root->right;
        free(old_root);
    }

    // Return
    return old_data;
}

// Removes the biggest element from "tree" and returns a pointer to its data
// (so you can free it). If the tree is empty returns a NULL pointer.
//
void *sp_tree_remove_max(sp_tree *tree) {

    sp_node *old_root;
    void    *old_data;

    // Sanity check:
    assert(tree != NULL);

    // Avoid trivial case: empty tree
    old_data = NULL;
    if (tree->root != NULL) {

        // Splay the biggest node to the root:
        splay_right(tree);

        // Remove it!
        old_root   = tree->root;
        old_data   = tree->root->data;
        tree->root = tree->root->left;
        free(old_root);
    }

    // Return
    return old_data;
}

// Removes all the elements from the tree in linear time. This is faster than
// calling "tree_remove_min(tree)" until "tree_is_empty(tree)" returns "YES".
//
// If you provide a "free_data" function it will be used to free the "data"
// inside each node. If "free_data" is NULL, no "data" will be freed. Use this
// later option when "data" is shared between several data structures but be
// aware that this may cause some memory leaks if you are not carefull.
//
// Most of the time, you can use just "free" as "free_data" function.
// However if your "data" struct contains dinamically allocated data
// you may need to provide a more complex "free_data" function like:
//
//  void free_data(void *ptr) {
//      MyData *data = (MyData *) ptr;
//      free(data->some_array);
//      free(data->some_other_array);
//      free(data);
//  }
//
void sp_tree_remove_all(sp_tree *tree, void (* free_data) (void *)) {

    sp_node *root;
    sp_node *left;
    sp_node *right;

    // Sanity check:
    assert(tree != NULL);

    // Initialize:
    root = tree->root;
    tree->root = NULL;

    // While the tree is not empty:
    while (root != NULL) {

        // Unravel the tree: Rotate right "root" & "left"
        if (root->left != NULL) {
            left        = root->left;
            right       = left->right;
            left->right = root;
            root->left  = right;
            root        = left;

        // Erase the current "root" node:
        } else {
            right = root->right;
            if (free_data != NULL) { free_data(root->data); }
            free(root);
            root = right;
        }
    }
}



// SET FUNCTIONS:

// Returns a (really degenerated) splay tree containing a copy of the union of
// tree_1 and tree_2. It does NOT modify the content of tree_1 or tree_2 but
// can modify their shape.
//
// If a given "element" is in both trees it takes the pointer from tree_1.
// Likewise, the new tree stores a pointer to the comparing function of tree_1.
//
sp_tree *sp_tree_union(sp_tree *tree_1, sp_tree *tree_2) {

    sp_tree *tree;
    void    *data_1;
    void    *data_2;
    int      comp;

    // Sanity check:
    assert(tree_1 != NULL);
    assert(tree_2 != NULL);

    // Special case: Both trees are the same
    if (tree_1 == tree_2) { return sp_tree_copy(tree_1); }

    // Create a new tree:
    tree = new_sp_tree(tree_1->comp);
    if (tree == NULL) { return NULL; }

    // Get the smallest element of each tree:
    data_1 = sp_tree_min(tree_1);
    data_2 = sp_tree_min(tree_2);

    // Until we have exhausted at least one of the trees:
    while (data_1 != NULL && data_2 != NULL) {

        // compare both nodes:
        comp = (tree->comp)(data_1, data_2);

        if (comp < 0) {

            // insert data_1 in tree:
            sp_tree_insert_max(tree, data_1);

            // advance data_1:
            data_1 = sp_tree_next(tree_1, data_1);

        } else if (comp > 0) {

            // insert data_2 in tree:
            sp_tree_insert_max(tree, data_2);

            // advance data_2:
            data_2 = sp_tree_next(tree_2, data_2);

        } else {

            // insert data_1 in tree:
            sp_tree_insert_max(tree, data_1);

            // advance data_1:
            data_1 = sp_tree_next(tree_1, data_1);

            // advance data_2:
            data_2 = sp_tree_next(tree_2, data_2);

        }
    }

    // Insert all remaining data from tree_1:
    while (data_1 != NULL) {

        // insert data_1 in tree:
        sp_tree_insert_max(tree, data_1);

        // advance data_1:
        data_1 = sp_tree_next(tree_1, data_1);
    }

    // Insert all remaining data from tree_2:
    while (data_2 != NULL) {

        // insert data_2 in tree:
        sp_tree_insert_max(tree, data_2);

        // advance data_2:
        data_2 = sp_tree_next(tree_2, data_2);
    }

    // Return the resulting tree:
    return tree;
}

// Returns a (really degenerated) splay tree containing a copy of the
// intersection of tree_1 and tree_2. It does NOT modify the content of tree_1
// or tree_2 but can modify their shape.
//
// The comparing function and all data pointers are taken from tree_1.
//
sp_tree *sp_tree_intersection(sp_tree *tree_1, sp_tree *tree_2) {

    sp_tree *tree;
    void    *data_1;
    void    *data_2;
    int      comp;

    // Sanity check:
    assert(tree_1 != NULL);
    assert(tree_2 != NULL);

    // Special case: Both trees are the same
    if (tree_1 == tree_2) { return sp_tree_copy(tree_1); }

    // Create a new tree:
    tree = new_sp_tree(tree_1->comp);
    if (tree == NULL) { return NULL; }

    // Special case: Some of them is empty
    if (sp_tree_is_empty(tree_1) == YES) { return tree; }
    if (sp_tree_is_empty(tree_2) == YES) { return tree; }
    
    // Get the smallest element of each tree:
    data_1 = sp_tree_min(tree_1);
    data_2 = sp_tree_min(tree_2);

    // Until we have exhausted at least one of the trees:
    while (data_1 != NULL && data_2 != NULL) {

        // compare both nodes:
        comp = (tree->comp)(data_1, data_2);

        if (comp < 0) {

            // advance data_1:
            data_1 = sp_tree_next(tree_1, data_1);

        } else if (comp > 0) {

            // advance data_2:
            data_2 = sp_tree_next(tree_2, data_2);

        } else {

            // insert data_1 in tree:
            sp_tree_insert_max(tree, data_1);

            // advance data_1:
            data_1 = sp_tree_next(tree_1, data_1);

            // advance data_2:
            data_2 = sp_tree_next(tree_2, data_2);

        }
    }

    // Return the resulting tree:
    return tree;
}

// Returns a (really degenerated) splay tree containing a copy of the
// difference: tree_1 - tree_2. It does NOT modify the content of tree_1 or
// tree_2 but can modify their shape.
//
// The comparing function and all data pointers are taken from tree_1.
//
sp_tree *sp_tree_diff(sp_tree *tree_1, sp_tree *tree_2) {
    
    sp_tree *tree;
    void    *data_1;
    void    *data_2;
    int      comp;

    // Sanity check:
    assert(tree_1 != NULL);
    assert(tree_2 != NULL);

    // Create a new tree:
    tree = new_sp_tree(tree_1->comp);
    if (tree == NULL) { return NULL; }

    // Special case: Both trees are the same
    if (tree_1 == tree_2) { return tree; }

    // Special case: Some of them is empty
    if (sp_tree_is_empty(tree_1) == YES) { return tree; }

    // Get the smallest element of each tree:
    data_1 = sp_tree_min(tree_1);
    data_2 = sp_tree_min(tree_2);

    // Until we have exhausted at least one of the trees:
    while (data_1 != NULL && data_2 != NULL) {

        // compare both nodes:
        comp = (tree->comp)(data_1, data_2);

        if (comp < 0) {

            // insert data_1 in tree:
            sp_tree_insert_max(tree, data_1);

            // advance data_1:
            data_1 = sp_tree_next(tree_1, data_1);

        } else if (comp > 0) {

            // advance data_2:
            data_2 = sp_tree_next(tree_2, data_2);

        } else {

            // advance data_1:
            data_1 = sp_tree_next(tree_1, data_1);

            // advance data_2:
            data_2 = sp_tree_next(tree_2, data_2);

        }
    }

    // Insert all remaining data from tree_1:
    while (data_1 != NULL) {

        // insert data_1 in tree:
        sp_tree_insert_max(tree, data_1);

        // advance data_1:
        data_1 = sp_tree_next(tree_1, data_1);
    }

    // Return the resulting tree:
    return tree;
}

// Returns a (really degenerated) splay tree containing a copy of the symmetric
// difference of tree_1 and tree_2. It does NOT modify the content of tree_1 or
// tree_2 but can modify their shape.
//
// The comparing function is taken from tree_1.
//
sp_tree *sp_tree_sym_diff(sp_tree *tree_1, sp_tree *tree_2) {
    sp_tree *tree;
    void    *data_1;
    void    *data_2;
    int      comp;

    // Sanity check:
    assert(tree_1 != NULL);
    assert(tree_2 != NULL);

    // Create a new tree:
    tree = new_sp_tree(tree_1->comp);
    if (tree == NULL) { return NULL; }

    // Special case: Both trees are the same
    if (tree_1 == tree_2) { return tree; }

    // Get the smallest element of each tree:
    data_1 = sp_tree_min(tree_1);
    data_2 = sp_tree_min(tree_2);

    // Until we have exhausted at least one of the trees:
    while (data_1 != NULL && data_2 != NULL) {

        // compare both nodes:
        comp = (tree->comp)(data_1, data_2);

        if (comp < 0) {

            // insert data_1 in tree:
            sp_tree_insert_max(tree, data_1);

            // advance data_1:
            data_1 = sp_tree_next(tree_1, data_1);

        } else if (comp > 0) {

            // insert data_2 in tree:
            sp_tree_insert_max(tree, data_2);

            // advance data_2:
            data_2 = sp_tree_next(tree_2, data_2);

        } else {

            // advance data_1:
            data_1 = sp_tree_next(tree_1, data_1);

            // advance data_2:
            data_2 = sp_tree_next(tree_2, data_2);

        }
    }

    // Insert all remaining data from tree_1:
    while (data_1 != NULL) {

        // insert data_1 in tree:
        sp_tree_insert_max(tree, data_1);

        // advance data_1:
        data_1 = sp_tree_next(tree_1, data_1);
    }

    // Insert all remaining data from tree_2:
    while (data_2 != NULL) {

        // insert data_2 in tree:
        sp_tree_insert_max(tree, data_2);

        // advance data_2:
        data_2 = sp_tree_next(tree_2, data_2);
    }

    // Return the resulting tree:
    return tree;
}



// DEBUG & VISUALIZATION:

// This is an auxiliary function to check the symmetric order property
// recursively. You should not use it directly, use "is_sp_tree" instead.
//
// Note that, unlike other functions of this library, it is safe to use
// recursive definitions here since they will only be used to test the
// code while debugging.
//
static int is_sp_subtree(const sp_tree *tree, const sp_node *node,
                         const void *min, const void *max) {

    // Make sure that node is (strictly) between specified limits:
    if (min != NULL && (tree->comp)(min, node->data) >= 0) {
        fprintf(stderr,"ERROR: Symmetric order not satisfied in sp_tree\n");
        return NO;
    }
    if (max != NULL && (tree->comp)(node->data, max) >= 0) {
        fprintf(stderr,"ERROR: Symmetric order not satisfied in sp_tree\n");
        return NO;
    }

    // Check recursively the left subtree of node:
    if (node->left != NULL) {
        if (is_sp_subtree(tree, node->left, min, node->data) == NO) {
            return NO;
        }
    }

    // Check recursively the right subtree of node:
    if (node->right != NULL) {
        if (is_sp_subtree(tree, node->right, node->data, max) == NO) {
            return NO;
        }
    }

    // All test are fine:
    return YES;
}

// This is an auxiliary function to check the symmetric order property of a
// splay tree. Returns YES if everything is correct and NO otherwise.
//
// This function should not be used in production code. I recommend to use:
//
//      assert(is_sp_tree(tree) == YES);
//
// to automatically remove all calls to this function when the flag NDEBUG
// is defined in the header files (deactivating all assertions).
//
int is_sp_tree(const sp_tree *tree) {

    // Basic Sanity Checks:
    if (tree == NULL) {
        fprintf(stderr, "ERROR: NULL pointer to sp_tree\n");
        return NO;
    }
    if (tree->comp == NULL) {
        fprintf(stderr, "ERROR: NULL comparing function in sp_tree\n");
        return NO;
    }

    // Trivial Case: empty tree
    if (tree->root == NULL) { return YES; }

    // General Case:
    return is_sp_subtree(tree, tree->root, NULL, NULL);
}

// This is an auxiliary function to print the tree recursively.
// You should not use it directly, use "print_sp_tree" instead.
//
// Note that, unlike the rest of the functions of this library, it is safe
// to use recursive definitions here since any tree large enough to hit the
// recursion limit will be too big to be printed anyway.
//
static void print_sp_subtree(const sp_node *node, const int is_right,
                             char *indent, void (* print_node) (const void *)) {

    // Allocate memory:
    char *new_indent = (char *) malloc((6+strlen(indent)*sizeof(char)));
    assert(new_indent != NULL);

    // Print right subtree recursively:
    if (node->right != NULL) {
        if (is_right == YES) { sprintf(new_indent, "%s%s", indent, "      "); }
        else                 { sprintf(new_indent, "%s%s", indent, "|     "); }
        print_sp_subtree(node->right, YES, new_indent, print_node);
    }

    // Print current node:
    if (is_right) { fprintf(stdout, "%s,----",indent); }
    else          { fprintf(stdout, "%s`----",indent); }
    if (print_node == NULL)  { fprintf(stdout, "(#)"); }
    else                     { print_node(node->data); }
    printf("\n");

    // Print left subtree recursively:
    if (node->left != NULL) {
        if (is_right == YES) { sprintf(new_indent, "%s%s", indent, "|     "); }
        else                 { sprintf(new_indent, "%s%s", indent, "      "); }
        print_sp_subtree(node->left, NO, new_indent, print_node);
    }

    // Free memory:
    free(new_indent);
}

// This function is used to print a splay tree on the screen.
//
//  * If a "print_node" function is provided it will be used to print the
//    content of each node.
//  * Otherwise (if print_node is NULL) all nodes will be printed as (#).
//
// Use this latter option if you only want to take a look at the tree shape.
//
// A correct print_node function only prints a single line of text using
// the "fprintf" command. It should NOT include any "\n" in that line.
//
// For example:
//
//  void MyPrintNodeFunction(const void *ptr) {
//      MyDataStruct *d = (MyDataStruct *) ptr;     // Pointer casting
//      fprintf(stdout, "(%d)", d->key);            // Printing integer key
//  }
//
// Finally, note that this is a visualization tool for debugging purposes only.
//
void print_sp_tree(const sp_tree *tree, void (* print_node) (const void *)) {

    // Avoid the trivial cases:
    if (tree != NULL && tree->root != NULL) {

        // Print right subtree recursively:
        if (tree->root->right != NULL) {
            print_sp_subtree(tree->root->right, YES, "     ", print_node);
        }

        // Print current node:
        fprintf(stdout, "----");
        if (print_node == NULL) { fprintf(stdout, "(#)"); }
        else { print_node(tree->root->data); }
        fprintf(stdout, "\n");

        // Print left subtree recursively:
        if (tree->root->left != NULL) {
            print_sp_subtree(tree->root->left, NO, "     ", print_node);
        }
    }

    // Empty the "stdout" buffer:
    fflush(stdout);
}

// END OF SPLAY TREES //////////////////////////////////////////////////////////
