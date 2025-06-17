#include <vector>

using namespace std;

class Solution 
{
public:
    int missingNumber(vector<int>& nums) 
    {
        int eorALL = 0;
        int eorVAL = 0;
        int length = nums.size();

        for(int i = 0; i < length; i++)
        {
            eorVAL ^= nums[i];
            eorALL ^= i;
        }
        eorALL ^= length;

        return eorALL ^ eorVAL;
    }
};
