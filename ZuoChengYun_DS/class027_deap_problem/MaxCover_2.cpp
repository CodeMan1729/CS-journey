// https://leetcode.cn/problems/divide-intervals-into-minimum-number-of-groups/

class Solution 
{
public:
    int minGroups(vector<vector<int>>& intervals) 
    {
        auto cmp = [](const vector<int> a, const vector<int> b){
            return a[0] < b[0];
        };

        sort(intervals.begin(), intervals.end(), cmp);

        priority_queue<int, vector<int>, greater<int>> pq; 

        int numbers = 0;
        for (const auto& interval : intervals) 
        {
            while (!pq.empty() && pq.top() < interval[0]) 
            {
                pq.pop();
            }
        pq.push(interval[1]);

        numbers = max(numbers, (int)pq.size());
        }

        return numbers;
    }


};
