import random
from card import *

def create_deck():
    """Creates a list with 4 suits with 14 cards in each and 2 jokers."""

    deck = [create_card(number, suits) for suits in range(1,3) for number in range(1,14)]
    deck.append(create_card('a', 5))
    deck.append(create_card('b', 5))
    return deck

def pick_card(deck, card=None, position=None):
    """
    Takes a card from the deck and returns it.
    If you don't specify a card, it will take the
    bottom one.
    """

    if card == None and position == None:
        return deck.pop()
    elif card == None:
        return deck.pop(position)

    for index, item in enumerate(deck):
        if item == card:
            return deck.pop(index)

def insert_card(card, deck, position):
    """Inserts a card at a given position"""

    deck.insert(position, card)
    return deck

def shuffle_deck(deck, seed):
    """
    Shuffles the deck and cuts the deck. Seed determines the shuffle order.
    """
    half = int(len(deck)/2)
    random.Random(seed).shuffle(deck)
    deck = deck[half:] + deck[:half]
    return deck

def find_card(card, deck):
    """
    Returns the index of the card in the deck. Can take both a Card object, or
    a card plaintext as input, e.g. both (1, 1) and "Ace of Hearts" works.
    """

    if isinstance(card, str):
        card = ugly_card(card)

    for index, item in enumerate(deck):
        if item == card:
            return index

def cut_deck(deck):
    """
    Checks the index from a and b joker to see which one is higher up in the
    deck. Cut the deck in 3 parts and put them back together in C,B,A order
    instead of A,B,C.
    """
    aindex = deck.index(('a', 5))
    bindex = deck.index(('b', 5))
    if aindex < bindex:
        deck = deck[bindex+1:] + deck[aindex:bindex+1] + deck[:aindex]
    elif bindex < aindex:
        deck = deck[aindex+1:] + deck[bindex:aindex+1] + deck[:bindex]
    return deck
