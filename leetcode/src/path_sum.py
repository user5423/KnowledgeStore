# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class Solution:
    def hasPathSum(self, root: Optional[TreeNode], targetSum: int) -> bool:
        return self.dfs(root, targetSum)
    
    
    def dfs(self, root, targetSum):
        if root is None:
            return False
        
        return self._dfs(root, targetSum)
        
    def _dfs(self, root, targetSum):
        ## basec case - root is a leaf
        if root.left is None and root.right is None:
            if targetSum - root.val == 0:
                return True
            return False
            
        ## otherwise
        newTargetSum = targetSum - root.val
        if root.right:
            out = self._dfs(root.right, newTargetSum)
            if out is True:
                return True
            
        if root.left:
            out = self._dfs(root.left, newTargetSum)
            if out is True:
                return True
            
        return False
        
        