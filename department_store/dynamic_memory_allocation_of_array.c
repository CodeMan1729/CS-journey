// Online C compiler to run C program online
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

// 增加动态数组的长度
// time complexity:O(n)

#define InitSize 10;

struct SqList
{
    int *data;
    size_t length;
    size_t MaxSize;
};

void InitList(struct SqList *s)
{
    s->length = 0;
    s->MaxSize = InitSize;
    s->data = (int *)malloc(sizeof(int) * s->MaxSize);
}

void increaseLen(struct SqList*s, size_t len)
{
    int *tmp = s->data;
    s->MaxSize += len;
    s->data = (int *)malloc(sizeof(int) * s->MaxSize);
    
    for(int i = 0; i < s->length; i++)
    {
        s->data[i] = tmp[i];
    }
    free(tmp);
}

int main() 
{
    struct SqList s;
    InitList(&s);
    increaseLen(&s, 100);
    
    for(int i = 0; i < s.MaxSize; i++)
    {
        s.data[i] = i;
        s.length++;
    }
    
    printf("%zu\n", s.length);
    
    for(int i = 0; i < s.length; i++)
    {
        printf("%i ", s.data[i]);
    }

    
    return 0;
}
