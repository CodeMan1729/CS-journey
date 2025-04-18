#include<stdio.h>
#include<cs50.h>

void check_16(char* digits)
{
    int digit;
    int sum_1 = 0;
    int sum_2 = 0;

    for(int i = 14;i>=0;i-=2)//从second-to-last位间隔往前
    {
        digit = 2 * (digits[i]-'0');
        sum_1 = sum_1 + digit%10 + digit/10;//加上各位和十位
    }

    for(int i = 15;i>=0;i-=2)//其他位置
    {
        sum_2 += (digits[i] - '0');
    }

    //printf("sum_1=%d,sum_2=%d\n",sum_1,sum_2);

    if((sum_1+sum_2)%10 == 0)//pass test!
    {
        if(digits[0] == '4')
        {
            printf("VISA\n");
        }
        else if(digits[0]=='5' && (digits[1]=='1' || digits[1]=='2' || digits[1]=='3' ||
                                   digits[1]=='4' || digits[1]=='5'))
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

void check_15(char* digits)
{
    int digit;
    int sum_1 = 0;
    int sum_2 = 0;

    for(int i = 13;i>=0;i-=2) //从second-to-last位间隔往前
    {
        digit = 2 * (digits[i]-'0');
        sum_1 = sum_1 + digit%10 + digit/10;//加上各位和十位
    }

    for(int i = 14;i>=0;i-=2)//其他位置
    {
        sum_2 += (digits[i] - '0');
    }

    //printf("sum_1=%d,sum_2=%d\n",sum_1,sum_2);

    if((sum_1+sum_2)%10 == 0)//pass test!
    {
        if(digits[0]=='3' && (digits[1]=='4' || digits[1]=='7'))
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

void check_13(char* digits)
{
    int digit;
    int sum_1 = 0;
    int sum_2 = 0;

    for(int i = 11;i>=0;i-=2)//从second-to-last位间隔往前
    {
        digit = 2 * (digits[i]-'0');
        sum_1 = sum_1 + digit%10 + digit/10;//加上各位和十位
    }

    for(int i = 12;i>=0;i-=2)//其他位置
    {
        sum_2 += (digits[i] - '0');
    }

    //printf("sum_1=%d,sum_2=%d\n",sum_1,sum_2);

    if((sum_1+sum_2)%10 == 0)//pass test!
    {
        if(digits[0] == '4')
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}


void luhn_algorithm(char* digits)//先判断卡的位数+前缀，然后再判断是不是valid
{
    if(digits[16] == '\0'  && digits[15] != '\0')
    {   //16 digits.Maybe is mastercard or visa
        check_16(digits);
    }
    else
    {
        if(digits[15] == '\0' && digits[14] != '\0')
        {   // maybe is american express
            check_15(digits);
        }
        else
        {
            if(digits[13] == '\0' && digits[12] != '\0')
            {   //maybe is visa
                check_13(digits);
            }
            else
            {
                printf("INVALID\n");
            }

        }
    }
}


int main()
{
    char* card_num = get_string("Number:");

    luhn_algorithm(card_num);
}
