# TDP015 Programming Assignment 5
# Graphs
# Skeleton Code

import sys

# In one of my current research projects, I am developing algorithms
# for the parsing of natural language to meaning representations in
# the form of directed graphs:
#
# http://www.ida.liu.se/~marku61/ceniit.shtml
#
# A desirable property of these graphs is that they should be acyclic,
# that is, should not contain any (directed) cycles. Your task in this
# assignment is to implement a Python function that tests this
# property, and to apply your function to compute the number of cyclic
# graphs in one of the datasets that I am using in my research.
#
# Your final script should be callable from the command line as follows:
#
# $ python3 p5.py ccg.train.json
#
# This should print out the IDs of the cyclic graphs in the file:
#
# $ python3 p5.py ccg.train.json
# 22172056
# 22153010
# 22106047
#
# The graphs are stored in a JSON file containing a single dictionary
# mapping graph ids (8-digit integers) to graphs, where each graph is
# represented as a dictionary mapping vertices (or rather their ids)
# to lists of neighbouring vertices.


def cyclic(graph):
    """Test whether the directed graph `graph` has a (directed) cycle.

    The input graph needs to be represented as a dictionary mapping vertex
    ids to iterables of ids of neighboring vertices. Example:

    {"1": ["2"], "2": ["3"], "3": ["1"]}

    Args:
        graph: A directed graph.

    Returns:
        `True` iff the directed graph `graph` has a (directed) cycle.
    """
    deg = {d: 0 for d in graph.keys()}
    for neighbours in graph.values():
        for neighbour in neighbours:
            deg[neighbour] += 1

    lonely = [k for k, v in deg.items() if v == 0]

    nr_nodes = 0
    while(len(lonely) > 0):
        nr_nodes += 1
        node = lonely.pop(0)
        for neighbour in graph[node]:
            deg[neighbour] -= 1
            if deg[neighbour] == 0:
                lonely.append(neighbour)

    return len(graph) - nr_nodes > 2

if __name__ == "__main__":
    loaded = eval(open(sys.argv[1], 'r').read())
    for graph_id in loaded:
        if cyclic(loaded[graph_id]):
            print(graph_id)
            print(loaded[graph_id])
