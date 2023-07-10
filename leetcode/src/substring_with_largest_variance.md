# Substring With Largest Variance (Hard)

Problem URL: https://leetcode.com/problems/substring-with-largest-variance/

#

## Solution 1

Iterated through all substrings O(n^2), and calculated (and tracked) the variance of each substring. The variance calculated re-used computations from previous substring variance caluclations. This means that the space complexity for calculating the variance of all substrings that start from an arbitrary index is O(n).

Despite this optimization over the naive O(n^3) version, this solution results in a Time Limit Exceeded (TLE) error from Leetcode on the last 10 test cases.


## Solution 2

This solution required some external help. It uses Kadane's algorithm to iterate over all distinct pairs of alphabet characters, and calculate the best variance when considering pairs of characters in the string. Calculating the maximum variance of a string with respect to a distinct pair can be performed using Kadane's algorithm in O(n) time complexity and O(1) extra space complexity.

Therefore, the total running time is O(n) * O(26^2) = O(n) and the space complexity is O(n).

Runtime: **196 ms**, faster than **71.17%** of C++ online submissions for Spiral Matrix.

Memory Usage: **6.8 MB**, less than **95.71%** of C++ online submissions for Spiral Matrix.

[C++ Code Implementation](substring_with_largest_variance.py)