class Solution:
    def longestConsecutive(self, nums: List[int]) -> int:
        if len(nums) == 0:
            return 0
        nums = set(nums)
        bestCounter = 1
        for val in nums:
            if (val - 1) not in nums:
                counter = 1
                while (val + 1) in nums:
                    counter += 1
                    val += 1
                
                bestCounter = max(bestCounter, counter)
        
        return bestCounter