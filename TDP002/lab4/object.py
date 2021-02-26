#! /usr/bin/env python3

def create_object(icon, x, y):
    '''
    Return a object, represented by a dictionary
    '''

    return {"icon": icon, "x": x, "y": y}

def get_x(obj):
    return obj["x"]

def get_y(obj):
    return obj["y"]

def get_icon(obj):
    return obj["icon"]

def set_x(obj, x):
    obj["x"] = x
    return obj

def set_y(obj, y):
    obj["y"] = y
    return obj

def set_icon(obj, icon):
    obj["icon"] = icon
    return obj

def is_wall(obj):
    if obj["icon"] == "#":
        return True
    return False

def is_storage(obj):
    if obj["icon"] == ".":
        return True
    return False

def is_player(obj):
    if obj["icon"] == "@":
        return True
    return False

def is_stored_player(obj):
    if obj["icon"] == "+":
        return True
    return False

def is_crate(obj):
    if obj["icon"] == "o":
        return True
    return False

def is_stored_crate(obj):
    if obj["icon"] == "*":
        return True
    return False

def get_coords(obj):
    x = get_x(obj)
    y = get_y(obj)
    return (x, y)

def set_coords(obj, x, y):
    set_x(obj, x)
    set_y(obj, y)
