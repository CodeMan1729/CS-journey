#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // only allow one argument
    if(argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // open card.raw
    FILE* card = fopen(argv[1],"r"); // card.raw is a binary file
    if(card == NULL)
    {
        printf("Fail to open !\n");
        return 1;
    }

    // While there's still data left to read from the memory card
    uint8_t buffer[512];
    FILE *cur_jpg = NULL;
    char jpg_num[100];
    int i = 0;

    while(fread(buffer, 512, 1, card) == 1)
    {
        if(buffer[0] == 0xff && buffer[1] == 0xd8 &&
            buffer[2] == 0xff && buffer[3] >= 0xe0 && buffer[3] <= 0xef)
        {
            if(cur_jpg != NULL)
            {
                fclose(cur_jpg);
            }
            sprintf(jpg_num, "%03d.jpg", i);
            cur_jpg = fopen(jpg_num, "w");
            fwrite(buffer, 512, 1, cur_jpg);
            i++;
        }
        else
        {
            if(cur_jpg == NULL)
            {
                continue;
            }
            if(fwrite(buffer, 512, 1, cur_jpg) != 1)
            {
                printf("fail to contiguously write\n");
                fclose(card);
                return 1;
            }
        }
    }

    if(cur_jpg != NULL)
    {
        fclose(cur_jpg);
    }
    fclose(card);

    return 0;
}

