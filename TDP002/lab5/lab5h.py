#! /usr/bin/env python3

def compose(func_1, func_2):
    return lambda x: func_1(func_2(x))

def multiply_five(n):
    return n * 5

def add_ten(x):
    return x + 10
