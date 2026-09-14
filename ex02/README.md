This is an implementation of the Ford-Johson sorting algorithm.

It is required to use 2 different containers to solve the problem. Here lists and vectors are chosen.

Vectors are more practical implementing binary insertion, as they are indexed. For the same binary insertion it is necesarry to run through the list elements one by one, which slows the insertion down a lot.
On the other hand using insert() for std::vector means to move all the vector elements in memory, as vectors are required to be saved in a continous memory space (which then provides the option to be indexed). insert() for std::list is quicker as the memory does not have to be continous and upon insertion just pointer to the next element have to be relinked.

Looking at this using std::list is likely to be more efficient if the elements are big in size as this will be a disadvantage for std::vector upon moving all the elements, while searching through the list does not depend on the size og the elements, at it is just searched for element id there. 