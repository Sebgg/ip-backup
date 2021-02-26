#! /usr/bin/env python3

def contains(string, list_):
    res = list(filter(lambda x: x == string, list_))
    return len(res) > 0

haystack = 'Can you find the needle in this haystack?'.split()
print(contains('you', haystack))
