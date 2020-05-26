#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    // Define variable "change" as a float
    float change;

    // Get input from user for amount of change owed
    do
    {
        change = get_float("Change owed: ");
    }
    while (change < 0);

    // Define cents
    int centsOwed = round(change * 100);

    // subtract quarters from cent total
    int quartersOwed = floor(centsOwed / 25);
    centsOwed -= quartersOwed * 25;

    // subtract dimes from cent total
    int dimesOwed = floor(centsOwed / 10);
    centsOwed -= dimesOwed * 10;

    // subtract nickels from cent total
    int nickOwed = floor(centsOwed / 5);
    centsOwed -= nickOwed * 5;

    // Add together total of each type of coin
    int coinsTotal = centsOwed + nickOwed + dimesOwed + quartersOwed;

    // Print answer and finish program
    printf("%i\n", coinsTotal);
    return 0;

}