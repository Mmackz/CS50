// Implements a dictionary's functionality
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 186019; // 1.3x size of default dictionary to reduce collisions
unsigned int word_count = 0;

// Hash table
node *table[N];
node *all_nodes = NULL;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Obtain a hash value for the word
    int index = hash(word);

    // Look up the value in the hash table by traversing the linked-list until its found or end of list(NULL)
    node *temp = table[index];

    while (temp != NULL)
    {
        // Compare each nodes word value with the word we are checking for
        if (!strcasecmp(temp->word, word))
        {
            // If found return true
            return true;
        }

        // At end of each iteration set the temp node to be the next node in the list
        temp = temp->next;
    }

    // Return false if search for word was unsuccessful (not in linked-list)
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Hash function is called string folding, what it does is takes a string, converts each char to lowercase (so it will work when compare case-insensitively)
    // splits it into 4-byte chunks, concatenates the binary values in each 4-byte chunk then finds the sum of all the chunks. It then takes the large integer value and
    // finds a suitable value for the hash table by using the modulus operator.  source: https://opendsa-server.cs.vt.edu/

    long sum = 0, factor = 1;
    int length = strlen(word);

    for (int i = 0; i < length; i++)
    {
        if (i % 4 == 0)
        {
            factor = 1;
        }
        else
        {
            factor <<= 8;
        }

        sum += tolower(word[i]) * factor;
    }
    return (unsigned int)(sum % N);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary file to read
    FILE *file = fopen(dictionary, "r");
    char word[LENGTH + 1];

    // Check if file load was successful
    if (!file)
    {
        return false;
    }

    // Allocate memory for all nodes
    all_nodes = malloc(143091 * sizeof(node));

    // Check if malloc was successful
    if (!all_nodes)
    {
        printf("Unable to allocate memory, program exiting\n");
        return false;
    }

    // Read strings from file one at a time until EOF
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a new node for every node
        node *new_node = all_nodes + word_count;

        // Copy word value and next pointer of new node
        strcpy(new_node->word, word);
        new_node->next = NULL;

        // Hash word to find what index to store word in the hashtable
        int index = hash(word);

        // Insert the word into the hash table
        if (table[index] == NULL)
        {
            // If there are no nodes at a particular index
            table[index] = new_node;
        }
        else
        {
            // If a node is already at index, insert the new node at the start of the linked-list at a particular index
            new_node->next = table[index];
            table[index] = new_node;
        }

        // Increase word count after every word for use in size function
        word_count++;

    }

    // Close file after creating the hash table and return true
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Free all nodes
    free(all_nodes);

    return true;
}
