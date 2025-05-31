#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

// 基数排序
// 时间复杂度：O(mn) = O(n)    !!! 但有限制：
//                                      (1) 对于存在负数的数组，涉及到arr[i]-min，可能会溢出
//                                      (2) 数据对基数敏感。同一组数组，不同基数花费的时间不同

// 空间复杂度：O(n + m), 基数为m，同时需要中间容器(和排序的容器一样的类型)存储中间结果

class Solution
{
private:
    static constexpr const size_t BASE = 10;

public:
    vector<int> sortArray(vector<int>& nums)
    {
        int n = nums.size();
        // 找出min max, 所以元素-min
        int min = nums[0];
        int max = nums[0];

        for (int i : nums)
        {
            min = std::min(min, i);
        }

        for (int i = 0; i < nums.size(); i++)
        {
            nums[i] = nums[i] - min;
            max = std::max(max, nums[i]); // 假设不溢出
        }

        size_t bits = cal_bits(max);
        vector<int> help(n);
        vector<int> help_before(n);
        int qzh[BASE];
        int buffer = 1;

        help_before = nums;

        for (int i = 0; i < BASE; i++)
        {
            qzh[i] = 0;
        }

        // 循环k次，k为当前进制下max的位数
        for (int i = 0; i < bits; i++)
        {
            // qzh清零
            for (int i = 0; i < BASE; i++)
            {
                qzh[i] = 0;
            }

            // 使用数字提取第(i+1)位，用前缀和技巧计数（数组存储）
            for (int j = 0; j < n; j++)
            {
                qzh[(nums[j] / buffer) % BASE]++;
            }

            // 前缀和技巧
            for (int j = 0; j < BASE; j++)
            {
                if (j - 1 >= 0)
                {
                    qzh[j] += qzh[j - 1];
                }
            }

            // 从n-1 -> 0位置放置
            int index = 0;
            for (int j = n - 1; j >= 0; j--)
            {
                index = --qzh[(help_before[j] / buffer) % BASE]; // !!! 存储中间结果  !!!
                help[index] = help_before[j];
            }
            buffer *= BASE;

            help_before = help;
        }

        // 所以元素+min
        for (int i = 0; i < n; i++)
        {
            help[i] += min;
        }

        // help -> nums 
        for (int i = 0; i < n; i++)
        {
            nums[i] = help[i];
        }

        return nums;
    }

    size_t cal_bits(int num)
    {
        size_t bits = 0;
        while (num > 0)
        {
            bits++;
            num = num / 10;
        }
        return bits;
    }
};

int main()
{
    Solution s;
    vector<int> vec = {-1,2,-8,-10};
    s.sortArray(vec);

    for (auto i : vec)
    {
        cout << i << " ";
    }
}
