#! /usr/bin/env python3
db = [
    {'name': 'Jakob', 'position': 'assistant'},
    {'name': 'Åke', 'position': 'assistant'},
    {'name': 'Ola', 'position': 'examiner'},
    {'name': 'Henrik', 'position': 'assistant'},
]

def dbsearch(db, key, value):
    return list(filter(lambda x: x[key] == value, db))

print(dbsearch(db, "position", "assistant"))
print(dbsearch(db, 'position', 'examiner'))
print(dbsearch(db, 'name', 'Ola'))
