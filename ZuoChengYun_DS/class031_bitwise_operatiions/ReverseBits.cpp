// https://leetcode.cn/problems/reverse-bits/submissions/639640910/

// 骚已经不能形容了，故我起名字《离骚》

class Solution 
{
public:
    uint32_t reverseBits(uint32_t n) 
    {
        n = ((n & 0xaaaaaaaa)  >> 1) | ((n & 0x55555555) << 1);
        n = ((n & 0xcccccccc)  >> 2) | ((n & 0x33333333) << 2);
        n = ((n & 0xf0f0f0f0)  >> 4) | ((n & 0x0f0f0f0f) << 4);
        n = ((n & 0xff00ff00)  >> 8) | ((n & 0x00ff00ff) << 8);
        n = (n >> 16) | (n << 16);

        return n;
    }
};
