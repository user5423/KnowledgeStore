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
    def connect(self, root: 'Node') -> 'Node':
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
    