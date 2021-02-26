#! /usr/bin/env python3

import json

def load(filename):
    #funkar
    try:
        with open(filename, 'r') as f:
            res = json.load(f)
            return res
    except FileNotFoundError:
        return None

def get_project_count(db):
    #funkar
    return len(db)

def get_project(db, id):
    #funkar
    for item in db:
        if id == item["project_id"]:
            return item
    return None

def get_latest_project(db):
    projects = search(db)
    return projects[0]

def check_search(search_word, item):
    #funkar
    if isinstance(item, int):
        try:
            search_word = int(search_word)
            if search_word == item:
                return True
        except ValueError:
                pass
    elif isinstance(item, list):
        for item_ in item:
            if check_search(search_word, item_):
                return True
    else:
        if search_word.upper() in item.upper():
            return True

    return False

def search_project(project, search_word, searchfield):
    #funkar
    if isinstance(searchfield, list):
        for field in searchfield:
            if check_search(search_word, project[field]):
                return True
    else:
        for item in project.values():
            if check_search(search_word, item):
                return True
    return False

def search(db, sort_by='start_date', sort_order='desc',
           techniques=None, search=None, searchfield=None):
    #funkar
    db_ = db.copy()
    remove_queue = set()

    if searchfield == []:
        return None
    for project in db_:
        if search != None:
            if not search_project(project, search, searchfield):
                remove_queue.add(project['project_id'])
                continue
        if techniques != None and techniques != []:
            if project["techniques_used"] == []:
                remove_queue.add(project["project_id"])
            for technique in techniques:
                if technique not in project["techniques_used"]:
                    remove_queue.add(project['project_id'])
                    continue

    for project in db:
        if project['project_id'] in remove_queue:
            db_.remove(project)

    if sort_order=='desc':
        return sorted(db_, key=lambda x : x[sort_by], reverse=True)
    elif sort_order=='asc':
        return sorted(db_, key=lambda x : x[sort_by])

def get_techniques(db):
    #funkar
    techniques = []
    for item in db:
        for technique in item["techniques_used"]:
            techniques.append(technique)
    techniques = list(set(techniques))
    return sorted(techniques, key=str.lower)

def get_technique_stats(db):
    #funkar
    techniques = get_techniques(db)
    technique_stats = {}
    for technique in techniques:
        technique_stats[technique] = []

    for item in db:
        for technique in techniques:
            if technique in item["techniques_used"]:
                technique_stats[technique].append(item)
    return technique_stats
