class Solution:
    def rob1(self, nums: List[int]) -> int:
        """ O(n) runtime complexity and O(n) extra space"""
        if len(nums) == 1:
            return nums[0]
        elif len(nums) == 2:
            return max(nums)

        dp = [0]*len(nums)
        dp[0] = nums[0]
        dp[1] = max(nums[0], nums[1])
        for i in range(2, len(nums)):
            dp[i] = max(dp[i-1], dp[i-2] + nums[i])
            
        return dp[-1]

    def rob2(self, nums: List[int]) -> int:
        """ O(n) runtime complexity and O(1) extra space"""
        if len(nums) == 1:
            return nums[0]
        elif len(nums) == 2:
            return max(nums)
        
        bestFirst = nums[0]
        bestSecond = nums[1]
        bestCurrent = max(bestFirst, bestSecond)
        for i in range(2,len(nums)):
            bestCurrent = max(bestFirst + nums[i], bestSecond)
            bestFirst = max(bestFirst, bestSecond)
            bestSecond = bestCurrent
            
        return bestCurrent
        