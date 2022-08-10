class Solution:
    def spiralOrder(self, matrix: List[List[int]]) -> List[int]:
        m = len(matrix)
        n = len(matrix[0])
        
        if m == 1:
            return matrix[0]
        elif n == 0:
            return []

        ## These are ordered
        directions = [
            ("RIGHT", (0,1)),
            ("DOWN", (1, 0)),
            ("LEFT", (0,-1)),
            ("UP", (-1,0)),
        ]
        
        bounds = {
            "RIGHT": {"m": (1,0), "n": (0,0)},
            "DOWN": {"m": (0,0), "n": (0,-1)}, 
            "LEFT": {"m": (0,-1), "n": (0,0)}, 
            "UP": {"m": (0,0), "n": (1,0)}, 
        }
        
        dirs = ["RIGHT", "DOWN", "LEFT", "UP"]
        
        arr = [matrix[0][0]]
        directionIndex = 0
        currentPosition = [0,0]
        
        lowerN = 0
        lowerM = 0
        upperN = n
        upperM = m
        
        ## There are X elements, but X - 1 hops
        counter = 0
        while counter < m*n - 1 :
            ## we get the current direction
            vector = directions[directionIndex][1]
            
            ## get the potential new direction
            newPosition = [*currentPosition]
            newPosition[0] += vector[0]
            newPosition[1] += vector[1]
            
            ## if this violates boundary constraints then we switch over
            if not (lowerM <= newPosition[0] < upperM) or not(lowerN <= newPosition[1] < upperN):
                ## shrink bounds
                bound = bounds[dirs[directionIndex]]
                lowerM += bound["m"][0]
                upperM += bound["m"][1]
                lowerN += bound["n"][0]
                upperN += bound["n"][1]
                ## switch direction
                directionIndex = (directionIndex + 1) % len(directions)
            else:
                ## append the current value
                counter += 1
                currentPosition = newPosition
                arr.append(matrix[currentPosition[0]][currentPosition[1]])
        
        return arr
                
            
        
        
        