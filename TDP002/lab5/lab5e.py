#! /usr/bin/env python3

def mirror(x):
    return x

def stars(n):
    return '*' * n

def generate_list(func, x):
    res = []
    for x in range(1, x+1):
        res.append(func(x))
    return res

print(generate_list(mirror, 4))
print(generate_list(stars, 5))
