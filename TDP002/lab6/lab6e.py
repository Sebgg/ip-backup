#! /usr/bin/env python3

import argparse, re, os

def rewrite_end(filename, copyright_file, new_end):
    rewriter(filename, copyright_file)
    if new_end:
        end_changer(filename)

def recurser(args):
    ls = os.listdir(args.destination)
    file_ending = ''
    if args.c == None:
        file_ending = '.*'
        for file in ls:
            rewrite_end(args.destination + '/' + file, args.copyright_file, args.u)
    else:
        file_ending = '.' + args.c
        for file in ls:
            if os.path.splitext(file)[1] == file_ending:
                rewrite_end(args.destination + '/' + file, args.copyright_file, args.u)

def copyright_reader(filename):
    output = ""
    with open(filename, 'r') as f:
        for line in f:
            output = output + line
    return '\\n' + output

def rewriter(filename, copyright_file):
    with open (filename, 'r') as f:
        result = re.sub(re.compile("(?s)(?<=<BEGIN COPYRIGHT>)(.*?)(?=<END COPYRIGHT>)", re.DOTALL), copyright_reader(copyright_file), f.read())

    with open(filename, 'w') as f:
        f.write(result)

def end_changer(filename, new_ending):
    os.rename(filename, os.path.splitext(filename)[0] + '.' + new_ending)

def main_():
    parser = argparse.ArgumentParser()
    parser.add_argument("copyright_file", help="Specifies what file the program should read copyright information from", type=str)
    parser.add_argument("destination", help="Specifies the destination file or folder the program should run in ", type=str)
    parser.add_argument("-c", help="Specifies file endings you want to add copyright to", type=str)
    parser.add_argument("-u", help="Change file ending for documents you've written the copyright information to.", type=str)
    args = parser.parse_args()
    if os.path.isfile(args.destination):
        rewriter(args.destination, args.copyright_file)
        if args.u:
            end_changer(args.destination, args.u)
    elif os.path.isdir(args.destination):
        recurser(args)
    return 0


if __name__ == "__main__":
    main_()
