# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class Solution:
    def isUnivalTree(self, root: Optional[TreeNode]) -> bool:
        if root is None:
            return True
        
        val = root.val
        q = collections.deque([root])
        while len(q):
            node = q.pop()
            
            if node.val != val:
                return False
            
            if node.left:
                q.append(node.left)
            if node.right:
                q.append(node.right)
            
        return True