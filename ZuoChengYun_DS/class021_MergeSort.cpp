#include<iostream>

static int help[1000];
static int arr[1000];//origin array

void merge(int l,int m,int r)
{
  int a = l;
  int b = m+1;
  int i = l;//the index of help array
  
  while(a<=m && b<=r)
  {
    help[i++] = arr[a++]<arr[b++] ? arr[a] : arr[b]; 
  }
  while(a<=m)
  {
    help[i++] = arr[a++];
  }
  while(b<=m)
  {
    help[i++] = arr[b++];
  }

  for(i=l;i<=r;i++)
  {
    arr[i] = help[i];
  }
}

//recursion version
void mergeSort_recursion(int l,int r)
{
  if(l == r){ return;}
  m = (l+r)/2;
  mergeSort_recursion(l,m);
  mergeSort_recursion(m+1,r);
  merge(l,m,r);
}

//non-recursion algorithm
void mergeSort(int l,int r,int n)
{
  int m = 0;
  for(int step = 1;step<n;step <<= 1)
  {
    l = 0;
    while(l<n)
    {
      m = l+step-1;
      if(m+1 >= n) {break;}// haven't right side, enter into next loop
      r = (l+(step<<1)-1) < n-1 ? (l+(step<<1)-1) : n-1;
      merge(l,m,r);
      l = r+1;
    }
  }
}

int main()
{
  
}

