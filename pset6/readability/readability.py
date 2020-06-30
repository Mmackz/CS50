from cs50 import get_string
import re

# Get input from user
text = get_string("Text: ")

# Find number of letters and sentences are in text
letter_count = len(re.findall("[A-Za-z]", text))
sentence_count = len(re.findall("[\?\.\!]", text))

# Find number of letters in text
if letter_count <= 0:
    word_count = 0
else:
    word_count = len(re.findall(" +", text)) + 1

# Find average number of letters/sentences per 100 words
try:
    avg_letters = letter_count / word_count * 100
except ZeroDivisionError:
    avg_letters = 0
try:
    avg_sentences = sentence_count / word_count * 100
except ZeroDivisionError:
    avg_sentences = 0

# Apply Coleman-Liau formula to find grade level
index = 0.0588 * avg_letters - 0.296 * avg_sentences - 15.8

# Print grade level of input text
if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {round(index)}")