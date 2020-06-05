#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(void)
{
    // Ask user for input
    string text = get_string("Text: ");

    int letterCount = 0;
    int wordCount = 1;
    int sentenceCount = 0;

    // Loop through each character of input text
    for (int i = 0; i < strlen(text) + 1; i++)
    {
        // If character is upper/lower A-Z, increase letter count variable
        if (text[i] >= 65 && text[i] <= 122)
        {
            letterCount++;
        }

        // If character is "SPACE", increase word count variable
        if (text[i] == 32)
        {
            wordCount++;
        }

        // If character is "!", ".", or "?", increase sentence count variable
        if (text[i] == 33 || text[i] == 46 || text[i] == 63)
        {
            sentenceCount++;
        }
    };

    // Edge case for if there is no letters
    if (letterCount == 0)
    {
        wordCount--;
    }

    // Average num of letters/sentences per 100 words
    float L = (float)letterCount / wordCount * 100;
    float S = (float)sentenceCount / wordCount * 100;

    // Algorithm to find reading grade-level
    float index = 0.0588 * L - 0.296 * S - 15.8;

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %.0f\n", index);
    }
}
