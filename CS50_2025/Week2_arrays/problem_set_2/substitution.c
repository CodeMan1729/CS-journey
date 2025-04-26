#include <cs50.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<ctype.h>

void case_insensitive(int count, string val[]);
void substitute(string origin, string key);
void check_duplicate(string key); // test key's duplicate conditions

int main(int argc, string argv[])
{
        case_insensitive(argc, argv);

        string plaintext = get_string("plaintext: ");
        substitute(plaintext, argv[1]);

        return 0;// valid conditions
}

void case_insensitive(int count, string val[])// return 1 is pass, -1 is error
{

    if (count != 2)
    {
        printf("Usage: ./substitution key\n");
        exit(1);
    }

    //1. 要是字母   2. 要26个
    int length = strlen(val[1]);

    for(int i = 0; i < length; i++)
    {
        if (val[1][i] < 'A' || (val[1][i] > 'Z' && val[1][i] < 'a') || val[1][i] > 'z') // 存在非字母
        {
            printf("Key must contain 26 characters.\n");
            exit(1);
        }
    }

    if(length != 26)
    {
        printf("Key must contain 26 characters.\n");
        exit(1);
    }

    check_duplicate(val[1]);//前面的if已经保证是26个字母，且都是字母
}

void substitute(string origin, string key)//punctuation isn't changed
{
    int length = strlen(origin);

    for(int i = 0; i < length; i++)
    {
        int index;
        if( isupper(origin[i]) )
        {
            index = origin[i] - 'A';
            origin[i] = toupper(key[index]);
        }

        if( islower(origin[i]) )
        {
            index = origin[i] - 'a';
            origin[i] = tolower(key[index]);
        }
    }

    printf("ciphertext: %s\n", origin);
}

void check_duplicate(string key) // key is a string
{
    bool check[26] = {false};
    int index;

    for(int i = 0; i < 26; i++)
    {
        if( isupper(key[i]) )
        {
            index = key[i] - 'A';
            if(check[index] != true)
            {
                check[index] = true;
            }
        }

        if( islower(key[i]) )
        {
            index = key[i] - 'a';
            if(check[index] != true)
            {
                check[index] = true;
            }
        }
    }

    for(int i = 0; i < 26; i++)
    {
        if(check[i] != true)
        {
            exit(1); // 26个中有没出现的字母
        }
    }
}
