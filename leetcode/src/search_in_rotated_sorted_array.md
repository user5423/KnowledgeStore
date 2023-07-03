# Permutations (Medium) Search In Rotated Sorted Array

Problem URL: https://leetcode.com/problems/search-in-rotated-sorted-array/

#

## Solution 1

**Modified Binary Search**

We use modified binary search to search for the index in which the increasing sequence starts at 
- This has time complexity **O(log n)** and extra space complexity **O(1)**

We then use this pivot to create a sorted array that removes this rotation
- i.e. An array [4,5,1,2] is stored in a new array as [1,2,4,5]
- This has time complexity **O(n)** - (A little cheating technically) and extra space complexity of **O(n)**.
We selected this method of generating a new array to avoid messy indexing that is less intuitive
- We then perform a binary search on the new array and return the index of the target (which is then adjusted for its rotation offset)


Runtime: **0 ms**, faster than **100%** of C++ online submissions for Search In Rotated Sorted Array.

Memory Usage: **11.3 MB**, less than **14.86%** of C++ online submissions for Search In Rotated Sorted Array.

[C++ Code Implementation](search_in_rotated_sorted_array.cpp)

