from thlr_automata import *
from thlr_regex import *

#[Q2]
def convert_regex(enfa, origin, destination,regex):
    if regex.children==[]:
        enfa.add_letter(regex.root)
        enfa.add_edge(origin,regex.root,destination)
    else:
        if(regex.root=="*"):
            temp0 = enfa.add_state()
            temp1 = enfa.add_state()
            enfa.add_edge(origin,"",temp0)
            enfa.add_edge(temp1,"",destination)
            enfa.add_edge(origin,"",destination)
            enfa.add_edge(temp1,"",temp0)
            convert_regex(enfa,temp0,temp1,regex.children[0])
        elif(regex.root=="+"):
            # création des états
            temp0 = enfa.add_state()
            temp1 = enfa.add_state()
            temp2 = enfa.add_state()
            temp3 = enfa.add_state()
            enfa.add_edge(origin,"",temp0)
            enfa.add_edge(temp1,"",destination)
            enfa.add_edge(origin,"",temp2)
            enfa.add_edge(temp3,"",destination)
            convert_regex(enfa,temp0,temp1,regex.children[0])
            convert_regex(enfa,temp2,temp3,regex.children[1])
        else:   # cas où on a une concaténation
            temp0 = enfa.add_state()
            temp1 = enfa.add_state()
            enfa.add_edge(temp0,"",temp1)
            convert_regex(enfa,origin,temp0,regex.children[0])
            convert_regex(enfa,temp1,destination,regex.children[1])


#[/Q2]

#[Q3]
def to_enfa(regex):
    res = ENFA([0,1],[0],[1],[],[])
    convert_regex(res, list(res.initial_states)[0], list(res.final_states)[0], regex)
    return res
#[/Q3]

#[Q4]
def get_epsilon_closure(enfa, origin):
    incoming = {origin}
    visited = set()
    while incoming != set():
        q = incoming.pop()
        visited.add(q)
        succ = set()
        succ = succ.union(enfa.get_successors(q, ''))
        for c in succ :
            if c not in visited:
                    incoming.add(c)
    return visited
#[/Q4]

#[Q5]
def to_nfa(enfa):
    res = NFA(enfa.all_states, enfa.initial_states, enfa.final_states, enfa.alphabet, [])
    for node in enfa.all_states:
        eps_closure = list(get_epsilon_closure(enfa, node))
        for letter in enfa.alphabet:
            for epsilon in eps_closure:
                if epsilon in enfa.final_states:
                    res.final_states.add(node)
                eps = list(enfa.next_states[(epsilon, letter)])
                if len(eps) > 0 and letter != '':
                    for elt in eps:
                        res.add_edge(node, letter, elt)
#[/Q5]