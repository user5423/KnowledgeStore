class Solution:
    def rob(self, nums: List[int]) -> int:
        return self.rob1(nums)
        
        
    def rob1(self, nums):
        if len(nums) == 1:
            return nums[0]
        return max(self._rob(nums[:-1]), self._rob(nums[1:]))
        
    def _rob(self, nums: List[int]):
        if len(nums) == 1:
            return nums[0]
        elif len(nums) == 2:
            return max(nums)
        
        bestFirst = nums[0]
        bestSecond = nums[1]
        bestCurrent = max(bestFirst, bestSecond)
        
        for i in range(2, len(nums)):
            bestCurrent = max(bestFirst + nums[i], bestSecond)
            bestFirst = max(bestSecond, bestFirst)
            bestSecond = bestCurrent
            
        return bestCurrent
    
    def rob2(self, nums):
        ...