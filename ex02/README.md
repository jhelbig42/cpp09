- given is a string input
- read the string in sections, converting to int
- check validity (INT_MIN, INT_MAX, no floats, no char ...)

- sort elements n+1 and n into vectors: main pair<id, main_val> and pend<val>
- sort main by main_val using merge sort

- create result vector
- insert pend[0] and main
- insert pends into result in Jacobsthal sequence, using indeces of main vector

