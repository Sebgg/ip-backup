#!/usr/bin/env python3

def printl(tecken, length):
    return tecken*length

def frame(phrase):
    ram = len(phrase)+4
    print("{0}\n* {1} *\n{0}".format(printl('*', ram), phrase))

def triangle(h):
    ln = 1
    for number in range(1,h+1):
        print(printl('*', ln))
        ln += 2

def flag(size):
    for number in range(1,size*5):
        print("{0}{1}{0}".format(printl('*', size*10), printl(' ', size)))
    print((printl(' ', size*21))*size)
    for number in range(1,size*5):
        print("{0}{1}{0}".format(printl('*', size*10), printl(' ', size)))
