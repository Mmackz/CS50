import math
from cs50 import get_float

# Ask user for input
change = get_float("Changed owed: ")

# Check if user gives correct input
while change <= 0:
    change = get_float("Changed owed: ")

# Calculate amount of quarters 
quarters = math.floor(change * 4)
change = round(change - quarters * 0.25, 2)

# Calculate amount of dimes
dimes = math.floor(change * 10)
change = round(change - dimes * 0.1, 2)

# Calculate amount of nickels
nickels = math.floor(change * 20)
change = round(change - nickels * 0.05, 2)

# Calculate amount of pennies
pennies = math.floor(change * 100)

# Print total amount of coins 
print(quarters + dimes + nickels + pennies)