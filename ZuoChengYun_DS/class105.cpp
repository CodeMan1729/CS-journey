#include<iostream>

static const base = 19937;

static int v(char c)
{
  if(c>='0' && c<='9')
  {
    return c-'0'+1;
  }
  else if(c>='A' && c<='Z')
  {
    return c-'A'+11;
  }
  else if(c>='a' && c<='z')
  {
    return c-'a'+37;
  }
}

long value(char* s)
{
  long ans = v(s[0]);
  for(int i = 1;i<(sizeof(s)/sizeof(s[0]) - 1);i++)
  {
    ans = base * ans + v(s[i]);
  }
  return ans;
}

int main()
{
  
}

