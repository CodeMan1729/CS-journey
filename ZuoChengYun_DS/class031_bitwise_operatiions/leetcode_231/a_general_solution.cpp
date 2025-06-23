#include <limits.h>

class Solution 
{
public:
    long long findMax()
    {
        long long max = 2;

        while(max < INT_MAX)
        {
            max *= 2;
        }

        return max / 2; // 会多乘一次2
    }

    bool isPowerOfTwo(int n) 
    {
        long long max = findMax();

        return n>0 && (max % n == 0);
    }
};
