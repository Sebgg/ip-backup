#!/usr/bin/env python3

primsum = 0

for x in range(1,1001):
    for y in range(2,x):
        if x % y == 0:
            break
    else:
        primsum += x

print(primsum)
