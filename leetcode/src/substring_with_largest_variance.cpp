#include <string>
#include <unordered_map>
#include <cmath>
#include <vector>
#include <limits.h>
#include <cassert>

class Solution {
public:
    int largestVariance(std::string s) {
        // return this->basic_largestVariance(s);
        return this->kadanes_largestVariance(s);
    }

    int basic_largestVariance(std::string s){
        // O(n^2) time complexity, O(n) space complexity
        int maxVariance = 0;
        int strSize = s.size();

        // Base case: if strSize == 1 (then variance is 0 and we never enter for loop)
        // Inductive: if strSize == 2 (then we execute outer loop, and inner loop)
        for (int i = 0; i < strSize - 1; i++){
            std::unordered_map<char, int> charCounter;
            charCounter.insert({s[i], 1});

            int _maxCount = 1;
            int remainderStrSize = strSize - i;
            std::vector<int> _maxCounts(remainderStrSize, 0);
            _maxCounts[0] = 1;
            
            {
                // Base case: variance of single char is 0 --> no need to update maxVariance
                for (int j = i+1; j < strSize; j++){
                    auto it = charCounter.find(s[j]);
                    if (it == charCounter.end()){
                        charCounter.insert({s[j], 1});
                    } else {
                        it->second++;
                        if (it->second > _maxCount){
                            _maxCount++;
                        }
                    }
                    _maxCounts[j-i] = _maxCount;
                }
            }

            int _minCount = INT_MAX;
            for (auto it = charCounter.begin(); it != charCounter.end(); it++)
                _minCount = std::min(_minCount, it->second);

            std::vector<int> _minCounts(remainderStrSize, 0);
            _minCounts[0] = 1;


            {
                for (int j = strSize - 1; j > i; j--){
                    auto &count = charCounter[s[j]];                    
                    assert( j-i >= 0 && j-i < remainderStrSize);
                    _minCounts[j-i] = _minCount;
                    count--;

                    if (count == 0)
                        charCounter.erase(s[j]);
                    else
                        _minCount = std::min(_minCount, count);

                }
            }

            // Now that we have both mins and maxs we can calculate
            for (int k = 0; k < remainderStrSize; k++){
                int diff = _maxCounts[k] - _minCounts[k];
                maxVariance = std::max(maxVariance, diff);
            }
        }

        return maxVariance;
    }

    int kadanes_largestVariance(std::string &s){
        // O(26^2 * O(n)) = O(n) time complexity and O(n) space complexity
        int maxVariance = 0; // guarnateed at least value of 0 - e.g. single character substr
        std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

        for (char &c1: alphabet){
            for (char &c2: alphabet){
                // we skip if they are the same characters -- variance would be 0
                if (c1 == c2)
                    continue;
                
                // otherwise we determine the max variance using c1 and c2 for any substr in s
                maxVariance = std::max(maxVariance, this->_kadane_largestVariance(c1, c2, s));
            }
        }
        return maxVariance;
    }

    int _kadane_largestVariance(char &c1, char &c2, std::string &s){
        int maxVariance = 0; // guaranteed at least 0 value -- e.g. single character substr
        int balance = 0;
        bool encounteredC2 = false;
        bool canCountC2 = true;

        for (char &c: s){
            // if the current char is not in focus, then we skip to the next character
            if (c != c1 && c != c2)
                continue;

            // otherwise we process it
            if (c == c1)
                balance++;
            else {
                encounteredC2 = true;
                if (!canCountC2)
                    canCountC2 = true;
                else
                    balance--;

                if (balance < 0){
                    balance = -1;
                    // avoids counting c2's unneccessarily
                    canCountC2 = false;
                }

            }

            if (encounteredC2)
                maxVariance = std::max(maxVariance, balance);

        }

        return maxVariance;
    }


    // once we have decreased to variance = 0 for str[k:l]
    // - there is the same amount of occurrence for each letter
    // - if there is a single letter, we want to grow it
    // - if there are multiple letters, we can potentially improve our variance
    //      - we can start dropping letters from str[k:] onwards
    //      -
    int calculateMaxVariance(std::vector<int> &minCounts, std::vector<int> &maxCounts){
        int maxVariance = 0;
        for (int i = 0; i < minCounts.size(); i++){
            maxVariance = std::max(maxVariance, maxCounts[i] - minCounts[i]);
        }
        return maxVariance;
    }

    // O(len(s)) time complexity and O(unique(s)) space complexity
    int basic_variance(std::string s){
        int maximum = 0;
        int minimum = INT_MAX;
        // We find the number of unique chars in the string s
        std::unordered_map<char, int> charCounter;

        for (char &c: s){
            auto it = charCounter.find(c);
            if (it == charCounter.end())
                charCounter.insert(std::make_pair(c, 1));
            else
                charCounter[c]++;
        }

        // We then loop through the map to find minimum and maximum
        for (auto it = charCounter.begin(); it != charCounter.end(); it++){
            int count = it->second;
            minimum = std::min(minimum, count);
            maximum = std::max(maximum, count);
        }

        return (maximum - minimum);
    }
};