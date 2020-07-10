import csv
import sys
import re

# Global variables
STR_LIST = list()
PROFILE_LIST = list()


# Check for valid command-line arguments
if len(sys.argv) != 3 or not re.search("(.csv)$", sys.argv[1]) or not re.search("(.txt)$", sys.argv[2]):
    print("Invalid command-line arguments. Please enter command in the following format:")
    print("Usage: python dna.py data.csv sequence.txt")

# Open  and read sequence text file into memory
with open(sys.argv[2], 'r') as dna_sequence:
    string = dna_sequence.read()


# Function to find the max STR count for each STR in string
def sequence_count(sequence):
    matches = re.finditer(fr"({sequence})+", string)
    match_list = list()
    for match in matches:
        match = match.group()
        match_list.append(len(match))

    if len(match_list) == 0:
        return 0
    else:
        return int(max(match_list) / len(sequence))


# Open and read CSV file into memory
with open(sys.argv[1], 'r') as csv_file:
    file = csv.reader(csv_file)
    i = 0
    j = 0
    for line in file:
        # Skip first line in file
        if i == 0:
            # Extract STRs from CSV file and store in list for comparing later
            while j < len(line):
                if j > 0:
                    STR_LIST.append(line[j])
                j += 1
        else:
            # Extract all the people and their STR counts to a seperate list for comparing later
            PROFILE_LIST.append(line)
        i += 1

# Compare each entry in profile list to see if it matches the STR counts of the provided DNA sequence
for entry in PROFILE_LIST:

    match_found = False

    for i in range(len(entry)):
        if i == 0:
            continue

        # If the entry matches all the sequence counts match is true
        if int(entry[i]) == sequence_count(STR_LIST[i - 1]):
            match = True
        else:
            # If any sequence count does not match, match = false and the loop breaks
            match = False
            break

    # If match is true, print the persons name
    if match:
        match_found = True
        print(entry[0])
        break

# If no match found, print "No match"
if not match_found:
    print("No match")