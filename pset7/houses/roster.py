import csv
import sys
import re
from cs50 import SQL

# Check for valid command-line arguments
if len(sys.argv) != 2 or not re.search("(^[a-zA-Z]+$)", sys.argv[1]):
    print("Invalid command-line arguments. Please enter command in the following format using only alphabetic characters:")
    sys.exit("Usage: python roster.py house_name")

# Open students database
try:
    db = SQL("sqlite:///students.db")
except:
    sys.exit("There was an error opening the database. Program exiting.")

# Make sure house provided is valid
valid_arg = False
house_list = db.execute("SELECT DISTINCT house FROM students")
for row in house_list:
    if sys.argv[1] == row['house']:
        valid_arg = True
        break

# Execute query on database
if valid_arg:
    db_data = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last, first", sys.argv[1])

    for row in db_data:
        if row['middle'] == None:
            print(f"{row['first']} {row['last']}, born {row['birth']}")
        else:
            print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}")
else:
    print("House provided is not valid.\nThe following house names are valid:")
    print("Gryffindor\nHufflepuff\nRavenclaw\nSlytherin")
    sys.exit("Usage: python roster.py house_name")
