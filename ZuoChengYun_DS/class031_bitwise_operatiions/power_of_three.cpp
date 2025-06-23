// https://leetcode.cn/problems/power-of-three/

#include <limits.h>

class Solution 
{
public:
    int max_3_power()
    {
        long long max = 3;
        while(max <= INT_MAX)
        {
            max *= 3;
        }

        return (int)(max / 3);
    }
    
    bool isPowerOfThree(int n) 
    {
        int max = max_3_power();
        return n>0 && (max % n == 0);
    }
};
