"""
# Definition for a Node.
class Node:
    def __init__(self, val=None, children=None):
        self.val = val
        self.children = children
"""

class Solution:
    def maxDepth(self, root: 'Node') -> int:
        self.depth = 0
        if root is None:
            return self.depth
        
        self._dfs(root)
        return self.depth
        
        
    def _dfs(self, node, counter=1):
        self.depth = max(self.depth, counter)
        for child in node.children:
            self._dfs(child, counter+1)
            