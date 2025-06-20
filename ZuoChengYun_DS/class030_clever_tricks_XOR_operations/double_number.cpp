// 测试链接：https://leetcode.cn/problems/single-number-iii/

// 测试用例有使用int的最大负数，没法表示他的相反数，所以使用long long 64位兜一下

class Solution
{
public:
    vector<int> singleNumber(vector<int>& nums) 
    {
        long long eorSum = 0;
        for(auto& elem : nums)
        {
            eorSum ^= elem;
        } // eorSum = a ^ b;

        long long rightBit_Binary = eorSum & (-eorSum);
        
        //筛选出来这一位是1的
        long long eorSum_1 = 0;
        for(auto& elem : nums)
        {
            // if((elem & rightBit_Binary) == rightBit_Binary) // 找这一位是1的数字
            if((elem & rightBit_Binary) == 0) // 找这一位是0的数字
            {
                eorSum_1 ^= elem;
            }
        }

        vector<int> ans = {(int)eorSum_1, (int)(eorSum_1 ^ eorSum)};

        return ans;
    }
};
