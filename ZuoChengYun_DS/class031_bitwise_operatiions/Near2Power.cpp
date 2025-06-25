#include <iostream>
#include <string>

using namespace std;

// n = 1001 --> 01111 --> 10000

int Near2power(int n)
{
    if(n <= 0)
    {
        return -1;
    }
    
    // n--;
    
    // 将左边第一位1及其左边全变成1， 然后+1，因为大于n的最小2次幂就是他
    n |= (n >> 1); // 使得第一个1以内，前2为全为1
    n |= (n >> 2); // 使得第一个1以内，前4为全为1
    n |= (n >> 4);
    n |= (n >> 8);
    n |= (n >> 16);
    
    n += 1;
    
    return n;
}

int main()
{
    int number = Near2power(15);
    cout << number;
}
