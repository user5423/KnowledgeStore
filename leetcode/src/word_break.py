class Solution:
    def wordBreak(self, s: str, wordDict: List[str]) -> bool:
        self.wordDict = set([word.lower() for word in wordDict])
        self.s = s.lower()
        self.largest = self.selectLargest()
        self.output = False
        self.memoization: Dict[Tuple[index: int, substr: int], bool] = {}
        self.recurse(0)
        return self.output      

        
    def selectLargest(self) -> int:
        length = 0
        for word in self.wordDict:
            if len(word) > length:
                length = len(word)
        return length
        
    def recurse(self, index: int) -> bool:
        substr = ""
        ## main cases
        for i in range(index, len(self.s)):
            substr += self.s[index]
            if substr in self.wordDict:
                memoizedOut = self.memoization.get((index, len(substr)))
                if memoizedOut is None:
                    out = self.recurse(index+1,)
                    if out is True:
                        return True
                    else:
                        self.memoization[(index, len(substr))] = False
                        
            ## Speedup to avoid useless iterations
            if len(substr) > self.largest:
                break

            index += 1
            
        ## base case
        if substr in self.wordDict:
            self.output = True
            return True
        return False
    
    
        
        