#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

void scrabble(string s1, string s2, int* alphabet, char* letter);
int is_what_letter(char c, char* letter);

int main()
{
    int alphabet[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1,
                        8, 5, 1, 3, 1, 1, 3, 10, 1,
                        1, 1, 1, 4, 4, 8, 4, 10};
    char letter[26] = {'A', 'B', 'C', 'D', 'E', 'F',
                        'G', 'H', 'I', 'J', 'K', 'L',
                        'M', 'N', 'O', 'P', 'Q', 'R',
                        'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    string play_1 = get_string("Player 1: ");
    string play_2 = get_string("Player 2: ");

    scrabble(play_1, play_2, alphabet, letter);
}

int is_what_letter(char c, char* letter)// true letter[] index, if not a letter, return -1
{
    char C = toupper(c);
    int i = 0;

    for( ; i < 26; ++i)
    {
        if(letter[i] - C == 0)
        {
            return i;
        }
    }
    return -1;
}

void scrabble(string s1, string s2, int* alphabet, char* letter)
{
    int sum_1 = 0;
    int sum_2 = 0;
    int val;

    for(int i =0; i < strlen(s1);++i)//sum s1
    {
        val = is_what_letter(s1[i], letter);
        if(val != -1)//is a letter
        {
            sum_1 += alphabet[val];
        }
    }

    for(int i =0; i < strlen(s2);++i)//sum s2
    {
        val = is_what_letter(s2[i], letter);
        if(val != -1)//is a letter
        {
            sum_2 += alphabet[val];
        }
    }

    if(sum_1 > sum_2)
    {
        printf("Player 1 wins!\n");
    }
    else if(sum_1 < sum_2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Ties\n");
    }
}

