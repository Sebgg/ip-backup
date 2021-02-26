#! usr/bin/env python3
from functools import reduce

def numsum(end, func):
    return reduce(func, [number for number in range(1, end+1)])
