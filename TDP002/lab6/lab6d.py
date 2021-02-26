#! /usr/bin/env python3

database = [
    ('j', 'g'), ('a','u'), ('k','l'), ('o', 'i'),
    ('b','s'), ('@','.'), ('p','s'), ('o','e')
]

def quicksort(db, func=lambda x: x):
    if db == []:
        return []

    pivot = db[0]
    larger = []
    smaller = []
    for index in range(1, len(db)):
        if func(db[index]) > func(pivot):
            larger.append(db[index])
        else:
            smaller.append(db[index])
    
    return quicksort(smaller, func) + [pivot] + quicksort(larger, func)

print(quicksort(database, lambda e: e[0]))
