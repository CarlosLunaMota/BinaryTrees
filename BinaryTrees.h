////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//      BinaryTrees.h                                                         //
//      -------------                                                         //
//                                                                            //
// Content: The headers of the BinaryTrees library.                           //
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


// BINARY_TREES LIBRARY ////////////////////////////////////////////////////////

#ifndef CLM_BINARY_TREES

    #define CLM_BINARY_TREES

    // GENERAL MACROS //////////////////////////////////////////////////////////

    #ifndef YES
        #define YES 1
    #endif

    #ifndef NO
        #define NO 0
    #endif
        
    ////////////////////////////////////////////////////////////////////////////


    // BINARY SEARCH TREES /////////////////////////////////////////////////////

    // STRUCTS:

    typedef struct bs_node {
        void           *data;   // Generic pointer to the content (never NULL)
        struct bs_node *left;   // Left subtree  (NULL if empty)
        struct bs_node *right;  // Right subtree (NULL if empty)
    } bs_node;

    typedef struct bs_tree {
        struct bs_node *root;                       // Root node of the tree
        int (* comp) (const void *, const void *);  // Comparing function
    } bs_tree;

    // CREATION & INSERTION:

    bs_tree *new_bs_tree(int (* comp) (const void *, const void *));

    bs_tree *bs_tree_copy(const bs_tree *tree);

    void *bs_tree_insert(bs_tree *tree, void *data);

    void *bs_tree_insert_min(bs_tree *tree, void *data);

    void *bs_tree_insert_max(bs_tree *tree, void *data);

    // SEARCH:

    int   bs_tree_is_empty(const bs_tree *tree);

    void *bs_tree_search(const bs_tree *tree, const void *data);

    void *bs_tree_min(const bs_tree *tree);

    void *bs_tree_max(const bs_tree *tree);

    void *bs_tree_prev(const bs_tree *tree, const void *data);

    void *bs_tree_next(const bs_tree *tree, const void *data);

    // REMOVE:

    void *bs_tree_remove(bs_tree *tree, const void *data);

    void *bs_tree_remove_min(bs_tree *tree);

    void *bs_tree_remove_max(bs_tree *tree);

    void bs_tree_remove_all(bs_tree *tree, void (* free_data) (void *));

    // SET FUNCTIONS:

    bs_tree *bs_tree_union(const bs_tree *tree_1, const bs_tree *tree_2);

    bs_tree *bs_tree_intersection(const bs_tree *tree_1, const bs_tree *tree_2);

    bs_tree *bs_tree_diff(const bs_tree *tree_1, const bs_tree *tree_2);

    bs_tree *bs_tree_sym_diff(const bs_tree *tree_1, const bs_tree *tree_2);

    // REBALANCE OPERATIONS:

    void bs_tree_to_list(bs_tree *tree);

    void bs_tree_to_reversed_list(bs_tree *tree);

    void bs_tree_rebalance(bs_tree *tree);

    // DEBUG & VISUALIZATION:

    int  is_bs_tree(const bs_tree *tree);

    void print_bs_tree(const bs_tree *tree, void (* print_node) (const void *));

    ////////////////////////////////////////////////////////////////////////////



    // RED BLACK TREES /////////////////////////////////////////////////////////

    // RED BLACK MACROS:

    #define RED 1
    #define BLACK 0
    #define IS_RED(p)   (((p) != NULL) && ((p)->color == RED))
    #define IS_BLACK(p) (((p) == NULL) || ((p)->color == BLACK))

    // STRUCTS:

    typedef struct rb_node {
        void           *data;   // Generic pointer to the content (never NULL)
        struct rb_node *left;   // Left subtree  (NULL if empty)
        struct rb_node *right;  // Right subtree (NULL if empty)
        char            color;  // Either RED (= 1) or BLACK (= 0)
    } rb_node;

    typedef struct rb_tree {
        struct rb_node *root;                       // Root node of the tree
        int (* comp) (const void *, const void *);  // Comparing function
    } rb_tree;

    // CREATION & INSERTION:

    rb_tree *new_rb_tree(int (* comp) (const void *, const void *));

    rb_tree *rb_tree_copy(const rb_tree *tree);

    void *rb_tree_insert(rb_tree *tree, void *data);

    void *rb_tree_insert_min(rb_tree *tree, void *data);
    
    void *rb_tree_insert_max(rb_tree *tree, void *data);

    // SEARCH:

    int   rb_tree_is_empty(const rb_tree *tree);

    void *rb_tree_search(const rb_tree *tree, const void *data);

    void *rb_tree_min(const rb_tree *tree);

    void *rb_tree_max(const rb_tree *tree);

    void *rb_tree_prev(const rb_tree *tree, const void *data);

    void *rb_tree_next(const rb_tree *tree, const void *data);

    // REMOVE:

    void *rb_tree_remove(rb_tree *tree, const void *data);

    void *rb_tree_remove_min(rb_tree *tree);

    void *rb_tree_remove_max(rb_tree *tree);

    void  rb_tree_remove_all(rb_tree *tree, void (* free_data) (void *));

    // SET FUNCTIONS:

    rb_tree *rb_tree_union(const rb_tree *tree_1, const rb_tree *tree_2);

    rb_tree *rb_tree_intersection(const rb_tree *tree_1, const rb_tree *tree_2);

    rb_tree *rb_tree_diff(const rb_tree *tree_1, const rb_tree *tree_2);

    rb_tree *rb_tree_sym_diff(const rb_tree *tree_1, const rb_tree *tree_2);

    // DEBUG & VISUALIZATION:

    int  is_rb_tree(const rb_tree *tree);

    void print_rb_tree(const rb_tree *tree, void (* print_node) (const void *));

    ////////////////////////////////////////////////////////////////////////////


    
    // SPLAY TREES /////////////////////////////////////////////////////////////

    // STRUCTS:

    typedef bs_tree sp_tree;    // Splay Trees are just Binary Search Trees
    typedef bs_node sp_node;    // Splay Nodes are just Binary Search Nodes

    // CREATION & INSERTION:

    sp_tree *new_sp_tree(int (* comp) (const void *, const void *));

    sp_tree *sp_tree_copy(sp_tree *tree);

    void *sp_tree_insert(sp_tree *tree, void *data);

    void *sp_tree_insert_min(sp_tree *tree, void *data);

    void *sp_tree_insert_max(sp_tree *tree, void *data);

    // SEARCH:

    int   sp_tree_is_empty(const sp_tree *tree);

    void *sp_tree_search(sp_tree *tree, const void *data);

    void *sp_tree_min(sp_tree *tree);

    void *sp_tree_max(sp_tree *tree);

    void *sp_tree_prev(sp_tree *tree, const void *data);

    void *sp_tree_next(sp_tree *tree, const void *data);

    // REMOVE:

    void *sp_tree_remove(sp_tree *tree, const void *data);

    void *sp_tree_remove_min(sp_tree *tree);

    void *sp_tree_remove_max(sp_tree *tree);
    
    void sp_tree_remove_all(sp_tree *tree, void (* free_data) (void *));

    // SET FUNCTIONS:

    sp_tree *sp_tree_union(sp_tree *tree_1, sp_tree *tree_2);

    sp_tree *sp_tree_intersection(sp_tree *tree_1, sp_tree *tree_2);

    sp_tree *sp_tree_diff(sp_tree *tree_1, sp_tree *tree_2);

    sp_tree *sp_tree_sym_diff(sp_tree *tree_1, sp_tree *tree_2);

    // DEBUG & VISUALIZATION:

    int  is_sp_tree(const sp_tree *tree);

    void print_sp_tree(const sp_tree *tree, void (* print_node) (const void *));

    ////////////////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////////////////////
