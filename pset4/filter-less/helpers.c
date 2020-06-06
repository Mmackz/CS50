#include <stdio.h>
#include <math.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through each pixel in 2D image pixel array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // round the result of amount from dividing the sum of R G and B by 3
            int average = round(((float)image[i][j].rgbtRed + (float)image[i][j].rgbtBlue + (float)image[i][j].rgbtGreen) / 3);

            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Checks value for overflow (255 for 8-bit unsigned char) *part of sepia function
void overflow_check(int *value)
{
    if (*value > 255)
    {
        *value = 255;
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int sepiaRed, sepiaGreen, sepiaBlue;

    // Loop through each pixel in 2D image pixel array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sepiaRed = round(image[i][j].rgbtRed * 0.393 + image[i][j].rgbtGreen * 0.769 + image[i][j].rgbtBlue * 0.189);
            sepiaGreen = round(image[i][j].rgbtRed * 0.349 + image[i][j].rgbtGreen * 0.686 + image[i][j].rgbtBlue * 0.168);
            sepiaBlue = round(image[i][j].rgbtRed * 0.272 + image[i][j].rgbtGreen * 0.534 + image[i][j].rgbtBlue * 0.131);

            overflow_check(&sepiaRed);
            overflow_check(&sepiaGreen);
            overflow_check(&sepiaBlue);

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Originally I copied the array to another array with each row reversed and copied it back,
    // while this worked it was not efficient (I think?) so I changed it to swap the first and last values,
    // like 0 and width - 1, then 1 and width - 2 and so on until they meet in the middle


    // RGBTRIPLE copy[height][width]; DONT NEED ANYMORE


    // Swap pixels in each row
    for (int i = 0; i < height; i++)
    {
        // Identify the initial pixels to swap
        int k = width - 1;
        int j = 0;

        while (k > j)
        {

            // Define temp variable to store value at image[i][j]
            unsigned char temp;

            // Swap image[i][j] with image[i][k]

            temp = image[i][j].rgbtRed;
            image[i][j].rgbtRed = image[i][k].rgbtRed;
            image[i][k].rgbtRed = temp;

            temp = image[i][j].rgbtGreen;
            image[i][j].rgbtGreen = image[i][k].rgbtGreen;
            image[i][k].rgbtGreen = temp;

            temp = image[i][j].rgbtBlue;
            image[i][j].rgbtBlue = image[i][k].rgbtBlue;
            image[i][k].rgbtBlue = temp;

            j++;
            k--;
        }
    }

    // // Re-write original image with copy REMOVE
    // for (int i = 0; i < height; i++)
    // {
    //     for (int j = 0; j < width; j++)
    //     {
    //         image[i][j].rgbtRed = copy[i][j].rgbtRed;
    //         image[i][j].rgbtGreen = copy[i][j].rgbtGreen;
    //         image[i][j].rgbtBlue = copy[i][j].rgbtBlue;
    //     }
    // }


    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Define structure to keep  the totals of each color which will be used to work out the averages
    typedef struct
    {
        int red;
        int green;
        int blue;
    }
    COLOR_TOTALS;

    COLOR_TOTALS totals[height][width];

    // Copy image and use it to calculate values from. Original image values will change during the process and alter
    // values needed for calculations if no copy is used

    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Copy image values to copy array and initialize totals array with 0's
            copy[i][j] = image[i][j];
            totals[i][j].red = 0;
            totals[i][j].green = 0;
            totals[i][j].blue = 0;
        }
    }

    for (int h = 0; h < height; h++)
    {
        // Outer loops h and w loop through each pixel of the image
        for (int w = 0; w < width; w++)
        {
            // Squares counts the number of pixels used in calculation.
            int squares = 0;

            // Inner loops i and j finds the 3*3 box surrounding the pixel to and upgrades the totals array
            for (int i = h - 1; i <= h + 1 && i < height; i++)
            {
                for (int j = w - 1; j <= w + 1 && j < width; j++)
                {
                    if (i < 0)
                    {
                        i = 0;
                    }
                    if (j < 0)
                    {
                        j = 0;
                    }
                    totals[h][w].red += copy[i][j].rgbtRed;
                    totals[h][w].green += copy[i][j].rgbtGreen;
                    totals[h][w].blue += copy[i][j].rgbtBlue;
                    squares++;
                }
            }

            // Finds averages for each pixels color
            int avg_red = round((float)totals[h][w].red / squares);
            int avg_green = round((float)totals[h][w].green / squares);
            int avg_blue = round((float)totals[h][w].blue / squares);

            // Overwrites pixels new values to image
            image[h][w].rgbtRed = avg_red;
            image[h][w].rgbtGreen = avg_green;
            image[h][w].rgbtBlue = avg_blue;
        }
    }
    return;
}
