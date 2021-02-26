#! /usr/bin/env python3

num = [1, 2, 3, 4, 6, 8, 9, 11, 23]
nam = ['Ada', 'Boolean', 'Kilo', 'Lima', 'Tango', 'Victor', 'Zulu']
imdb = [{'title': 'Blablamovie123', 'score': 7},
        {'title': 'hejhoppmovie', 'score': 9},
        {'title': 'blopblap', 'score': 10}]

def binary_search(list, value, func=lambda x: x):
    low = 0
    high = len(list) - 1
    while low <= high :
        mid = (low + high) // 2
        if func(list[mid]) < value:
            low = mid + 1
        elif func(list[mid]) > value:
            high = mid - 1
        else:
            return mid;
    return -1

print(binary_search(nam, 'Zulu'))
print(binary_search(num, 2))
print(binary_search(imdb, 9, lambda e: e['score']))
