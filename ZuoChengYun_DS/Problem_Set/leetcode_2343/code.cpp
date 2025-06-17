#include <vector>
#include <string>

using namespace std;

// 找到 nums 中第 ki 小数字对应的 下标 <----> 基数排序有稳定性

class Solution 
{
private:
    size_t BASE = 10; // 因为题目的字符串数组时10进位！

public:
    vector<int> smallestTrimmedNumbers(vector<string>& nums, vector<vector<int>>& queries) 
    {
        // 搞清楚本次字符数字有几位
        size_t n = nums[0].length();
        size_t nums_size = nums.size();

        vector<vector<int>> bucket(BASE);
        vector<vector<int>> ans_list(n + 1); // 装入nums的下标

        // 第0次排序放nums
        for(int i = 0; i < nums_size; i++)
        {
            ans_list[0].push_back(i);
        }

        // 基数排序+存储每轮结果
        for(int i = 1; i <= n; i++)
        {
            // 从左到右遍历，装入桶
            for(auto& elem : ans_list[i - 1]) // item为nums的某一位的下标
            {
                int digit = nums[elem][n - i] - '0';
                bucket[digit].push_back(elem);
            }

            // 依次从桶里倒出
            for(int j = 0; j < BASE; j++)
            {
                for(auto& elem : bucket[j])
                {
                    ans_list[i].push_back(elem);
                }
                bucket[j].clear();
            }
        }

        // 查询即可
        size_t queries_size = queries.size();
        vector<int> answer;
        for(auto& elem : queries)
        {
            int k = elem[0];
            int trim = elem[1];

            answer.push_back(ans_list[trim][k - 1]);
        }

        return answer;
    }
};
