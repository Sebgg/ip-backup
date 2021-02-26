#!/usr/bin/env python3
import random
from deck import *
from card import *
import string

def get_alpha():
    """Returns a alphabet dictionary. Like {1:'A'...26:'Z'}"""

    return {i+1 : chr(65+i) for i in range(26)}

def get_alpha_revkey():
    """Returns reverse of get alpha. {'A':1...'Z':26}"""

    return {chr(65+i): i+1 for i in range(26)}

def move_card(deck):
    """
    Assigns the value of the bottom card to variable and then moves that
    amount of cards from the top to the bottom, above the last card.
    """

    card = pick_card(deck)
    val = get_value(card)
    if val in ['a', 'b']:
        insert_card(card, deck, 0)
        return deck
    move = 0
    while move < val:
        move += 1
        temp = pick_card(deck, position=0)
        deck = insert_card(temp, deck, len(deck))

    deck = insert_card(card, deck, len(deck))
    return deck

def encrypt_card(deck):
    """
    Chooses a card based on value of top card in deck. Returns a
    single value, based on suit and value of chosen card.
    """

    alphabet = get_alpha()

    deck = deck[:]

    card = pick_card(deck, position=0)
    if get_suit(card) == 5:
        deck = insert_card(card, deck, 0)
        return ''

    val = get_value(card)
    deck = insert_card(card, deck, 0)

    card = pick_card(deck, position=val)
    if get_suit(card) == 5:
        deck = insert_card(card, deck, val)
        return ''

    encrypt_val = get_encrypt_val(card)
    deck = insert_card(card, deck, val)

    return alphabet[encrypt_val]

def move_joker(card, deck):
    """
    Checks the value of found card and moves it depending on its
    value and position
    """

    card = ugly_card(card)
    x = find_card(card, deck)
    y = pick_card(deck, card)
    #import pdb; pdb.set_trace()
    if get_value(y) == 'a':
        if x == len(deck):
            deck = insert_card(y, deck, 1)
        else:
            deck = insert_card(y, deck, x+1)
    elif get_value(y) == 'b':
        if x == len(deck):
            deck = insert_card(y, deck, 2)
        elif x == len(deck)-1:
            deck = insert_card(y, deck, 1)
        else:
            deck = insert_card(y, deck, x+2)
    return deck

def solitaire_keystream(length, deck, seed):
    """Generates a encryption key with help from other functions"""

    key = ""

    while length > 0:
        deck = move_joker('a of Joker', deck)
        deck = move_joker('b of Joker', deck)
        deck = cut_deck(deck)
        deck = move_card(deck)
        ch = encrypt_card(deck)
        if ch != '':
            length -= 1
            key += encrypt_card(deck)

    return key


def lower_uppercase(text):
    """
    Transforms text from lowercase to uppercase and removes letter not in A-Z.
    """
    alpha_revkey = get_alpha_revkey()
    converted = ''
    for l in text:
        l = l.title()
        if l in alpha_revkey:
            converted += l

    return converted

def letter_numerical(text):
    """
    Transforms letters to their corresponding number. A = 1, Z = 26.
    """

    alpha_revkey = get_alpha_revkey()
    return [alpha_revkey[l] for l in text if (l) in alpha_revkey]

def numerical_letter(text):
    """
    Transforms numbers to their corresponding letter. 1 = A, 26 = Z.
    """

    alphabet = get_alpha()
    return [alphabet[int(l)] for l in text if int(l) in alphabet]


def encrypt_text(text, deck, seed):
    """
    Uses other functions to encrypt input text. Seed sets the shuffle
    output, so it is the same, for every unshuffled deck. Seed needs to be
    the same when decrypting and encrypting.
    """

    text = lower_uppercase(text)
    deck = shuffle_deck(deck, seed)
    key = solitaire_keystream(len(text), deck, seed)
    text = letter_numerical(text)
    key = letter_numerical(key)

    """Assigns value to the text and puts it in a list."""
    out = []

    for i in range(len(text)):
        temp = text[i] + key[i]
        if temp > 26:
            temp -= 26
        out.append(temp)

    out = numerical_letter(out)
    out = ''.join(out)
    return out

def decrypt_text(text, deck, seed):
    """
    Uses other functions to decrypt input text. Seed sets the shuffle
    output, so it is the same, for every unshuffled deck. Seed needs to be
    the same when decrypting and encrypting.
    """

    text = letter_numerical(text)
    deck = shuffle_deck(deck, seed)
    key = solitaire_keystream(len(text), deck, seed)
    key = letter_numerical(key)


    out = []

    for i in range(len(text)):
        temp = text[i] - key[i]
        if temp < 1:
            temp += 26
        out.append(temp)

    out = numerical_letter(out)
    out = ''.join(out)
    return out
