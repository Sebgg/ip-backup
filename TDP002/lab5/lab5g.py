#!/usr/bin/env python3

def compose(func_1, func_2):
    return lambda x: func_1(func_2(x))

def partial(func, value):
    return lambda x: func(value, x)

def make_filter_map(filter_, map_):
    filter_func = partial(filter, filter_)
    map_func = partial(map, map_)
    fil_map_func = compose(map_func, filter_func)
    return lambda x: list(fil_map_func(x))

comp = make_filter_map(lambda x: x % 2 == 1, lambda x: x * x)
print(comp(range(10)))
