#! /usr/bin/env python3

def add(x,y):
    return x+y

def sub(x,y):
    return x-y

def mult(x,y):
    return x*y

def partial(func, value):
    return lambda x: func(x, value)

add_five = partial(add, 5)
print(add_five(3))
