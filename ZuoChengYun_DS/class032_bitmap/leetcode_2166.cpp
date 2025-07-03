// https://leetcode.cn/problems/design-bitset/

#include <sstream>

class Bitset 
{
private:
    int* set = nullptr;
    const int size;
    bool reversed;
    size_t ones; // 不是代表实际存储时的1，而是存在
    size_t zeros; // 代表这一位不存在

public:
    // size个数：0~size-1, set剩下的位用不到
    Bitset(int n) : size(n), reversed(false), zeros(n), ones(0) 
    {
        set = new int[(n + 31) / 32]();
    }

    ~Bitset()
    {
        delete[] set;
    }
    
    void fix(int idx) 
    {
        int index = idx / 32;
        int shift = idx % 32;

        if(reversed == false) // 0为不存在，1为存在
        {
            // 如果这一位为1，则不用去修改
            if( (set[index] & (1 << shift)) == 0) // 这一位之前不存在
            {
                set[index] |= (1 << shift);
                zeros--;
                ones++;
            }
        }
        else{ // 0为存在，1为不存在
            // 如果这一位为0，则不用去修改
            if( (set[index] & (1 << shift)) != 0) // 这一位之前不存在
            {
                set[index] ^= (1 << shift);
                zeros--;
                ones++;
            }
        }
    }
    
    void unfix(int idx) 
    {
        int index = idx / 32;
        int shift = idx % 32;

        if(reversed == false) // 0代表不存在，1代表存在
        {
            // 若这一位是0，不用修改
            if( (set[index] & (1 << shift)) != 0)// 为1，要修改
            {
                set[index] &= ~(1 << shift);
                ones--;
                zeros++;
            }

        }   
        else{ // 1代表不存在，0代表存在
            // 若这一位是1，不用修改
            if( (set[index] & (1 << shift)) == 0) // 为0，要修改
            {
                set[index] |= (1 << shift);
                ones--;
                zeros++;
            }
        }         
    }
    
    void flip() 
    {
        reversed = !reversed;    

        size_t temp = zeros;
        zeros = ones;
        ones = temp;        
    }
    
    bool all() 
    {
        return ones == size;
    }
    
    bool one() 
    {
        return ones > 0;    
    }
    
    int count() 
    {
        return (int)ones;    
    }
    
    std::string toString() 
    {
        std::stringstream all_conditions;

        for(int i = 0, k = 0; i < size; k++)
        {
            int number = set[k];
            for(int j = 0; j < 32 && i < size; i++, j++) // 防止最后一个整数超过size
            {
                int status = (number >> j) & 1;
                status ^= reversed; // 恢复
                all_conditions << status;
            }
        }

        return all_conditions.str();
    }
};

/**
 * Your Bitset object will be instantiated and called as such:
 * Bitset* obj = new Bitset(size);
 * obj->fix(idx);
 * obj->unfix(idx);
 * obj->flip();
 * bool param_4 = obj->all();
 * bool param_5 = obj->one();
 * int param_6 = obj->count();
 * string param_7 = obj->toString();
 */
