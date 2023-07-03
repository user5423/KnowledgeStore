class Solution {
public:
    int best_transfer;
    int maximumRequests(int n, vector<vector<int>>& requests) {
        // there are n buildings
        // - each has a number of employees

        // Input: Requests - nested vector
        // - Requests[i] = [from, to] represents an employees transfer from one building to another
        
        // All buildings are full
        // - Therefore, a requests vector is valid iff the net change in employee transfers is 0 (for each building)
        
        // GOAL: Return the maximum number of achievable requests
        
        // APPROACH:
        // - this requires us to select the largest subset of requests that result in a net change
        // - there are 2^n subsets of requests = O(2^n) size of 'solution space'

        // Naive approach:
        // - we essentially create a virtual tree via a DFS search
        // - root node (no requests selected) ## our starting 'maximumRequest' val = 0
        //      1. we then accept a request at index = 0 and perform dfs on requests[1:]
        //      2. we also skip index=0, and perform the same dfs on requests[1:]

        // At each DFS call
        // - we check if the net transfer is 0
        //      - how do we check this? easiest way is to verify each building_transfer is 0
        //      - is there a better way? O(1) instead of O(n)
        //          - store two tracking variables - one for min, the other for max
        //          - on each call, (we verify whether accepting the request exceeds one of these bounds)
        //          - we'll figure this out later...
        // - if it is, we perform a maximization with the current working 'maxRequests value'

        // base case:
        // 1. there are no more requests to manage
        //      - perform working 'maxRequests value' maximization if relevant

        // How do we know the net transfer is 0 at each DFS call?
        // - on the original call, we know the net transfer is 0 (we haven't accepted a request)
        // - on each dfs call that is due to accepting a request
        //      - net_transfers[building_from] -= 1
        //      - net_transfers[building_to] += 1
        //      - perform the dfs call
        //      - then reverse the above increment/decrement operations (backtracking)

        this->best_transfer = 0;
        std::vector<int> net_transfers(n, 0);
        int depth = 0;
        int requests_accepted = 0;
        // this->solution1(requests, net_transfers, depth, requests_accepted);
        this->solution2(requests, net_transfers, depth, requests_accepted);
        return this->best_transfer;
    }

    void solution1(std::vector<vector<int>>&requests, std::vector<int> &net_transfers, int depth, int requests_accepted){
        auto nt_begin = net_transfers.begin();
        auto nt_end = net_transfers.end();
        if (*std::max_element(nt_begin, nt_end) == 0 && *std::min_element(nt_begin, nt_end) == 0)
            this->best_transfer = max(requests_accepted, this->best_transfer);

        // this for loop does not need to consider if we don't every select another element
        // - the above if statement already does that for us
        // if the loop selects the last element in the list (this for loop should never execute)
        for (int index = depth; index < requests.size(); index++){
            auto request = requests[index];
            int building_from = request[0];
            int building_to = request[1];

            net_transfers[building_from] -= 1;
            net_transfers[building_to] += 1;
            this->solution1(requests, net_transfers, index+1, requests_accepted+1);
            net_transfers[building_from] += 1;
            net_transfers[building_to] -= 1;
        }
    }

    void solution2(std::vector<vector<int>>&requests, std::vector<int> &net_transfers, int depth, int requests_accepted){
        auto nt_begin = net_transfers.begin();
        auto nt_end = net_transfers.end();
        // std::cout << "depth: " << depth << ", requests_accepted: " << requests_accepted << std::endl;
        if (depth == requests.size()){
            for (auto it = nt_begin; it != nt_end; it++)
                if (*it != 0)
                    return;
                this->best_transfer = max(requests_accepted, this->best_transfer);
            return;
        }

        // this for loop does not need to consider if we don't every select another element
        // - the above if statement already does that for us
        // if the loop selects the last element in the list (this for loop should never execute)
        for (int index = depth; index < requests.size(); index++){
            auto request = requests[index];
            int building_from = request[0];
            int building_to = request[1];

            net_transfers[building_from] -= 1;
            net_transfers[building_to] += 1;
            this->solution2(requests, net_transfers, index+1, requests_accepted+1);
            net_transfers[building_from] += 1;
            net_transfers[building_to] -= 1;
        }

        for (auto it = nt_begin; it != nt_end; it++)
            if (*it != 0)
                return;
            this->best_transfer = max(requests_accepted, this->best_transfer);
    }



    // Optimizations:
    // - we can try to get rid of the max_element and min_element operations (requests.length upper bound is quite low though)
    // - we can try to avoid unneccessary dfs calls (if we know nothing will come out of it)

    // Try a different algorithm or version?

};