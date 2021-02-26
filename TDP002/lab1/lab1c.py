#!/usr/bin/env python3

def primw():
    number = 2521
    eq_div = True
    while eq_div:
        for num in range(1, 14):
            if number % num != 0:
                number += 1
                break
        else:
            return number


print(primw())
