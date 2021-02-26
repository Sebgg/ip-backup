#! /usr/bin/env python3

database = [
    ('j', 'g'), ('a','u'), ('k','l'), ('o', 'i'),
    ('b','s'), ('@','.'), ('p','s'), ('o','e')
]

def insertion_sort(db, func=lambda x: x):
    for index in range(0, len(db)):
        while index > 0 and func(db[index-1]) > func(db[index]):
            temp = db.pop(index)
            db.insert(index-1, temp)
            index -= 1
    return db


print(insertion_sort(database, lambda e: e[0]))
