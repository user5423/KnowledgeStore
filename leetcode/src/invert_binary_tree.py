# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class Solution:
    def invertTree(self, root: Optional[TreeNode]) -> Optional[TreeNode]:
        if root is None:
            return root
        elif root.left is None and root.right is None:
            return root
        
        q = collections.deque([root])
        
        while len(q):
            node = q.pop()
            
            ## switch right and left
            left = node.left
            node.left = node.right
            node.right = left
            
            ## we then add to q
            if node.left:
                q.append(node.left)
            if node.right:
                q.append(node.right)
            
        return root
        