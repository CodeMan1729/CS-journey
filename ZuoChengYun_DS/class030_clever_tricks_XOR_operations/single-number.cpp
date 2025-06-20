// https://leetcode.cn/problems/single-number/
//
//这适用于，除了一个数字出现了奇数次，其他都出现了偶数次的数字。需要找到这个奇数次的数字。

class Solution {
public:
    int singleNumber(vector<int>& nums) 
    {
        int eorSum = 0;
        for(auto& item : nums)
        {
            eorSum ^= item;
        }    
        return eorSum;
    }
};
