#include<iostream>

// O(n)
template<typename T>
  T getMax(T* arr,T l,T r)//arr[r,l]
  {
    T temp = l;
    for(int i = l;i<=r;++i)
    {
      if(temp < arr[i])
      {
	temp = arr[i];
      }
    }
    return temp;
  }

//O(n)
int getMax(int* arr,int l,int r)
{
  if(l == r) { return arr[l]; }
  m = (l+r)/2;
  int lmax = getMax(arr,l,m);
  int rmax = getMax(arr,m+1,r);

  if(lmax > rmax){return lmax;}
  else {return rmax;}
}

int main()
{
  
}

