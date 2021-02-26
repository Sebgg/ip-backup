def create_card(number, suit):
    """Creates a card"""

    return (number, suit)

def get_value(card):
    """Returns the value of the card"""

    return card[0]

def get_suit(card):
    """Returns the suit of the card"""

    return card[1]

def get_encrypt_val(card):
    """
    Takes a card and transforms it to a singel value
    based on its value and suit.
    """

    val = get_value(card)
    suit = get_suit(card)

    if suit == 5:
        return 27

    suit -= 1 #because it starts at 1, first suit-value is 13*0 + val

    return ((suit * 13) + val)

def show_card(card):
    """Prettifies the output of a card for readability"""

    specials = {11:"Knight", 12:"Queen", 13:"King", 1:"Ace"}
    suits = {1:"Hearts", 2:"Spades", 3:"Diamonds", 4:"Clubs", 5:"Joker"}
    name = ""
    val = get_value(card)
    suit = get_suit(card)
    if val in specials:
        name = "{} of {}".format(specials[val], suits[suit])
    else:
        name = "{} of {}".format(val, suits[suit])

    return name

def ugly_card(card):

    specials = {"Knight": 11, "Queen": 12, "King": 13, "Ace":1}
    suits = {"Hearts": 1, "Spades":2, "Diamonds":3, "Clubs":4, "Joker":5}
    usrinput = card.split()
    if usrinput[0] in specials:
        return create_card(specials[usrinput[0]], suits[usrinput[2]])
    elif usrinput[0] in ['a', 'b']:
        return create_card(usrinput[0], suits[usrinput[2]])
    else:
        return create_card(int(usrinput[0]), suits[usrinput[2]])
