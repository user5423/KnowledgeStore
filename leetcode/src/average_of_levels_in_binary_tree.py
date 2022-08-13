# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class Solution:
    def averageOfLevels(self, root: Optional[TreeNode]) -> List[float]:
        
        q = collections.deque([(root, 0)])
        levelSums = {}
        counters = {}
        
        while len(q):
            node, level = q.pop()
            
            if level not in levelSums:
                levelSums[level] = 0
                counters[level] = 0
                
            levelSums[level] += node.val
            counters[level] += 1
                
            if node.left:
                q.append((node.left, level + 1))
            if node.right:
                q.append((node.right, level + 1))
                
        return [value / counters[key] for key, value in levelSums.items()]
        