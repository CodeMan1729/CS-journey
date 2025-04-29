#include<iostream>
#include<random>

using namespace std;

int min(int a,int b)
{
  if(a<b) {return a;}
  else {return b;}
}

void a_bigger_b(int& a,int& b)
{
  if(a <b)
  {
    int temp = a;
    a = b;
    b = temp;
  }
  return;
}

int gcd(int a,int b)
{
  return b==0 ? a : gcd(b,a%b);//avoid deep stack
}

int lcm(int a,int b)
{
  return (a*b) / (gcd(a,b));
}

int nthMagicNumber(int n,int a,int b)
{
  size_t ans = 0;
  size_t lcm_val = lcm(a,b);
  size_t r = (size_t)(n * min(a,b));
  size_t m = 0;//middle point

  for(size_t l = 0;l<=r;)
  {
    m = (l+r)/2;

    if((m/a + m/b - m/lcm_val) >= n)// [0,m] : the number of satisfied numbers
    {
      ans = m;
      r = m-1;
    }
    else{
      l = m+1;
    }
  }

  return (int)(ans % 1000000007);
}


int main()
{
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0,1000);

  int a = 0;
  int b = 0;

  for(int i = 0;i<20;i++)//number of experiment
  {
    a = dis(gen);
    b = dis(gen);

    cout << a << " " << b;
    a_bigger_b(a,b);
    cout << " Gcd is " << gcd(a,b) << '\n';
  }
}
