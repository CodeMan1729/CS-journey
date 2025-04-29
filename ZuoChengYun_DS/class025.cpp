#include<iostream>
#include<random>

using namespace std;

random_device dev;
mt19937 gen(dev());
uniform_int_distribution<int> dist_N(0,100000);
uniform_int_distribution<int> dist_val(1,1000000000);

int N;
static int arr[100000];

void swap(int& a,int& b)//space complexity O(1)
{
  int temp = a;
  a = b;
  b = temp;
}

void adjust_up(int i)//index i to upward adjust
{
  while(i>=0 && arr[i]>arr[(i-1)/2]){
    swap(arr[i],arr[(i-1)/2])
    i = (i-1)/2;
  }
}

void adjust_down(int* arr,int i,int size)// i is a index of array
{
  int largest;
  while(2*i+1 < size)
  {
    largest = 2*i+2<size && arr[2*i+1]<arr[2*i+2] ?  2*i+2 : 2*i+1;
    if(arr[largest] > arr[i]){
      swap(arr[largest],arr[i]);
      i = largest;
     // adjust_down(arr,i,size);
    }else{
      break;
    }
  }
}


void heap_sort_down(int* arr,int N)//O(nlogn)
{
  int j = 0;
  while(j<N){//O(nlogn)
    adjust_up(j);
    j++;
  }
  
  int size = N;
  while(size>1){
    swap(arr[0],arr[size-1]);
    size--;
    adjust_down(arr,0,size);
  }
}

void heap_sort_up(int* arr,int N)//O(nlogn)
{
  for(int i = (N-2)/2;i>=0;--i)// O(n)            only create a big root heap once
  {
    adjust_down(arr,i,N);//O(logn)
  }

  int size = N;
  while(size>1){//O(nlogn)
    swap(arr[0],arr[size-1]);
    --size;
    adjust_down(arr,0,size);//O(logn)
  }
}

int main()
{
  int N;
  cin >> N;

  for(int i = 0;i<N;++i)
  {
    cin >> arr[i];
  }

  heap_sort_down(arr,N);

  for(int i = 0;i<N;++i)
  {
    cout << arr[i] << " ";
  }
}

