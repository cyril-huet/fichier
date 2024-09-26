from thlr_automata import *

A = ENFA([0, 1, 2,3,4,6,7,8,9,10], [0], [11], ["a", "b","c","eps"], [(0,"a",2),(2,"eps",1),(1,"eps",3),(3,"b",4),(4,"eps",10),(1,"eps",6),(6,"eps",7),(7,"c",8),(8,"eps",9),(6,"eps",9),(9,"eps",10),(8,"eps",7)])
A.export("A")

B = ENFA([0, 1, 2], [0], [1], ["a", "b"], [(0, "a", 1), (0, "", 1),(1, "b", 2), (2, "", 0)])
B.export("B")

#[Q3]
def get_accessible_states(automaton,origin):
    incoming = {origin} # étape 1
    visited = set() # étape 1
    while incoming!=set():  # étape 3
        element = incoming.pop()    # étape 2
        visited.add(element)    # étape 2
        next = set()
        for elt in automaton.alphabet:    # étape 2
                next = next.union(automaton.get_successors(element,elt))
        for node in next:    # étape 2
            if node not in visited:    # étape 2
                incoming.add(node)    # étape 2
    return visited
#[/Q3]

#[Q4]
def is_accessible(automaton,state):
    for begin in automaton.initial_states:
        if state in get_accessible_states(automaton,begin):
            return True
    return False
#[/Q4]

#[Q5]
def is_co_accessible(automaton,state):
    qqch = get_accessible_states(automaton, state)
    for final in automaton.final_states:
        if final in qqch:
            return True
    return False
#[/Q5]

#[Q6]
def is_useful(automaton,state):
    return is_accessible(automaton,state) and is_co_accessible(automaton,state)
#[/Q6]

#[Q7]
def prune(automaton):
    l = list(automaton.all_states)
    for node in l:
        if not is_useful(automaton,node):
            automaton.remove_state(node)
#[/Q7]

prune(A)
A.export("A_prune")

prune(B)
B.export("B_prune")