#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Asks for a string and prints it
    string name = get_string("What is your name?\n");
    printf("Hello, %s!\n", name);
    return 0;
}