// 测试链接：https://www.nowcoder.com/practice/d2707eaf98124f1e8f1d9c18ad487f76

#include<iostream>
#include<stdint.h>

using namespace std;

                       // 因为后面返回a的两个条件都为非负，所以把非负设置为1 
static int sign(int n) // 非负数返回1，负数返回0
{
    int n_sign = (uint32_t)(n) >> 31;
    n_sign ^= 1;
    return n_sign;
}

int getMax(int a, int b) 
{
    int c = a - b; // 可能溢出
    int sa = sign(a);
    int sb = sign(b);
    int sc = sign(c);
    
    // 两种情况返回a :
    //    (1) a, b符号相同，且c非负
    //    (2) a, b符号不同，且a非负    
    int diffAB = sa ^ sb; // 不同为1，相同为0
    int sameAB = diffAB ^ 1; // 反一下就对了
    
    int returnA = diffAB * sa + sameAB * sc; // A大，则为1，否则为0
    int returnB = returnA ^ 1;
    
    return returnA * a + returnB * b; 
}

// 总结：M * x + N * y中，M, N只能同时成立一个

int main(void)
{
    cout << getMax(3,-3);
    return 0;
}
