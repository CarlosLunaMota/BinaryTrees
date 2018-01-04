# CLM_BinaryTrees

## TL;DR:

This library implements three different binary search trees in C:

* The _Classic_ **Binary Search Tree** (```bs_tree``` functions)
* The **Red Black Tree** (```rb_tree``` functions)
* The **Splay Tree** (```sp_tree``` functions)

All operations are implemented using top-down, single-pass, iterative
algorithms to avoid using parent pointers, recursion or any kind of
explicit or implicit limit on the size of the trees (other than the available
RAM).

The library uses _void pointers_ rather than macros (at the expense of
a little bit of time and memory) to be easier to understand, modify
and maintain.

----

## Content

* [Introduction](#introduction)
* [Binary Search Trees](#binary-search-trees)
* [This Library](#this-library)
* [References](#references)

----

## Introduction

A Binary Search Tree [^1] is a versatile linked data structure designed to
store and retrieve data quickly.

They can be used to implement, among others, the Dictionary [^2], the Set [^3]
and the Priority Queue [^4] abstract data types.

The basic primitives of a Binary Search Tree are:

* ```Insert(elem)``` - Store a new element in the tree
* ```Remove(elem)``` - Remove an element from the tree
* ```Search(elem)``` - Find an element in the tree

These primitives can also be implemented using a Hash Table and, indeed, it
will be more efficient to use a Hash Table in many cases, but at the cost of
providing a good _hash function_ every time you use the data structure (which
is hard) instead of providing just a _comparing function_ (easier in general).

In addition, with a Binary Search Trees you can implement other primitives:

* ```Find_Min()     ``` - Find the smallest element stored in the tree
* ```Find_Max()     ``` - Find the biggest element stored in the tree
* ```Remove_Min()   ``` - Remove the smallest element stored in the tree
* ```Remove_Max()   ``` - Remove the biggest element stored in the tree
* ```Find_Next(elem)``` - Find the smallest element that is bigger than elem
* ```Find_Prev(elem)``` - Find the biggest element that is smaller than elem

That cannot be implemented efficiently using a Hash Table and that add a
lot of functionality to this data structure.

----

## Binary Search Trees

There are many variants of the Binary Search Tree data structure.
All of them share the following common assumptions:

* The data is stored in _nodes_ and each node is linked to 2 other nodes
(traditionally called _left_ and _right_) forming a _rooted_ binary tree.
* The elements stored in the tree form a totally ordered set. That means
that given any two of them (A and B) then either A < B, A = B or A > B.
* The _left subtree_ of a node contains elements that are smaller than
the element stored in the node. Likewise, the _right subtree_ contains
elements that are bigger than the element stored in the node.
* Althought it is not strictly required, it is very common to assume that
that all elements are different, which means that inserting an element
twice will overwrite the information asociated to that element.

As long as we are free to define the contents of the elements stored in the
tree and to provide a custom _comparing function_, the last 2 points (the
*symmetric order property* and the *uniqueness property* respectively) do
not supose any practical limitation:

* We can always define an arbitrary (but consistent) total ordering for
the data stored in a computer. In general it is not even difficult.
* We can include a _counter_ field in the data stored in the tree and
increment or decrement it acordingly to implement a multiset.

The efficiency of all operations made in a Binary Search Tree usually depend
on the distance between the element that we are manipulating and the _root
node_ of the tree. On well behaved Binary Search Trees, this distance is
proportional to the (base 2) logarithm of the number of stored elements.

It turns out, however, that the assumptions stated above do not impose a
rigid configuration, so two different Binary Search Trees can contain the
same elements and greatly differ in shape:

      2                   4
     / \                /   \
    1   3              2     6
         \            / \   / \
          4          1   3 5   7
           \
            5
             \
              6
               \
                7

Although both trees contain the same elements and satisfy all the assumptions 
stated above, the left tree (rooted at 2) is quite _degenerated_ while the one
on the right (rooted at 4) is perfectly _balanced_.

If we define the _height_ of a tree as the maximum distance between any of its
nodes and the _root node_ then, a Binary Search Tree is _balanced_ if its
_height_ is proportional to the logarithm of the number of elements stored in
it and all operations will be fast.

In contrast, when a tree is _degenerated_ the _height_ of the tree is
proportional to the number of stored elements and all opeartions will be
painfully slow.

> For a tree with 1 million of elements, a linear performance will require
> about 1 million of steps whereas a logarithmic performance can accomplish
> the same with just 20 steps.

Although a Binary Search Tree will turn out naturally balanced if we
introduce the elements uniformly at random, the fact is that for most
applications of Binary Search Trees we cannot guarantee the required degree
of randomness and it is worth to perform some extra operations to ensure that
the tree remains fairly balanced.

The most common self-balancing Binary Search Tree variants are:

* **AVL trees**[^5] are the oldest balanced search trees and the most rigidly
balanced ones. However, there is not any top-down single-pass deletion algorithm
for them which makes them less popular nowadays.
* **Treaps**[^6] are efficient and relatively easy to implement but require the
generation and storage of pseudo-random numbers in each node.
* **B-trees**[^7] is a huge family of variants that include the 2-3 trees[^8]
and the 2-3-4[^9] trees as a particular cases. Their main drawback is that they
either use different sized nodes or waste memory using always the biggest node
type.
* **Red Black trees**[^10] simulate the behavior of 2-3-4 trees by
using binary nodes with and additional field called _the color_. They are less
rigidly balanced than AVL trees (which makes possible to use top-down
single-pass insertion and deletion algorithms) but tend to perform like
them, making Red Black trees the most popular self-balancing variant
nowadays.
* Finally, **Weight Balanced trees**[^11] are less known but more versatile than
Red Black trees, however, they require to store _the size_ of each subtree in
every node, which imposes an artificial upper limit in the size of the tree.
With the popularization of the 64 bit computers this is no longer a problem and
because of the _memory alignment_ of modern compilers they use the same memory
as the theoretically more efficient Red Black trees.

All these variants require to store extra information in the nodes of the
tree and to perform some _rebalancing_ operations every time an element is
inserted or removed.

There are other solutions to avoid the problems associated to degeneracy without
storing additional information in the nodes of the tree. Two of the most
interesting ones are:

* **Scapegoat trees**[^12] are Binary Search Trees that perform an expensive
rebalancing operation when they detect degeneracy in a subtree.
The amortized cost of such operation is constant and it will only be
applied after a insertion or a deletion. Their only drawback is that
rebalance operation needs to know the size of the rebalanced subtree and
I would rather preffer algorithms that do not explicitly keep track of
the size or the height of the tree.
* **Splay trees**[^13] are Binary Search Trees that perform a rebalancing 
operation called _Splaying_ each time an element is accesed. That means
that **all operations** (insert, remove, search, find_min, etc...) will be
more expensive than they are in the other Binary Search Tree variants.
In exchange, however, the _Splaying_ operation will move the accessed
element to the root of the tree, which means that accesing again to this
element will be cheaper. Furthermore, any node that has been accesed
recently or that is _close_ in value to the last accesed element will be
cheaper to find and manipulate. The self-adaptability property of Splay
trees works well with many _not so random_ access sequences that we often
find in real world projects and the compressing nature of the Splaying
operation will ensure that Splaying tree operation run in amortized 
logarithmic time which makes them interesting for many aplications.
Finally, Splay tree functions are simpler, cleaner and easier to code.

### Sumarizing:

* If you are sure that your insertion and deletion sequences will be random
you should use the simple **Binary Search Tree** functions because they are
fast and require less memory.
* If the operations that you are performing have any kind of _temporal
locality_ (elements accesed recently will be accesed again soon) or
_spatial locality_ (elements similar to elements accesed recently will tend
to be accesed) you should use a **Splay tree**.
* In any other cases you must sacrifice a little bit of extra memory to
gain the security of using a bomb-proof **Red Black tree**.

----

## This Library

Since the C programming language does not have a standar library of abstract
data types, it is very convenient to use this kind of swiss-army-knife data
structures that are able to implement a wide range of the most commonly used
containers.

It is difficult, however, to find good generic implementations of this data
structure that do not rely on _parent pointers_ (which waste memory) or
_recursion_ (which wastes time and imposes an artificial limit on the amount
of data stored in the tree). As a result I decided to write my own library.

It started as a personal project and, as such, some of the design decisions
made here will not fit your needs. Let me list the objectives that I pursued
while coding this library so you can judge better:

* To write a standalone generic library in C that implements one or more
variants of the Binary Search Tree data structure.
* To avoid any explicit or implicit limit on the size of the tree by using
iterative algorithms that do not require this kind of data as input.
* To avoid the use of _parent pointers_ by using single-pass top-down
algorithms.
* To avoid using hacks or hardware/software dependend tricks that may stop
working, make the library difficult to use or slow down the code in a future.
* To keep the code as clear, simple and well documented as possible so I can
use it for many years as the underlying data structure of my future projects.
* To be memory and time efficient as long as it does not conflict with the
previously stated objectives.
* To provide testing and visualizing tools in the library to help the final
users to understand what is happening inside this data structure and to debug
any modification made in the code.

So, in particular:

* I would rather use _void pointers_ than macros to attain genericness while
keeping the library simple.
* The final user of the library will not be required to deal with _fingers_,
_iterators_, _traversers_ or _callback functions_ (other than the _comparing
function_) because increasing the traversing time from O(n) to O(n log n) is
usually not a big deal and the resulting code is simpler and more robust that
way.
* Binary Search Trees and Splay trees will have a memory overhead of 3 pointers
(_left_, _right_ and _data_) per stored item while Red Black trees will have a
memory overhead of 3 pointers and a char (_left_, _right_, _data_ and _color_).
This is good enough for most projects and quite competitive if you take into
account the amount of functionality provided.
* The elements stored in the tree need to be created and destroyed outside
the tree. This allows the user to store the same element in multiple data
structures without wasting memory. This also avoids the mandatory use of
_copy_ and _destroy_ functions.
* I can not provide an efficient _select_ function without storing the number
of elements of each subtree. So you either modify the library to store that
information or use the ```find_min``` and ```find_next``` functions to retrieve
the k-th element in O(k log n) time.
* I will provide the common Set Functions (Union, Intersection, Difference and
Symmetric Difference) for all tree variants but their efficiency will vary.
* I will code all three variants in this library using the same syntax so you
only need to change the prefix of the functions to try another variant:
  * Classic Binary Search Tree functions use the ```bs_tree``` prefix.
  * Red Black tree functions use the ```rb_tree``` prefix.
  * Splay tree functions use the ```sp_tree``` prefix.

Since you can mix and match Classic Binary Tree functions and Splay tree
funtions I used the same ```bs_tree``` and ```bs_node``` structs in both
cases (although I _typedef_ them as ```sp_node``` and ```sp_tree``` to maintain
the change-the-prefix-to-change-the-implementation property stated above).

In contrast, althought most of the Red Black tree functions are, indeed, equal
to their Classic Binary Tree counterparts, you cannot mix them because Red
Black functions require that the nodes contain an additional _color_ field
and because they expect the tree to be already balanced.

----

## References:

[^1]  https:en.wikipedia.org/wiki/Binary_search_tree
[^2]  https:en.wikipedia.org/wiki/Associative_array
[^3]  https:en.wikipedia.org/wiki/Set_(abstract_data_type)
[^4]  https:en.wikipedia.org/wiki/Priority_queue
[^5]  https:en.wikipedia.org/wiki/AVL_tree
[^6]  https:en.wikipedia.org/wiki/Treap
[^7]  https:en.wikipedia.org/wiki/B-tree
[^8]  https:en.wikipedia.org/wiki/2–3_tree
[^9]  https:en.wikipedia.org/wiki/2–3–4_tree
[^10] https:en.wikipedia.org/wiki/Red–black_tree
[^11] https://en.wikipedia.org/wiki/Weight-balanced_tree 
[^12] https:en.wikipedia.org/wiki/Scapegoat_tree
[^13] https:en.wikipedia.org/wiki/Splay_tree
