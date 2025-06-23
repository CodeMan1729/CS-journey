// 4 100
// 16 10000
// 64 1000000
// 256 100000000

// https://leetcode.cn/problems/power-of-four/

#include <vector>
#include <limits>

using namespace std;

static vector<int> OnlyFour;

class Solution 
{
public:
    void AllOfPowerFour()
    {
        long long num = 1;
        while(num <= INT_MAX)
        {
            OnlyFour.push_back(num);
            num *= 4;
        }
    }

    bool isPowerOfFour(int n) 
    {
        AllOfPowerFour();

        for(auto& elem : OnlyFour)
        {
            if(elem == n)
            {
                OnlyFour.clear();
                return true;
            }
        }

        OnlyFour.clear();
        return false;
    }
};
