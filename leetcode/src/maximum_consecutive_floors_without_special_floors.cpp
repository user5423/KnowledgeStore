#include <vector>
#include <algorithm>
#include <cmath>

class Solution {
public:
    int std::maxConsecutive(int bottom, int top, vector<int>& special) {
        // O(n log n) time complexity and O(n) space complexity

        // Sorts the array, lowest to highest in O(n log n) time
        std::sort(special.begin(), special.end());

        // For cases with complete contiguity:
        int specialCount = special.size();
        int lastSpecialFloor = special[specialCount -1];
        int firstSpecialFloor = special[0];
        if (bottom > lastSpecialFloor)
            return top - bottom + 1;
        else if (bottom == lastSpecialFloor)
            // not inclusive
            return top - bottom;
        else if (top < firstSpecialFloor)
            return top - bottom + 1;
        else if (top == firstSpecialFloor)
            // not inclusive
            return top - bottom;

        // For cases with broken contiguity:
        int largestBlockSize = 0;
        int currentBlockSize = 0;
        if (bottom < firstSpecialFloor){
            currentBlockSize = firstSpecialFloor - bottom; // non-inclusive
            largestBlockSize = std::max(largestBlockSize, currentBlockSize);
        }

        // We then iterate through
        int i = 0;
        for (i = 0; i < specialCount - 1; i++){
            int currentSpecialFloor = special[i];
            int nextSpecialFloor = special[i+1];

            // We keep on skipping iterations until we choose to deal with it
            if (bottom >= nextSpecialFloor)
                continue;

            // At this point, bottom >= currentSpecialFloor
            currentBlockSize = std::min(nextSpecialFloor-1, top) - std::max(currentSpecialFloor + 1, bottom) + 1;
            largestBlockSize = std::max(largestBlockSize, currentBlockSize);
            
            if (top <= nextSpecialFloor)
                break;
        }

        if (i == specialCount - 1){
            currentBlockSize = top - lastSpecialFloor;
            largestBlockSize = std::max(largestBlockSize, currentBlockSize);
        }

        return largestBlockSize;

    }
};
Console
