#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

void readability(string text);

int main()
{
    string text = get_string("Text: ");

    readability(text);
}

void readability(string text)//句子就是末尾有. ? ! ; ---->  sentence++
{
    int sentences_n = 0, letters_n = 0;
    int words_n = 1;
    double L, S, index;
    size_t length = strlen(text);

    for(int i = 0; i < length; i++)
    {
        if(text[i]>='A' && text[i]<='z')//is a letters
        {
            letters_n++;
        }

        if(text[i] == '!' || text[i] == '?')//get sentences' number
        {
            sentences_n++;
        }

        if(text[i] == '.' && text[i+1] != '.')
        {
            sentences_n++;
        }

        if(text[i] == '.' && text[i+1] == '.')
        {
            continue;
        }

        if(text[i] == ' ' && text[i+1] != ' ')
        {
            words_n++;//对空格后面的单词计数
        }

        if(text[i] == ' ' && text[i+1] == ' ')
        {
            continue;
        }
    }

    //calculate index
    L = ((double) letters_n / words_n) * 100;
    S = ((double) sentences_n / words_n) * 100;
    index = round(0.0588 * L - 0.296 * S - 15.8);

    if(index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if(index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) index);
    }
}
