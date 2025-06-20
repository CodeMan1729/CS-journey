// https://leetcode.cn/problems/single-number-ii/description/

class Solution 
{
public:
    int singleNumber(vector<int>& nums) 
    {
        return find(nums, 3);
    }

    int find(vector<int>& nums, int m)
    {
        // 统计所有数字一共每一位1的数量
        // static int count[32]; // 静态变量默认初始化为 0, 但是leetcode平台所有例子只用这一个count，上一次测试用例的结果会污染下一次的！
        int count[32] = {};
        for(auto& num : nums)
        {
            for(int i = 0; i < 32; i++)
            {
                count[i] += (num >> i) & 1; // 查看第i - 1位
            }
        }

        int ans = 0;

        // 若整除m，则为1
        for(int i = 0; i < 32; i++)
        {
            if(count[i] % m != 0)
            {
                ans ^= 1 << i; // ^ 或者 | 都可以
            }
        }

        return ans;
    }
};
