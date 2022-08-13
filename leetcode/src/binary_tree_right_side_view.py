# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class Solution:
    def rightSideView(self, root: Optional[TreeNode]) -> List[int]:
        ## cases
        ## 1. root is NOne
        ## -- return []
        
        ## 2. root.right is None and root.left is None:
        ## -- return [root.val]
        
        ## 3. otherwise
        
        if root is None:
            return []
        
        visitedLevels = set()
        view = []
        
        q = collections.deque([(root, 0)])
        while len(q):
            node, level = q.popleft()
            if level not in visitedLevels:
                view.append(node.val)
                visitedLevels.add(level)
                
            ## Remember!! right first
            if node.right:
                q.append((node.right, level+1))
            if node.left:
                q.append((node.left, level+1))
                
        return view
                
            
        