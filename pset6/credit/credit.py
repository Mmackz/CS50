from cs50 import get_string

# Ask user for input
cc_num = get_string("Number: ")

# Check if input is valid
while not cc_num.isnumeric() or len(cc_num) < 4:
    cc_num = get_string("Number: ")

# Split the string into individual characters in a list
test = list(cc_num)
cc_num = list(cc_num)


# Convert characters to integers
for i in range(len(cc_num)):
    cc_num[i] = int(cc_num[i])
    test[i] = int(cc_num[i])

# Get length of card number
length = len(cc_num)

# Double every second digit from end, applying luhn formula
if length % 2 == 0:
    i = 0
    while i < length:
        cc_num[i] *= 2
        if cc_num[i] >= 10:
            cc_num[i] = (cc_num[i] + 1) % 10
        i += 2
else:
    i = 1
    while i < length:
        cc_num[i] *= 2
        if cc_num[i] >= 10:
            cc_num[i] = (cc_num[i] + 1) % 10
        i += 2

# Find sum of all digits after applying the luhn formula       
sum_digits = sum(cc_num)

# Print out card type if card is valid
if sum_digits % 10 == 0:
    if length == 16 and test[0] == 5 and 0 < test[1] < 6:
        print("MASTERCARD")
    elif 13 <= length <= 16 and test[0] == 3 and test[1] == 4 or test[1] == 7:
        print("AMEX")
        exit(0)
    elif length == 13 or length == 16 and test[0] == 4:
        print("VISA")
        exit(0)
    else:
        print("INVALID")
        exit(0)
else:
    print("INVALID")
    exit(0)