#include<iostream>

static const int MAX = 50000;

static int nums[MAX];
static int help[MAX];

int reversePairs_benchmark(int* nums,int n)
{
  int count = 0;
  for(int i=0;i<n;++i)
  {
    for(int j=i+1;j<n;++j)
    {
      if(nums[i] > ((long)nums[j]) * 2) {++count;}
    } 
  }

  return count;
}

int merge(int l,int m,int r) //return number of verse_pairs
{
  //count 
  int count = 0;
  int i = l;
  int j = m+1;
  int k = l;//the begin of help array

  while(i<=m && j<=r)
  {
    if(nums[i] > 2 * ((long)nums[j]))
    {
      ++j;
      count += m-i+1;
    }
    else
    {
      ++i;
    }
  }

  int a=l;
  int b=m+1;
  //merge sort
  while(a<=m && b<=r)
  {
    help[k++] = nums[a]<nums[b] ? nums[a] : nums[b];
  } 
  while(a<=m)
  {
    help[k++] = nums[a++];
  }
  while(b<=r)
  {
    help[k++] = nums[b++];
  }

  for(int w=l;w<=r;++w)
  {
    nums[w] = help[w];  
  }

  return count;
}


int reversePairs(int l,int r)
{
  if(l == r) {return 0;}//forget it!

  int m = (l+r)/2;

  return reversePairs(l,m) + reversePairs(m+1,r) + merge(l,m,r);
}

int main()
{
  
}

