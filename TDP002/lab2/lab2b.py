#!/usr/bin/env python3

def create_shopping_list():
    slist = []
    usrinput = input("Vad vill du lagga till? ")
    lists = usrinput.split()

    for item in lists:
        slist.append(item)
    return slist

def shopping_list(slist):
    for index, item in enumerate(slist):
        print("{}. {}".format(str(index+1), item.title()))

def shopping_add(slist):
    usrin = input("Vad ska laggas till i listan? ")

    slist.append(usrin)

def shopping_remove(slist):
    usrin = input("Vad vill du ta bort? ")

    del(slist[int(usrin)-1])

def shopping_edit(slist):
    usrin = input("Vilken sak vill du andra pa? ")
    usred = input("Vad ska det sta istallet for \"{}\"?".format(slist[int(usrin)-1]))

    slist[int(usrin)-1] = usred
