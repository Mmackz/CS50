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

    char hash = '#';
    char space = ' ';

    // Build pryamids
    for (int i = 0; i < height; i++)
    {
        // Add spaces to pyramid
        for (int j = height; j > i + 1; j--)
        {
            printf("%c", space);
        }

        // Add hashes to pyramid
        for (int k = 0; k < i + 1; k++)
        {
            printf("%c", hash);
        }

        // Print new line at end of each loop
        printf("\n");
    }

    // Program finish
    return 0;

}