#  Ask for height input from user
height = (input("Height: "))

# Reprompt until user gives valid input (1-8)
while not height.isnumeric() or not 1 <= int(height) <= 8:
    height = (input("Height: "))

# Converts input to an integer
height = int(height)

# Print out the pyramid
i = 1
while(i <= height):
    print(" " * (height - i), end="")
    print("#" * i, end="")
    print("  ", end="")
    print("#" * i)
    i += 1