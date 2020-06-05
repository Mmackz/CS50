#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    bool is_valid = false;

    // Loop the candidate array and check if the vote name matches
    for (int i = 0; i < candidate_count; i++)
    {
        // If valid push candidates index "i" to ranks array at the index of "rank"
        if (!strcmp(candidates[i], name))
        {
            is_valid = true;
            ranks[rank] = i;
        }
    }

    return is_valid;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Increase preferences array given each vote
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Loop i and j=i+1, if same value.. no pair. if different values.. winner has higher value
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                // i wins j loses
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[j][i] > preferences[i][j])
            {
                // j wins i loses
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // "max" is the index of the pair with the highest winning margin
    int max;
    int max_val;
    int j_val;

    for (int i = 0; i < pair_count; i++)
    {

        // Set current index as max
        max = i;
        max_val = preferences[pairs[max].winner][pairs[max].loser] - preferences[pairs[max].loser][pairs[max].winner];

        // Compare max to other indexes of the array and compare for victory margin
        for (int j = i + 1; j < pair_count; j++)
        {
            j_val = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];
            if (j_val > max_val)
            {
                max = j;
            }
        }

        if (max != i)
        {
            int temp_w = pairs[max].winner;
            int temp_l = pairs[max].loser;
            pairs[max].winner = pairs[i].winner;
            pairs[max].loser = pairs[i].loser;
            pairs[i].winner = temp_w;
            pairs[i].loser = temp_l;
        }
    }
    return;
}

bool is_cycle(int W, int L)
{
    // If loser of a call to is_cycle ends up back at the winner
    if (locked[L][W])
    {
        return true;
    }

    // Check if loser beats anybody
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[L][i])
        {
            // If the loser beats someone check if his path leads back to winner
            if (is_cycle(W, i))
            {
                // If any calls to is_cycle come back true (found locked[L][W]), this will return true back to lock pairs
                return true;
            }
        }
    }

    // After checking every path and finding no cycles
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Lock first 2 pairs then check for cycles
    // If no cycle, add lock pairs winner over loser
    for (int i = 0; i < pair_count; i++)
    {
        if (i < 2 || !is_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // Search the locked array and see if anyone has no edges locked against them
    for (int i = 0; i < candidate_count; i++)
    {
        bool is_source = true;

        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                // If a candidate has an edge locked stop looping
                is_source = false;
                break;
            }
        }

        // If candidate has no edges locked print their name
        if (is_source)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

