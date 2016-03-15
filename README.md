# WalkerMethod

Walker's Alias Method is an O(1) algorithm for selecting elements from an array given a weighted distribution.

For example, let's say you want to return `:win` 60% of the time, `:lose` 20% of the time, and `:tie` 20% of the time.  You could fill an array with
6 examples of `:win` and 2 examples each of `:lose` and `:tie`, then pick a random element.  You could also use ranges, picking a random number
between 0.0 and 1.0 and returning `:win` when the number is below 0.6, `:lose` if the number is below 0.8, and `:tie` otherwise.  But, these algorithms are still O(n).
You can do better by using a heap or binary search tree, but Walker's Alias Method is better still, with a constant runtime once an O(n)
pre-computation phase has completed.

#### how to use
- if you need main function, please comment out there.
- ```cmake .```
- ```make```
