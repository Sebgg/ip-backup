from sokoban import *

def pop_obj(field, coords):
    '''
    Pops an object a coords(x, y)
    in the playing field and returns it.
    '''

    obj = field[coords]
    #obj = get_obj(field, coords)
    del field[coords]
    return obj

def sokoban_add(field, obj):
    key = get_coords(obj)
    field[key] = obj
    return field

def get_player_pos(field):
    '''Returns current position of the player.'''

    for key, item in field.items():
        if is_player(item) or is_stored_player(item):
            return get_coords(item)

def can_player_move(field, coords, x, y):
    '''
    Collisionhandler for player and a eventual crate in front of it.
    '''

    player = get_obj(field, coords)
    can_move = True
    if (get_x(player)+x, get_y(player)+y) not in field.keys():
        return can_move


    obj = get_obj(field, (get_x(player)+x, get_y(player)+y))
    if is_wall(obj):
        can_move = False
    elif is_crate(obj) or is_stored_crate(obj):
        crate_coords = (get_x(obj)+x, get_y(obj)+y)

        if crate_coords not in field.keys():
            return "Crate"

        obj = get_obj(field, crate_coords)
        if is_crate(obj) or is_stored_crate(obj):
            can_move = False
        elif is_wall(obj):
            can_move = False
        else:
            return "Crate"
    return can_move

def store_obj(field, coords, change_coords):
    '''
    Handles the events when a player or crate
    moves on top of a storage space. Also handles
    if a crate moves between, and player moves on a, storage space.
    '''

    obj = pop_obj(field, coords)
    storage = pop_obj(field, change_coords)
    if is_stored_player(obj):
        set_coords(obj, change_coords[0], change_coords[1])
        set_coords(storage, coords[0], coords[1])
        sokoban_add(field, obj)
        sokoban_add(field, storage)
    elif is_stored_crate(obj):
        set_coords(obj, change_coords[0], change_coords[1])
        set_coords(storage, coords[0], coords[1])
        sokoban_add(field, obj)
        sokoban_add(field, storage)
    elif is_player(obj):
        storage = set_icon(storage, '+')
    elif is_crate(obj):
        storage = set_icon(storage, '*')

    sokoban_add(field, storage)

def unstore_obj(field, coords, change_coords):
    '''
    Handles when a player or crate moves of a storagespace
    '''

    obj = pop_obj(field, coords)
    storage = create_object('.', get_x(obj), get_y(obj))
    if is_stored_player(obj):
        objec = create_object('@', change_coords[0], change_coords[1])
    elif is_stored_crate(obj):
        objec = create_object('o', change_coords[0], change_coords[1])
    sokoban_add(field, storage)
    sokoban_add(field, objec)

def change_store(field, coords, crate_coords, change_coords):
    '''
    Checks if change_coords is in field and if the next space is a storage.
    Stores the object accordingly with store_obj and unstore_obj functions.
    '''
    if change_coords in field.keys() and is_storage(field[change_coords]):
        store_obj(field, crate_coords, change_coords)
        store_obj(field, coords, crate_coords)
    else:
        unstore_obj(field, crate_coords, change_coords)
        store_obj(field, coords, crate_coords)

def get_obj(field, coords):
    '''
    Returns the object at coords(x, y)
    '''

    return field[coords]

def move_obj(field, coords, x, y):
    '''
    Moves the object at coords(x, y) on x and y steps through the field.
    '''
    obj = pop_obj(field, coords)
    set_x(obj, get_x(obj)+x)
    set_y(obj, get_y(obj)+y)
    sokoban_add(field, obj)

def change_pos(field, coords, x, y):
    '''
    The bulk of the moving. Calls appropriate functions based on the players next move.
    '''
    obj = get_obj(field, coords)

    if can_player_move(field, coords, x, y) == True:
        '''Moves the player without moving any crate'''

        change_coords = (get_x(obj)+x, get_y(obj)+y)
        if is_stored_player(obj) and change_coords in field.keys():
            store_obj(field, coords, change_coords)
        elif is_stored_player(obj):
            unstore_obj(field, coords, change_coords)
        else:
            move_obj(field, coords, x, y)
            
    elif can_player_move(field, coords, x, y) == "Crate":
        '''Moves the player and a crate.'''

        crate_coords = (get_x(obj)+x, get_y(obj)+y)
        crate_obj = get_obj(field, (get_x(obj)+x, get_y(obj)+y))
        change_coords = (get_x(obj)+(x*2), get_y(obj)+(y*2))
        if is_stored_crate(crate_obj):
            change_store(field, coords, crate_coords, change_coords)
        elif is_stored_player(obj):
            move_obj(field, crate_coords, x, y)
            unstore_obj(field, coords, crate_coords)

        if change_coords in field.keys():
            if is_storage(get_obj(field, change_coords)):
                store_obj(field, crate_coords, change_coords)
                move_obj(field, coords, x, y)
        else:
            move_obj(field, crate_coords, x, y)
            move_obj(field, coords, x, y)

def move_player(move, field):
    '''
    Calls on the change_pos function with appropriate x and y,
    based on which way the player wants to move.
    '''

    coords = get_player_pos(field)
    if move == 'w':
        change_pos(field, coords, 0, -1)

    elif move == 'a':
        change_pos(field, coords, -1, 0)

    elif move == 's':
        change_pos(field, coords, 0, 1)

    elif move == 'd':
        change_pos(field, coords, 1, 0)
