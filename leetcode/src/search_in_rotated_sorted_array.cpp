#define get_middle(X,Y) (int) ((X + Y) / 2.0)
class Solution {
public:
    int search(vector<int>& nums, int target) {
        // find the pivot in log n time

        // store that pivot as offset
        // perform binary search while using offset
        // std::cout << "New question\n\n\n" << std::endl;
        int rotated_pivot = this->find_pivot(nums);
        int out = this->pivoted_binary_search_1(nums, rotated_pivot, target);
        // std::cout << "Answer: " << out << std::endl;
        return out;
    }
    
    int pivoted_binary_search_1(vector<int>& nums, int rotation_offset, int target){
        // first we want to create a new array
        vector<int> nnums;
        nnums.insert(nnums.end(), nums.begin() + rotation_offset, nums.end());
        nnums.insert(nnums.end(), nums.begin(), nums.begin() + rotation_offset);
        int nnums_size = nnums.size();

        int left, right, middle;
        left = 0;
        right = nnums_size -1;

        while (left < right){
            middle = get_middle(left, right);
            int middle_value = nnums[middle];
            if (target < middle_value)
                right = middle -1;
            else if (target > middle_value)
                left = middle + 1;
            else
                return (middle + rotation_offset) % nnums_size;
        }
        
        if (nnums[left] == target)
            return (left + rotation_offset) % nnums_size;
        return -1;
    }

    int find_pivot(vector<int>& nums){
        int left, right, middle;
        left = 0;
        right = nums.size() - 1;

        while (right - left > 1){
            middle = get_middle(left, right);
            int middle_value = nums[middle];

            if (nums[left] > middle_value)
                right = middle;
            else if (middle_value > nums[right])
                left = middle;
            else
                return left;

        }

        if (right - left == 1)
            return (nums[left] < nums[right]) ? left: right;

        return 0;

    }
};

