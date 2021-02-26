#! /usr/bin/env python3

imdb = [{'title': 'Blablamovie123', 'score': 10},
        {'title': 'hejhoppmovie', 'score': 9},
        {'title': 'blopblap', 'score': 10}]

def linear_search(list, value, func=lambda x: x):
    for elem in list:
        if func(elem) == value:
            return elem
    return None


print(linear_search(imdb, 'blopblap', lambda e: e['title']))
