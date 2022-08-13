class Solution:
    def rotate(self, matrix: List[List[int]]) -> None:
        """
        Do not return anything, modify matrix in-place instead.
        """

        ## how many attemps
        for i in range(len(matrix) // 2):
            ## how many rotations
            rowEnd = len(matrix) - i - 1
            rowStart = i
            rowBottom = rowEnd
            for j in range(i, rowEnd):
                ## how to rotate the set of four elements
                prev = matrix[i][j]

                ## 1.
                newJ = rowEnd
                newI = j
                newPrev = matrix[newI][newJ]
                matrix[newI][newJ] = prev
                prev = newPrev

                ## 2.
                newJ = len(matrix) - j - 1
                newI = rowBottom
                newPrev = matrix[newI][newJ]
                matrix[newI][newJ] = prev
                prev = newPrev

                ## 3. 
                newJ = rowStart
                newI = len(matrix) - j - 1
                newPrev = matrix[newI][newJ]
                matrix[newI][newJ] = prev
                prev = newPrev

                ## 4.
                newJ = j
                newI = i
                newPrev = matrix[newI][newJ]
                matrix[newI][newJ] = prev

