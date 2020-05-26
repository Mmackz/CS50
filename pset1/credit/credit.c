#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(void)
{
    // Define variables
    char cardArray[20];
    long ccNum;
    int length;
    char test[20];

    // Prompt user for CC number
    do
    {
        ccNum = get_long("Number: ");
        sprintf(cardArray, "%li", ccNum);
        sprintf(test, "%li", ccNum);
        length = strlen(cardArray);
    }
    while (length < 4);

    // Double every second positon from end and sum digits if over 10
    if (length % 2 == 0)
    {
        for (int i = 0; i < length; i += 2)
        {
            // Using ascii numbers so subtract 48
            cardArray[i] = (cardArray[i] - 48) * 2 + 48;
            if (cardArray[i] > 57)
            {
                cardArray[i] -= 9;
            }
        }
    }
    else // length is odd
    {
        for (int i = 1; i < length; i += 2)
        {
            cardArray[i] = (cardArray[i] - 48) * 2 + 48;
            if (cardArray[i] > 57)
            {
                cardArray[i] -= 9;
            }
        }
    }

    // Find sum of all numbers
    int sum = 0;
    for (int j = 0; j < length; j++)
    {
        sum += cardArray[j] - 48;
    }

    // Print message if card is valid
    if (sum % 10 == 0)
    {
        if (strlen(test) == 16 && test[0] == '5' && test[1] < 54 && test[1 > 48])
        {
            printf("MASTERCARD\n");
            return 0;
        }
        else if (strlen(test) >= 13 && strlen(test) <= 16 && test[0] == '3' && (test[1] == 52 || test[1] == 55))
        {
            printf("AMEX\n");
            return 0;
        }
        else if ((strlen(test) == 13 || strlen(test) == 16) && test[0] == '4')
        {
            printf("VISA\n");
            return 0;
        }
        else // If card passes no previous tests return invalid
        {
            printf("INVALID\n");
            return 0;
        }
    }
    else // Print Invalid if card is not valid
    {
        printf("INVALID\n");
        return 0;
    }

}