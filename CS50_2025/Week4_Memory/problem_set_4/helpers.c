#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE average = 0;
    float temp = 0.0;

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            temp = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;
            average = (BYTE) round(temp);

            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;

            // average = 0; no need, because I will assign the value in line 13
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_backup[height][width];

    for(int i = 0; i < height; i++)// coye the origin image
    {
        for(int j = 0; j < width; j++)
        {
            image_backup[i][j] = image[i][j];
        }
    }

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width ; j++)
        {
            image[i][j] = image_backup[i][width - 1 - j];
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    DWORD average_b = 0;
    DWORD average_g = 0;
    DWORD average_r = 0;
    int count = 0;

    RGBTRIPLE image_backup[height][width];

    for(int i = 0; i < height; i++)// copy the origin image
    {
        for(int j = 0; j < width; j++)
        {
            image_backup[i][j] = image[i][j];
        }
    }

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width ; j++)
        {
            for(int h = i - 1; h <= i + 1; h++) // cheack around valid pixel
            {
                for(int w = j - 1; w <= j + 1; w++)
                {
                    if(h >=0 && h < height && w >=0 && w < width)
                    {
                        average_b += image_backup[h][w].rgbtBlue;
                        average_g += image_backup[h][w].rgbtGreen;
                        average_r += image_backup[h][w].rgbtRed;
                        count++;
                    }
                }
            }

            image[i][j].rgbtBlue = round(average_b / (float) count);
            image[i][j].rgbtGreen = round(average_g / (float) count);
            image[i][j].rgbtRed = round(average_r / (float) count);

            average_b = 0;
            average_g = 0;
            average_r = 0;
            count = 0;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_backup[height][width];
    int Gx_r = 0;
    int Gy_r = 0;
    int Gx_b = 0;
    int Gy_b = 0;
    int Gx_g = 0;
    int Gy_g = 0;


    for(int i = 0; i < height; i++)// copy the origin image
    {
        for(int j = 0; j < width; j++)
        {
            image_backup[i][j] = image[i][j];
        }
    }

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++) // 计算(i,j)位置的pixel的三原色的Gx, Gy
        {
            //redundent but explicit
            // if(i-1 >= 0 && i-1 < height && j-1 >=0 && j-1 < width) // 左上
            if(i-1 >= 0 && j-1 >=0) // 左上
            {
                Gx_r += -1 * (int) image_backup[i-1][j-1].rgbtRed;
                Gy_r += -1 * (int) image_backup[i-1][j-1].rgbtRed;
                Gx_b += -1 * (int) image_backup[i-1][j-1].rgbtBlue;
                Gy_b += -1 * (int) image_backup[i-1][j-1].rgbtBlue;
                Gx_g += -1 * (int) image_backup[i-1][j-1].rgbtGreen;
                Gy_g += -1 * (int) image_backup[i-1][j-1].rgbtGreen;
            }

            // if(i-1 >= 0 && i-1 < height && j >= 0 && j < width) // 上中
            if(i-1 >= 0) // 上中
            {
                Gy_r += -2 * (int) image_backup[i-1][j].rgbtRed;
                Gy_b += -2 * (int) image_backup[i-1][j].rgbtBlue;
                Gy_g += -2 * (int) image_backup[i-1][j].rgbtGreen;
            }

            // if(i-1 >= 0 && i-1 < height && j+1 >=0 && j+1 < width) // 右上
            if(i-1 >= 0 && j+1 < width) // 右上
            {
                Gx_r += (int) image_backup[i-1][j+1].rgbtRed;
                Gy_r += -1 * (int) image_backup[i-1][j+1].rgbtRed;
                Gx_b += (int) image_backup[i-1][j+1].rgbtBlue;
                Gy_b += -1 * (int) image_backup[i-1][j+1].rgbtBlue;
                Gx_g += (int) image_backup[i-1][j+1].rgbtGreen;
                Gy_g += -1 * (int) image_backup[i-1][j+1].rgbtGreen;
            }

            // if(i >= 0 && i < height && j-1 >=0 && j-1 < width) // 中左
            if(j-1 >=0) // 中左
            {
                Gx_r += -2 * (int) image_backup[i][j-1].rgbtRed;
                Gx_b += -2 * (int) image_backup[i][j-1].rgbtBlue;
                Gx_g += -2 * (int) image_backup[i][j-1].rgbtGreen;
            }

            // if(i >= 0 && i < height && j+1 >= 0 && j+1 < width) // 中右
            if(j+1 < width) // 中右
            {
                Gx_r += 2 * (int) image_backup[i][j+1].rgbtRed;
                Gx_b += 2 * (int) image_backup[i][j+1].rgbtBlue;
                Gx_g += 2 * (int) image_backup[i][j+1].rgbtGreen;
            }

            // if(i+1 >= 0 && i+1 < height && j-1 >=0 && j-1 < width) // 左下
            if(i+1 < height && j-1 >=0) // 左下
            {
                Gx_r += -1 * (int) image_backup[i+1][j-1].rgbtRed;
                Gy_r += (int) image_backup[i+1][j-1].rgbtRed;
                Gx_b += -1 * (int) image_backup[i+1][j-1].rgbtBlue;
                Gy_b += (int) image_backup[i+1][j-1].rgbtBlue;
                Gx_g += -1 * (int) image_backup[i+1][j-1].rgbtGreen;
                Gy_g += (int) image_backup[i+1][j-1].rgbtGreen;
            }

            // if(i+1 >= 0 && i+1 < height && j >= 0 && j < width) // 中下
            if(i+1 < height) // 中下
            {
                Gy_r += 2 * (int) image_backup[i+1][j].rgbtRed;
                Gy_b += 2 * (int) image_backup[i+1][j].rgbtBlue;
                Gy_g += 2 * (int) image_backup[i+1][j].rgbtGreen;
            }

            // if(i+1 >= 0 && i+1 < height && j+1 >= 0 && j+1 < width) // 右下
            if(i+1 < height && j+1 < width) // 右下
            {
                Gx_r += (int) image_backup[i+1][j+1].rgbtRed;
                Gy_r += (int) image_backup[i+1][j+1].rgbtRed;
                Gx_b += (int) image_backup[i+1][j+1].rgbtBlue;
                Gy_b += (int) image_backup[i+1][j+1].rgbtBlue;
                Gx_g += (int) image_backup[i+1][j+1].rgbtGreen;
                Gy_g += (int) image_backup[i+1][j+1].rgbtGreen;
            }

            int red = round(sqrt(Gx_r * Gx_r + Gy_r * Gy_r));
            int green = round(sqrt(Gx_g * Gx_g + Gy_g * Gy_g));
            int blue = round(sqrt(Gx_b * Gx_b + Gy_b * Gy_b));

            image[i][j].rgbtRed = red < 255 ? red : 255;
            image[i][j].rgbtBlue = blue < 255 ? blue : 255;
            image[i][j].rgbtGreen = green < 255 ? green : 255;

            Gx_r = 0;
            Gy_r = 0;
            Gx_b = 0;
            Gy_b = 0;
            Gx_g = 0;
            Gy_g = 0;

        }
    }

    return;
}
