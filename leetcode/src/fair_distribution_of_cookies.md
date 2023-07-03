# Fair Distribution of Cookies (Medium)

Problem URL: https://leetcode.com/problems/fair-distribution-of-cookies/

#


## Solution 1 (C++)

Runtime: **1758 ms**, faster than **37.53%** of C++ online submissions for Fair Distribution of Cookies.

Memory Usage: **13.9 MB**, less than **50.51%** of C++ online submissions for Fair Distribution of Cookies.

### Implementation

The solution performs a backtracking DFS search. This search corresponds to the virtual tree where an edge corresponds allocating a single unallocated
bag of cookies to a child from the parent node partial allocation. Leaf nodes of this virtual tree are complete allocations (i.e. all bags have been allocated).

Once our DFS search has reached a leaf node, we determine the unfairness value of the current allocation. This is a minimization operation over a vector of size children
which holds a unfairness count for each child. 

The time complexity is **O((child_count ^ bag_count)/2) * O(child_count) + O((child_count ^ bag_count)/2) * O(child_count)**
- The LHS of the time complexity sum is for non-leaf processing
- The RHS of the time complexity sum is for base-case processing (i.e. leaves)

Therefore, the total time complexity is **O(child_count * child_count ^ bag_count)

The space complexity of our solution is **O(child_count + bag_count) + O(1*bag_count)**
- Two vectors of the above sizes respectively are passed by reference around the DFS search process
- All other variables/ datastructures in a single DFS call are constant size, and the max size of the DFS stack is **bag_count**

Therefore, the total space complexity is **O(child_count + bag_count)**

## Solution 2 (C++)

Runtime: **233 ms**, faster than **71.94%** of C++ online submissions for Fair Distribution of Cookies.

Memory Usage: **6.8 MB**, less than **50.51%** of C++ online submissions for Fair Distribution of Cookies.

### Implementation

We speedup the time complexity of base-cases significantly here. We use a working variable that we pass through DFS calls that holds the current unfairness value.
This working variable performing minimization computations to determine if the new bag allocation increases the overall fairness value (and what it is).

This distributes the **O(child_count)** time complexity minimization operation performed over each DFS call. However, at a given internal node, all paths originating
from this internal node can re-usea single minimization operation to calculate the unfairness value. This re-use of already computed values provides a nice speedup

The time complexity is **O((child_count ^ bag_count)/2) * O(child_count + 1) + O((child_count ^ bag_count)/2) * O(1)**
- The minimization operation costs constant time for each DFS call performed on internal nodes - (This makes no change to the time complexity for internal node processing)
- The base-case time complexity reduces significantly such that DFS calls on leaf nodes are no longer linear but constant time.

We also add a condition before each potential DFS call to check if allocating a specific bag to a child increases the unfairness value above the current lowest unfairness value
for a complete allocation


**[C++ Code Implementation](fair_distribution_of_cookies.cpp)**

## Solution 3 (C++)

Runtime: **30 ms**, faster than **84.19%** of C++ online submissions for Fair Distribution of Cookies.

Memory Usage: **6.8MB**, less than **50.19%** of C++ online submissions for Fair Distribution of Cookies.

### Implementation

Searching through the Leetcode answers, we find something we have missed. Children are identical in this question. Therefore, testing the allocation of bag 'A' to children who'se 
unfairness value is 0 results in redundant searches to the same unfairness values. Their solutions add a condition to reduce executing these redundant searches
