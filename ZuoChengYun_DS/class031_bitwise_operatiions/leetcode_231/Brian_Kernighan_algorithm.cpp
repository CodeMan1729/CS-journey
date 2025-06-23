// https://leetcode.cn/problems/power-of-two/

// 2的幂的二进制只有一位是1，其余都是0

class Solution
{
public:
    bool isPowerOfTwo(long long n) 
    {
        return n>0 && (n == (n & -n));
    }
};
