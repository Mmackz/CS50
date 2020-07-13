import csv
import sys
import re
from cs50 import SQL

# Check for valid command-line arguments
if len(sys.argv) != 2 or not re.search("(.csv)$", sys.argv[1]):
    print("Invalid command-line arguments. Please enter command in the following format:")
    sys.exit("Usage: python import.py filename.csv")

# Open database file
try:
    db = SQL("sqlite:///students.db")
except:
    sys.exit("There was an error opening the database. Program exiting.")

# Open CSV file and read data into a dictionary
with open(sys.argv[1]) as csv_file:
    csv_data = csv.DictReader(csv_file)

    # Iterate over each row in the dictionary and parse the data into variables
    for row in csv_data:

        # Split name into 2 or 3 parts, depending on if a middle name is given
        split_name = re.split(" {1}", row['name'])
        if len(split_name) == 2:
            first_name = split_name[0]
            middle_name = None
            last_name = split_name[1]
        else:
            first_name = split_name[0]
            middle_name = split_name[1]
            last_name = split_name[2]

        # Insert the parsed data into the students database
        try:
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                       first_name, middle_name, last_name, row['house'], row['birth'])
        except RuntimeError:
            sys.exit("The Database may be locked. You may have it open in another program.")