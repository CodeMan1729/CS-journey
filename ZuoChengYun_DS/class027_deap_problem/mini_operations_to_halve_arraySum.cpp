// https://leetcode.cn/problems/minimum-operations-to-halve-array-sum/

class Solution 
{
public:
    int halveArray(vector<int>& nums) 
    {
        size_t count = 0;
        double sum = 0;
        priority_queue<double, vector<double>> pq;

        for(int items : nums)
        {
            pq.push(items);
            sum += items;
        }

        if(pq.empty())
        {
            return 0;
        }

        sum = sum / 2;
        double limit = sum;
        double now_sum = 0;
        double temp = 0;

        while(now_sum < limit)
        {
            if(!pq.empty())
            {
                now_sum += pq.top() / 2;
                temp = pq.top() / 2;
                pq.pop();
                pq.push(temp);
                count++;
            }            
        }

        return count;
    }
};
