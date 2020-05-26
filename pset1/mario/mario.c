#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Define height as an integer
    int height;

    // Prompt for input until user inputs integer between 1-8
    do
    {
        height = get_int("Height: ");
    }
    while (height <= 0 || height > 8);

    // Build pryamids

    // Define characters used to build pyramids

    char hash = '#';
    char space = ' ';

    // Outer loop to add new line
    for (int i = 0; i < height; i++)
    {
        // Add spaces to left pyramid
        for (int j = height; j > i + 1; j--)
        {
            printf("%c", space);
        }

        // Add hashes to left pyramid
        for (int k = 0; k < i + 1; k++)
        {
            printf("%c", hash);
        }

        // Add 2 spaces between pryamids
        printf("%c%c", space, space);

        // Build the right pryamid
        for (int p = 0; p < i + 1; p++)
        {
            printf("%c", hash);
        }

        // Print new line at end up each loop iteration
        printf("\n");
    }

    // Finish program
    return 0;

}