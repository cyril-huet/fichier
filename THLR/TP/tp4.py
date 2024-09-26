from thlr_automata import *
from thlr_regex import *

r1 = new_regex("a*.b+c")
r2 = new_regex("(a+0)*.c")

A = ENFA([0,1],[0],[1],["a","b","c"],[])
B = ENFA([0,1],[0],[1],["a","b","c"],[])


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
    convert_regex(res,0,1,regex)
    return res
#[/Q3]

T = to_enfa(r1)
# convert_regex(B,0,1,r2)
T.export("T")
# B.export("B")



#[Q4]
def get_epsilon_closure(enfa, origin):
    liste = []
    epsilon = ''
    for state in enfa.all_states:
        if len(enfa.next_states[(state, epsilon)]) > 0:
            qqch = enfa.next_states[(state, epsilon)]
            qqch = list(qqch)
            for node in qqch:
                if node != list(enfa.final_states)[0]:
                    liste.append(node)
    liste.append(origin)
    return set(liste)
#[/Q4]

print(get_epsilon_closure(T,0))

#[Q5]
def to_nfa(enfa):
    return NFA(enfa,T.initial_states,T.final_states,T.alphabet,T.next_states)
#[/Q5]

T2 = to_nfa(T)
T2.export("T2")