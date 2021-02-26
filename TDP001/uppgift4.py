#! /usr/bin/env python3

import re

def main_(filename):
    with open(filename, 'r') as f:
        return re.findall('(?s)(?<=<html lang="[A-Za-z]{2}">)(.*?)(?=</html>)', f.read())

if __name__ == "__main__":
    filename = input("What file do you want to look in? ")
    print(*main_(filename), sep='\n')
