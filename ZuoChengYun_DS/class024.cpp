#include<iostream>
#include<random>

using namespace std;

random_device dev;
mt19937 gen(dev());
uniform_int_distribution<int> dist_nums(-10000,10000);
uniform_int_distribution<int> dist_N(1,100000);

static int nums[100000];

int ans;

void swap(int& a,int& b){
    int temp = a;
    a = b;
    b = temp;
}

void sort(int* nums,int l,int r,int k){
    uniform_int_distribution<int> dist_x(l,r);
    int x = nums[dist_x(gen)];
    
    int a = l;
    int b = r;
    int i = l;
    
    while(i<=b){
        if(nums[i]<x){
            swap(nums[i++],nums[a++]);
        }
        else if(nums[i]>x){
            swap(nums[i],nums[b--]);    
        }
        else{
            ++i;    
        }
    }
    
    if(k<a){
        sort(nums,l,a-1,k);    
    }
    else if(k>b){
        sort(nums,b+1,r,k);    
    }
    else{
        ans = nums[a];    
    }
}

int main(){
    
    for(int i = 0;i<10;++i)//10 experiment
    {
        int N = dist_N(gen);//length of nums
        for(int j = 0;j<N;++j)
        {
            nums[j] = dist_nums(gen);
        }
        uniform_int_distribution<int> dist_K(1,N);
        int K = dist_K(gen);
        
        cout << "原始数组：";
        for(int j = 0;j<N;++j)//print before
        {
            cout << nums[j] << " ";
        }
        cout << '\n' << "K=" << K << '\n';
        
        sort(nums,0,N-1,N-K);//第k大＝nums[length-k]
        
        cout << "第" << K << "大的是" << ans << '\n';
    }
}
