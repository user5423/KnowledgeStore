# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class Solution:
    def binaryTreePaths(self, root: Optional[TreeNode]) -> List[str]:
        
        return self._iterative(root)
    
    def _iterative(self, root):
        
        if root is None:
            return []
        if root.left is None and root.right is None:
            return [str(root.val)]
        
        ## (node, path)
        q = collections.deque([(root, str(root.val))])
        paths = []
        
        while len(q):
            node, path = q.pop()
            
            ## base case
            if node.left is None and node.right is None:
                paths.append(path)
                continue
                
            ## otherwise
            if node.right:
                q.append((node.right, f"{path}->{node.right.val}" ))
            if node.left:
                q.append((node.left,  f"{path}->{node.left.val}" ))
  
        return paths
        