// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h> // 包含strcasecmp函数
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1]; // 添加\0
    struct node *next;
} node;

const unsigned int N = 1024 * 1024 * 8 * 10; // 10MB

// 单词中的数量
unsigned int numbers_of_dictionary = 0;

// Hash table
node *table[83886080]; // N

// Returns true if word is in dictionary, else false
// 检测单词对应的hash值在不在table里面 dskfsdlfds
bool check(const char *word)
{

    int hash_val = hash(word);

    // 检测是否为词典以外拼写的拼写方式的单词
    if(table[hash_val] == NULL) // table里面是指向结构体的指针
    {
        return false;
    }

    // 寻找匹配的单词, 遍历查找
    node *n = table[hash_val];

    // 忽略大小写比较， Apple, apple算一个单词
    while(n != NULL)
    {
        if(strcasecmp(word, n->word) == 0)
        {
            return true;
        }
        n = n->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int hash_val = 0;
    int i = 0;
    int word_length = 1; // 单词最少是一个

    // 计算单步的hash值(不同大小写的单词hash值相同)
    while(word[i] != '\0')
    {
        hash_val += toupper(word[i++]);
        word_length++;
    }

    // 计算总的hash
    hash_val += word_length;

    return hash_val;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // 打开文件
    FILE *dictionary_name = fopen(dictionary, "r");

    // 检查是否成功打开
    if (dictionary_name == NULL)
    {
        return false;
    }

    // 哈希表清空
    memset(table, 0, N);

    // 把字典的单词哈希值化，然后写入哈希表
    char tmp_word[LENGTH + 1];
    int hash_val = 0;

    while(fscanf(dictionary_name, "%45s", tmp_word) != EOF)
    {
        tmp_word[strcspn(tmp_word, "\n")] = '\0';

        // 计算哈希值
        hash_val = hash(tmp_word); // fscanf会去掉\n

        // 创建新节点
        node *n = malloc(sizeof(node));
        if(n == NULL)
        {
            fclose(dictionary_name);
            return false;
        }

        // 复制单词到节点
        strncpy(n->word, tmp_word, LENGTH + 1); // 当src的长度小于n时，dest的剩余部分将用空字节填充。
        n->next = NULL;

        // 插入到哈希表
        if(table[hash_val] == 0)
        {
            table[hash_val] = n;
        }
        else
        {
            n->next = table[hash_val];
            table[hash_val] = n;
        }

        numbers_of_dictionary++;
    }

    // 关闭文件
    fclose(dictionary_name);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
// 返回字典中单词的数量，0是没有加载成功
unsigned int size(void)
{
    return numbers_of_dictionary;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *cur_node;
    node *next_node;

    // 遍历清除每一个table[hash_val]的链表
    for(int hash_val = 0; hash_val < N; hash_val++)
    {
        if(table[hash_val] == 0) // 无需清理
        {
            continue;
        }

        cur_node = table[hash_val];
        while(cur_node != NULL)
        {
            next_node = cur_node->next;
            free(cur_node);
            cur_node = next_node;
        }
    }

    return true;
}
