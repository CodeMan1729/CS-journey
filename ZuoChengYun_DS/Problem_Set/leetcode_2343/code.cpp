class Solution {
public:
    vector<int> smallestTrimmedNumbers(vector<string>& nums, vector<vector<int>>& queries) {
        int n = nums.size();         // nums 的数量
        int m = nums[0].size();      // 每个字符串的长度
        vector<vector<int>> vecs(m + 1); // 存储每轮排序后的下标
        
        // 初始化第 0 轮，所有 nums 的下标
        for (int i = 0; i < n; i++) {
            vecs[0].push_back(i);
        }

        // 逐位进行基数排序，从低位到高位
        for (int i = 1; i <= m; i++) 
        {
            vector<vector<int>> buckets(10); // 10 个桶，分别对应数字 0-9

            // 根据当前位（从右边第 i 位）将下标放入桶中
            for (int idx : vecs[i - 1]) 
            {
                int digit = nums[idx][m - i] - '0'; // 提取当前位的数字 + 两个技巧
                buckets[digit].push_back(idx);
            }

            // 将桶中的下标按顺序合并到 vecs[i]
            for (int j = 0; j < 10; j++) 
            {
                for (int idx : buckets[j]) 
                {
                    vecs[i].push_back(idx);
                }
            }
        }

        // 处理查询
        vector<int> answer;
        for (auto& q : queries) 
        {
            int k = q[0];        // 第 k 小
            int trim_bit = q[1]; // 裁剪位数
            answer.push_back(vecs[trim_bit][k - 1]); // 从排序结果中取第 k-1 个下标
        }

        return answer;
    }
};
