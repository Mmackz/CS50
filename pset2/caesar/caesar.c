#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // Define error message for invalid key
    string error = "Usage: ./caesar key";

    // Check if only one key provided or first character is < 0 or > 9
    if (argc != 2 || (int)argv[1][0] < 48 || (int)argv[1][0] > 57)
    {
        printf("The key must only be a positive number\nYou must enter the key in the following format:\n");
        printf("%s\n", error);
        return 1;
    }

    // Check if every character of key is a digit
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (isalpha(argv[1][i]))
        {
            printf("Non-digit character found in key\nYou must enter the key in the following format:\n");
            printf("%s\n", error);
            return 1;
        }
    }

    // Convert key string into integer
    int key = atoi(argv[1]);

    // Check if key has not overflowed
    if (key < 0)
    {
        printf("Key value has overflowed\nEnter integer between 0 - 2147483647\n");
        printf("%s\n", error);
        return 1;
    }

    // Ask user to enter plaintext for ciphering
    string ptext = get_string("plaintext: ");
    // Lower value of key to size of alphabit
    key = key % 26;

    if (!key)
    {
        printf("ciphertext: %s\n", ptext);
        printf("ERROR!!  This key is unable to encrypt the text. Use a different key.\n");
        printf("%s\n", error);
        return 1;
    }

    // Loop the user-entered string "ptext" and change values with key
    for (int i = 0; i < strlen(ptext); i++)
    {
        // If letter is uppercase
        if (ptext[i] < 91 && ptext[i] > 64)
        {
            int uval = (((ptext[i] - 65) + key) % 26) + 65;
            ptext[i] = (char)uval;
        }
        // Else if letter is lowercase
        else if (ptext[i] < 123 && ptext[i] > 96)
        {
            int lval = (((ptext[i] - 97) + key) % 26) + 97;
            ptext[i] = (char)lval;
        }
    }

    // Print encrypted cipher
    printf("ciphertext: %s\n", ptext);

    // Program finished successful!!
    return 0;

}