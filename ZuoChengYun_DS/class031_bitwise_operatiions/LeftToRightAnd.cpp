// https://leetcode.cn/problems/bitwise-and-of-numbers-range/

class Solution 
{
public:
    int rangeBitwiseAnd(int left, int right) 
    {
        while(right > left)
        {
            right -= (right & -right);
        }

        return right;
    }
};
