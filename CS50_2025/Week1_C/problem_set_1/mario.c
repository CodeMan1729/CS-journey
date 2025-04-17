#include<stdio.h>
#include<cs50.h>

void left(int i, int n)
{
    for(int j = 0;j < n-i;++j)//打印左边空格
    {
        printf(" ");
    }
    for(int j = 0;j < i;++j)
    {
        printf("#");
    }
}

void right(int i)
{
    for(int j = 0;j < i;++j)
    {
        printf("#");
    }
}

int main()
{
    int h = 0;
    while(h <= 0)
    {
        h = get_int("Height: ");
    }

    for(int i = 1;i < h+1;++i)// rows
    {
        left(i, h);
        printf("  ");
        right(i);
        printf("\n");
    }
}
