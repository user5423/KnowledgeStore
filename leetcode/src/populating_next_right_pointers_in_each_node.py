"""
# Definition for a Node.
class Node:
    def __init__(self, val: int = 0, left: 'Node' = None, right: 'Node' = None, next: 'Node' = None):
        self.val = val
        self.left = left
        self.right = right
        self.next = next
"""

class Solution:
    def connect(self, root: 'Optional[Node]') -> 'Optional[Node]':
        ## cases
        ## 1. root is NOne
        ## -- return None
        
        ## 2. root.right == root.left == None
        ## -- root.next = None; return root
        
        ## 3. otherwise
        return self._bfs(root)
    
    def _bfs(self, root):
        if root is None:
            return root
        
        q = collections.deque([(root, 0)])
        while len(q):
            node, currentLevel = q.popleft()
            
            ## custom
            if len(q) > 0 and q[0][1] == currentLevel:
                node.next = q[0][0]

            ## bfs
            if node.left:
                q.append((node.left, currentLevel + 1))
            if node.right:
                q.append((node.right, currentLevel + 1))
                
        return root
    
    
    def _bfs_fast(self, root):
        if root is None:
            return root
        
        nextNode = (root, 0)
        firstNode = (root, 0)
        while nextNode:
            root, currentLevel = nextNode

            if root.left:
                if root.right:
                    root.left.next = root.right
                elif root.next and root.next.left:
                    root.left.next = root.next.left
                else:
                    root.left.next = root.next.right
            if root.right:
                if root.next and root.next.left:
                    root.right.next = root.next.left
                elif root.next and root.next.right:
                    root.right.next = root.next.right

                
        return root
    
    
    def _dfs(self, root):
        if root is None:
            return root
        
        if root.left:
            if root.right:
                root.left.next = root.right
            elif root.next and root.next.left:
                root.left.next = root.next.left
            else:
                root.left.next = root.next.right
        if root.right:
            if root.next and root.next.left:
                root.right.next = root.next.left
            elif root.next and root.next.right:
                root.right.next = root.next.right
            
        self._dfs(root.left)
        self._dfs(root.right)
        return root
    
    def _dfs_fast(self, root):
        if root is None:
            return None
        
        if root.left:
            if root.right:
                root.left.next = root.right
            elif root.next and root.next.left:
                root.left.next = root.next.left
            else:
                root.left.next = root.next.right
        
        if root.right and root.next:
            if root.next.left:
                root.right.next = root.next.left
            else:
                root.right.next = root.next.right
            
        self._dfs(root.left)
        self._dfs(root.right)
        return root
    
        
        
        
            