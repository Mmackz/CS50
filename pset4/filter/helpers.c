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

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
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

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Define structure to keep the totals of each color which will be used to work out the averages
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

// Function to check for overflowed values using pointers *part of edges

void overflow_check(int *value)
{
    if (*value > 255)
    {
        *value = 255;
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Define 3x3 arrays for gx and gy
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};


    // Define structure to keep the 3x3 array of each color which will be used to work out the new values
    typedef struct
    {
        int red;
        int green;
        int blue;
    }
    PIXEL_ARRAY;

    PIXEL_ARRAY pixel[3][3];
    PIXEL_ARRAY Gx_val;
    PIXEL_ARRAY Gy_val;
    PIXEL_ARRAY new_val;


    // Copy image and use it to calculate values from. Original image values will change during the process and alter
    // values needed for calculations if no copy is used
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }


    for (int h = 0; h < height; h++)
    {

        // Outer loops h and w loop through each pixel of the image
        for (int w = 0; w < width; w++)
        {
            // Y value for filling 3x3 array
            int Y = 0;

            // Initialize all Gx and Gy values to zero
            Gx_val.red = 0;
            Gy_val.red = 0;
            Gx_val.green = 0;
            Gy_val.green = 0;
            Gx_val.blue = 0;
            Gy_val.blue = 0;


            // Inner loops i and j finds the 3*3 box surrounding the pixel  and upgrades the pixel array
            for (int i = h - 1; i <= h + 1; i++)
            {
                // X value for filling 3x3 array
                int X = 0;

                for (int j = w - 1; j <= w + 1; j++)
                {
                    // Conditions for pixels in edges/corners
                    if (i < 0 || j < 0 || i >= height || j >= width)
                    {
                        pixel[Y][X].red = 0;
                        pixel[Y][X].green = 0;
                        pixel[Y][X].blue = 0;
                        X++;
                        continue;
                    }

                    // If no edge conditions found, fill location [Y][X] in pixel array with values from copied image
                    pixel[Y][X].red = copy[i][j].rgbtRed;
                    pixel[Y][X].green = copy[i][j].rgbtGreen;
                    pixel[Y][X].blue = copy[i][j].rgbtBlue;

                    // Increase X by 1 after every loop of j
                    X++;
                }

                // Increase Y by 1 after every loop of i
                Y++;
            }

            // 3x3 arrays now set, calculate Gx_val and Gy_val using the arrays
            for (int a = 0; a < 3; a++)
            {
                for (int b = 0; b < 3; b++)
                {
                    Gx_val.red += Gx[a][b] * pixel[a][b].red;
                    Gx_val.green += Gx[a][b] * pixel[a][b].green;
                    Gx_val.blue += Gx[a][b] * pixel[a][b].blue;

                    Gy_val.red += Gy[a][b] * pixel[a][b].red;
                    Gy_val.green += Gy[a][b] * pixel[a][b].green;
                    Gy_val.blue += Gy[a][b] * pixel[a][b].blue;
                }
            }

            // Calculate new values by applying Sobel operator
            new_val.red = round(sqrt(pow(Gx_val.red, 2) + pow(Gy_val.red, 2)));
            new_val.green = round(sqrt(pow(Gx_val.green, 2) + pow(Gy_val.green, 2)));
            new_val.blue = round(sqrt(pow(Gx_val.blue, 2) + pow(Gy_val.blue, 2)));

            // Check for overflows
            overflow_check(&new_val.red);
            overflow_check(&new_val.green);
            overflow_check(&new_val.blue);

            // Apply new values to image
            image[h][w].rgbtRed = new_val.red;
            image[h][w].rgbtGreen = new_val.green;
            image[h][w].rgbtBlue = new_val.blue;

        }
    }

    return;
}