#include <cmath>
#include <algorithm>
#include <climits>
class Solution {
public:
    int unfairness;
    int half_cookies;
    int distributeCookies(vector<int>& cookies, int k) {
        // Unfairness == max total cookies obtained by a single child in the distribution
        // Bags of cookies cannot be split up and distributed to multiple children
        // All bags must be distributed

        // Our goal is to find the minimum 'unfairness' value

        // The potential distributions can be considered as a tree
        // - where each internal node is a partial distribution
        // - a edge is an allocation of unallocated bag to some child
        // - and the leaf node is a complete allocation

        // This tree would has size O(child_count ^ bag_count)
        // Naive solution: A dfs search is plausable for this (which can be sped up with backtracking)
        this->unfairness = INT_MAX;
        // this->half_cookies = (float) this->unfairness / 2.0;
        std::vector<int> child_unfairness(k, 0);
        // this->solution1(cookies, k, 0, child_unfairness);
        // this->solution2(cookies, k, 0, child_unfairness, 0);
        this->solution3(cookies, k, 0, child_unfairness, 0);
        return this->unfairness;
    }


    // Optimizations
    // - If a single child has more than half of the cookies, then we can exit early
    // - If during the DFS search, any child has greater than current minimum unfairness value, we can exit early
    // - If the minimum unfairness value is equal to the smallest bag in the cookies, we can stop ** (trivial case)
    void solution1(vector<int>& cookies, int k, int depth, std::vector<int>&child_unfairness){
        // Base Case: We have processed all bags
        if (depth == cookies.size()){
            this->unfairness = min(*std::max_element(child_unfairness.begin(), child_unfairness.end()), this->unfairness);
            return;
        }
        
        // Base Case: We have unprocessed bags
        int cookie_count = cookies[depth];
        for (int child = 0; child < k; child++){
            child_unfairness[child] += cookie_count;
            this->solution1(cookies, k, depth+1, child_unfairness);
            child_unfairness[child] -= cookie_count;
        }

        return;
    }

    void solution2(vector<int>& cookies, int k, int depth, std::vector<int>&child_unfairness, int most_unfair_child){
        // Optimized cases
        // Base Case: We have processed all bags
        if (depth == cookies.size()){
            // we can get removed  of the max element
            // We can optimize to constant time on each call (but we'd still be doing same operations (or more) over multiple calls)
            this->unfairness = min(child_unfairness[most_unfair_child], this->unfairness);
            return;
        }
        
        // Base Case: We have unprocessed bags
        int cookie_count = cookies[depth];
        for (int child = 0; child < k; child++){
            int& child_val = child_unfairness[child];
            if (child_val >= this->unfairness)
                continue;

            child_val += cookie_count;

            // We share minimimization operations between leaf nodes
            int new_most_unfair_child = most_unfair_child;
            if (child_val > child_unfairness[new_most_unfair_child])
                new_most_unfair_child = child;

            // We avoid searching subtrees via backtracking
            if (child_val < this->unfairness)
                this->solution2(cookies, k, depth+1, child_unfairness, new_most_unfair_child);
            child_val -= cookie_count;
        }

        return;
    }


    void solution3(vector<int>& cookies, int k, int depth, std::vector<int>&child_unfairness, int most_unfair_child){
        // Optimized cases
        // Base Case: We have processed all bags
        if (depth == cookies.size()){
            // we can get removed  of the max element
            // We can optimize to constant time on each call (but we'd still be doing same operations (or more) over multiple calls)
            this->unfairness = min(child_unfairness[most_unfair_child], this->unfairness);
            return;
        }
        
        std::set<int> unfairness_values;

        // Base Case: We have unprocessed bags
        int cookie_count = cookies[depth];
        for (int child = 0; child < k; child++){
            int& child_val = child_unfairness[child];
            
            child_val += cookie_count;

            // We share minimimization operations between leaf nodes
            int new_most_unfair_child = most_unfair_child;
            if (child_val > child_unfairness[new_most_unfair_child])
                new_most_unfair_child = child;

            // We avoid searching subtrees via backtracking
            if (child_val < this->unfairness)
                this->solution2(cookies, k, depth+1, child_unfairness, new_most_unfair_child);
            child_val -= cookie_count;

            // Here's another optimization (found by somewhere else)
            // - children are the identical in this question
            // - if we have multiple children with 0's then allocating bag a to any of them would result
            // in a path to identical unfairness value in the search tree.
            if (child_val == 0)
                break;
        }

        return;
    }

};