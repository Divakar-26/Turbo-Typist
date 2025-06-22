import random
import re

# Load from dictionary or download from GitHub
with open("words_alpha.txt") as f:  # Replace with your word file
    raw_words = f.readlines()

# Clean words: only pure lowercase alphabetic, no punctuation or possessives
clean_words = {
    word.strip().lower()
    for word in raw_words
    if re.fullmatch(r"[a-z]{3,10}", word.strip())
}

# Pick 3000 unique clean words
selected = random.sample(list(clean_words), 3000)

# Save to a new file
with open("words_clean.txt", "w") as out:
    out.write("\n".join(selected))
