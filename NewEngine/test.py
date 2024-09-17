import random

def generate_hash_table():
    hash_table = {}
    for i in range(256):
        ranVal = random.randint(0, 255)
        while ranVal in hash_table.values():
            ranVal = random.randint(0, 255)
        hash_table[i] = ranVal
    return hash_table

# Example usage
hash_table = generate_hash_table()
print(hash_table)