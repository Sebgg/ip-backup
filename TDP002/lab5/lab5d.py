#! /usr/bin/env python3

import os

def get_ls():
    dir = os.listdir(path='.')
    for item in dir:
        if os.path.isdir(item):
            print(item + '/')
        else:
            print(item)

def get_file_content(filename):
    with open(filename, 'r') as f:
        for line in f:
            print(line)

def shell_():
    while True:
        usrinput = input('Command> ')
        usrinput = usrinput.split()
        if 'pwd' in usrinput:
            print(os.getcwd())
        elif 'cd' in usrinput:
            os.chdir(usrinput[1])
        elif 'ls' in usrinput:
            get_ls()
        elif 'cat' in usrinput:
            get_file_content(usrinput[1])
        else:
            print("invalid command")



shell_()
