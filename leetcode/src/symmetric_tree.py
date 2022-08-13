# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class Solution:
    def isSymmetric(self, root: Optional[TreeNode]) -> bool:
        
        if root is None:
            return True
        elif root.right is None and root.left is None:
            return True
        elif (root.right is None) != (root.left is None):
            return False
        
        
        ## right here we should have a root with left and right
        
        leftQ = collections.deque([root.left])
        rightQ = collections.deque([root.right])
        
        while len(leftQ) and len(rightQ):
            lnode = leftQ.pop()
            rnode = rightQ.pop()
            
            ## check if values are the same
            if lnode.val != rnode.val:
                return False
            
            ## check if children structure is the same
            if (lnode.left is None) != (rnode.right is None):
                return False
            elif (lnode.right is None) != (rnode.left is None):
                return False
            
            ## add children to trees
            if lnode.left:
                leftQ.append(lnode.left)
                rightQ.append(rnode.right)
                
            if lnode.right:
                leftQ.append(lnode.right)
                rightQ.append(rnode.left)
                
            
        if len(leftQ) == len(rightQ) == 0:
            return True
        
        return False