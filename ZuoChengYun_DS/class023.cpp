#include<iostream>
#include<random>

using namespace std;

static int arr[100000];



std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<int> arr_val(1,1000000000);
std::uniform_int_distribution<int> N_val(1,100000); //[1,100000]
//std::uniform_int_distribution<int> range(l,r);


void swap(int& x,int& y)///////error_1
{
  int temp = x;
  x = y;
  y = temp;
}

void partition(int l,int r,int x,int& a,int& b)
{
  int i = l;

  while(b>=i)
  {
    if(arr[i]<x) {
      swap(arr[i],arr[a]);//error_2: swap(arr[i],x);
      i++;
      a++;  
    }else if(arr[i] == x) {
      i++;
    }else {
      swap(arr[i],arr[b]);
      b--;
    }
  }
}

void quick(int l,int r)
{
  if(l>=r) {return;}

  std::uniform_int_distribution<int> range(l,r);
  int x = arr[range(rng)];//error_3: int x = arr[l + range(rng)*(r-l)]; x only can be arr[l] or arr[r]
  int a = l;
  int b = r;

  partition(l,r,x,a,b);

  quick(l,a-1);
  quick(b+1,r);
}

int main()
{
  int n;
  cin >> n;

  for(int i = 0;i<n;++i)
  {
    cin >> arr[i];
  }

  quick(0,n-1);

  for(int i = 0;i<n;++i)
  {
    cout << arr[i] << " ";
  }
}

