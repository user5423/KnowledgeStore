class Solution:
    def permute(self, nums: List[int]) -> List[List[int]]:
        return self.permute1(nums)
        return self.permute2(nums)


    def permute1(self, nums):
        self.permutations = []
        self._permute1(set(nums))
        return self.permutations
        
    def _permute1(self, nums: List[int], path = []) -> None:
        ## base case
        if len(nums) == 1:
            num = nums.pop()
            self.permutations.append(path + [num])
            nums.add(num)
        
        ## otherwise
        for num in copy.deepcopy(nums):
            nums.remove(num)
            path.append(num)
            self._permute1(nums, path)
            path.pop()
            nums.add(num)


    def permute2(self, nums: List[int]) -> List[List[int]]:
        return list(self.permute2(nums))

    def _permute2(self, nums: List[int]) -> List[List[int]]:
        if len(nums) <= 1:
            yield nums
        else:
            for i in self.permute2(nums[1:]):
                for j in range(len(nums)):
                    yield i[:j] + nums[0:1] + i[j:]
                