This is an implementation of the Ford-Johson sorting algorithm.

## Parsing
Given as arguments are string inputs. Every argv needs to be converted into int and checked for validity (positive integers allowed by the subject)

# Sorting
- group the input elements into pairs, in case of an odd number of elements leave the last one unpaired
- sort within the pairs, creating a "main chain" of elements with a bigger value and a "pend chain" of elements of smaller value. if an odd element exists its part of the "pend chain"
- sort the main chain using merge sort

- insert the elements of the pend chain into the main chain using binary insertion sort. 
    - If we call main chain elements $a_n$ and pend chain element $b_n$, we know that $a_n$ will be an upper bound for insertion of $b_n$ into the main chain, as for every n $a_{n-1}$ < $a_n$ and $b_n$ < $a_n$
    - using Jacobsthal sequence to insert $b_n$ reduces the number of comparisons further (0, 1, 3, 2, 5, 4, 7, 6, 11, 10, 9, 8, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, ...)

## Sorting using Vectors
- compare input elements n and n + 1 to determine which element has the smaller value (pushed to _pendV vector) and which the bigger (pushed to _main vector).
    - _main vector elements are added as pairs, storing their original index to later determine their corresponding _pendV element. Thus keeping the original assigned pairs
    - if there is an odd amount of input values: push the remaining, not pairable, value to _pendV
- recursively sorting _main vector using merge sort
- reordering _pendV vector, so the original pairs are kept
- determine insertion order using Jacobsthal sequence
- create _result vector. $b_0$, which is _pendV[0] becomes the first element of _result, followed by all the elements of _main (representing $a_n$)
- create mainPos vector, which contains the current index within _result of the elements $a_n$ within _main
- insert _pendV element $b_n$ in insertion order into _result using binary insertion sort
    - upper bound for inserting $b_n$ is the current position on $a_n$ in result (lookup in mainPos vector)
    - updating the current positions of $a_n$ in mainPos



