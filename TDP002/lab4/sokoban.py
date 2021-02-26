#! /usr/bin/env python3
from object import *
from soko_move import *
import os

def create_sokoban_field():
    '''Creates the playing field.'''

    field =  {}
    return field

def sokoban_load(field, level):
    '''
    Loads the chosen sokoban level from a file
    and creates a field with the data.
    '''

    with open(level, 'r') as f:
        y = 0
        for line in f:
            x = 0
            for char in line:
                if char != ' ' and char != '\n':
                    field = sokoban_add(field, create_object(char, x, y))
                x +=1
            y += 1
    return field



def sokoban_display(field):
    '''
    Displays the play field in terminal.
    '''

    max_x = 0
    max_y = 0
    for key in field.keys():
        if key[0] > max_x:
            max_x = key[0]
        if key[1] > max_y:
            max_y = key[1]

    for y in range(max_y+1):
        line_out = ""
        for x in range(max_x+1):
            if (x, y) in field.keys():
                line_out += get_icon(field[(x, y)])
            else:
                line_out += ' '
        print(line_out)

def check_win(field):
    '''
    Checks to see if all crates are on a storage space.
    If true, stops the current gameloop.
    '''
    for key, item in field.items():
        if is_crate(item):
            return True
    return False

def handle_input(inp):
    '''
    Makes input for user easier, can be expanded to a dictionary
    '''
    levels = {
        '1':"first_level.txt",
        '2':"second_level.txt",
        '3':"third_level.txt",
        '4':"fourth_level.txt"
    }
    if inp not in levels.keys():
        return False

    return levels[inp]


def sokoban_play():
    '''
    Main gameloop, breaks when check_win returns false.
    '''

    level = input("Which level do you want to play? \n"
                  "Choose anumber between 1 and 4: ")
    level = handle_input(level)
    while level == False:
        level = input("That level doesn't exist, try again: ")
        level = handle_input(level)

    field = create_sokoban_field()
    field = sokoban_load(field, level)
    playing = True
    while playing:
        os.system('clear')
        sokoban_display(field)
        move = input("Do you want to move (w)up, (s)down, (a)left, (d)right? (q) to quit: ")
        if move == 'q':
            os.system('clear')
            break
        move_player(move, field)
        playing = check_win(field)
    os.system('clear')
    if not playing:
        sokoban_display(field)
        print("Grattis! Du vann!!")
