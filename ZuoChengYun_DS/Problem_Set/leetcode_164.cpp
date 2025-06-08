// 时间复杂度：O(n)
// 空间复杂度：O(n)

#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

class Solution
{
private:
    static const size_t BASE = 10;

public:
    int maximumGap(vector<int>& nums)
    {
        if (nums.size() < 2)
        {
            return 0;
        }

        int max = nums[0];
        int min = nums[0];
        int l = (int)nums.size();
        vector<int> help = nums;
        vector<int> mid_status = nums;
        size_t qzh[BASE];
        size_t buffer = 1;

        // 初始化前缀和
        for (int i = 0; i < BASE; i++)
        {
            qzh[i] = 0;
        }

        // 最大值，最小值
        for (auto& i : nums)
        {
            max = std::max(max, i);
            min = std::min(min, i);
        }

        // 数组-min
        for (auto& item : mid_status)
        {
            item -= min;
        }

        for (auto& item : help)
        {
            item -= min;
        }

        // 先基数排序 O(n)
        for (int i = 0; i < bits(max); i++)
        {
            // 用求位数的技巧，填充前缀和数组
            for (auto item : help)
            {
                qzh[(item / buffer) % BASE]++;
            }

            // 把前缀和数组前缀化
            for (int j = 0; j < BASE; j++)
            {
                if (j != 0)
                {
                    qzh[j] += qzh[j - 1];
                }
            }

            // 从后往前，放置数组
            for (int j = l - 1; j >= 0; j--)
            {
                int index = --qzh[(mid_status[j] / buffer) % BASE];
                help[index] = mid_status[j];
            }

            mid_status = help;

            // 前缀和数组归零
            for (int i = 0; i < BASE; i++)
            {
                qzh[i] = 0;
            }

            buffer *= BASE;
        }

        // 基数排序的结果：1,999999

        // 还原
        for (auto& i : help)
        {
            i += min;
        }
        
        // 找相邻的最大差值
        int diff = 0;
        for (int i = 1; i < l; i++)
        {
            diff = std::max(diff, help[i] - help[i - 1]);
        }
        return diff;
    }

    size_t bits(int number)
    {
        size_t bit = 0;
        while (number > 0)
        {
            bit++;
            number = number / (int)BASE;
        }
        return bit;
    }
};
