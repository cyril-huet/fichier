# -*-coding:UTF-8 -*
from graphviz import Digraph


# Non-deterministic finite automata with epsilon transitions
class ENFA:

    def __init__(self, all_states, initial_states, final_states, alphabet,
                 edges, states_labels=None):

        # States: a set of integers
        self.all_states = set(all_states)
        # The alphabet: a set of strings
        # "" stands for epsilon
        self.alphabet = set(alphabet)
        self.alphabet.add("")
        # Initial and final states: two sets of integers
        self.initial_states = set(initial_states).intersection(self.all_states)
        self.final_states = set(final_states).intersection(self.all_states)
        # There must be an initial state; if there isn't, an initial state 0
        # is added
        if not self.initial_states:
            self.initial_states.add(0)
            self.all_states.add(0)
        # Labels: a dictionnary of string labels for states
        if states_labels == None:
            # If it's not defined, each state is labelled by default by its own
            # index.
            self.labels = {state: str(state) for state in self.all_states}
        else:
            # Otherwise, use the informations provided by the dictionnary
            self.labels = {}
            for state in self.all_states:
                if state in states_labels:
                    self.labels[state] = states_labels[state]
                else:
                    self.labels[state] = str(state)
        # Edges: a dictionnary (origin, letter): set of destinations
        self.next_states = {(state, letter): set()
                            for state in self.all_states
                            for letter in self.alphabet}
        for edge in set(edges):
            if (edge[0] in self.all_states) and (edge[2] in self.all_states) \
                    and (edge[1] in self.alphabet):
                self.next_states[(edge[0], edge[1])].add(edge[2])

    # Adds a new state to the automaton, and labels it with label.
    def add_state(self, label=None):
        add_state = max(self.all_states) + 1
        self.all_states.add(add_state)
        for letter in self.alphabet:
            self.next_states[(add_state, letter)] = set()
        if label == None:
            self.labels[add_state] = str(add_state)
        else:
            self.labels[add_state] = label
        return add_state

    # Removes the state 'state' from the automaton if it exists.
    def remove_state(self, state):
        if state in self.all_states:
            self.all_states.remove(state)
            if state in self.initial_states:
                self.initial_states.remove(state)
            if state in self.final_states:
                self.final_states.remove(state)
            del self.labels[state]
            # Remove the successors of 'state', but also all the edges
            # leading to 'state'
            for letter in self.alphabet:
                del self.next_states[(state, letter)]
                for origin in self.all_states:
                    if state in self.next_states[(origin, letter)]:
                        self.next_states[(origin, letter)].remove(state)

    # Adds a new letter 'letter' to the automaton.
    def add_letter(self, letter):
        if letter == "ε":
            raise KeyError("The letter ε should be represented "
                           + "by the empty string in the alphabet.")
        if letter == "":
            raise KeyError("The empty string cannot be added (again?) "
                           + "to the alphabet.")
        if not letter in self.alphabet:
            for state in self.all_states:
                self.next_states[state, letter] = set()
            self.alphabet.add(letter)

    # Adds a new edge from the state 'origin' to the state 'destination'
    # labelled by 'letter' to the automaton, if possible.
    def add_edge(self, origin, letter, destination):
        if letter in self.alphabet:
            if origin in self.all_states:
                if destination in self.all_states:
                    self.next_states[(origin, letter)].add(destination)
                else:
                    raise KeyError("The destination " + str(destination)
                                   + " isn't a known state.")
            else:
                raise KeyError("The origin " + str(origin)
                               + " isn't a known state.")
        else:
            raise KeyError("The letter " + letter
                           + " doesn't belong to the alphabet.")

    # Returns the number of edges in the automaton.
    def count_edges(self):
        edges_number = 0
        for state in self.all_states:
            for letter in self.alphabet:
                edges_number += len(self.next_states[(state, letter)])
        return edges_number

    # Returns the set of all states reachable from the state 'origin'
    # with an edge labelled by 'letter', if possible.
    def get_successors(self, origin, letter):
        if not letter in self.alphabet:
            if letter == "":
                letter = "ε"
            raise KeyError("The letter " + letter +
                           " doesn't belong to the alphabet.")
        if not origin in self.all_states:
            raise KeyError("The origin " + str(origin)
                           + " isn't a known state.")
        return self.next_states[(origin, letter)]

    # Exports a picture of the automaton to 'file'.pdf.
    def export(self, file):
        graph = Digraph(filename=file)
        graph.graph_attr['rankdir'] = 'LR'
        for state in self.all_states:
            node_shape = 'circle'
            if state in self.final_states:
                node_shape = 'doublecircle'
            graph.attr('node', shape=node_shape, style='filled',
                       fillcolor='white')
            graph.node(str(state), label=self.labels[state])
            if state in self.initial_states:
                graph.node(str(state) + "_i", style="invisible")
                graph.edge(str(state) + "_i", str(state))
        for state in self.all_states:
            for letter in self.alphabet:
                for next_state in self.next_states[(state, letter)]:
                    if letter == "":
                        edge_colour = 'gray'
                        edge_label = "ε"
                    else:
                        edge_colour = 'black'
                        edge_label = letter
                    graph.edge(str(state), str(next_state), label=edge_label,
                               color=edge_colour)
        graph.render()


# Non-deterministic finite automaton
class NFA(ENFA):

    def __init__(self, all_states, initial_states, final_states, alphabet,
                 edges, states_labels=None):
        # States: a set of integers
        self.all_states = set(all_states)
        # The alphabet: a set of strings
        # "" stands for epsilon and isn't allowed.
        self.alphabet = set(alphabet)
        if "" in self.alphabet:
            self.alphabet.remove("")
        # Initial and final states: two sets of integers
        self.initial_states = set(initial_states).intersection(self.all_states)
        self.final_states = set(final_states).intersection(self.all_states)
        # There must be an initial state; if there isn't, an initial state 0
        # is added.
        if not self.initial_states:
            self.initial_states.add(0)
            self.all_states.add(0)
        # Labels: a dictionnary of string labels for states
        if states_labels == None:
            # If it's not defined, each state is labelled by default by its own
            # index.
            self.labels = {state: str(state) for state in self.all_states}
        else:
            # Otherwise, use the informations provided by the dictionnary
            self.labels = dict()
            for state in self.all_states:
                if state in states_labels:
                    self.labels[state] = states_labels[state]
                else:
                    self.labels[state] = str(state)
        # Edges: a dictionnary (origin, letter): set of destinations
        self.next_states = {(state, letter): set()
                            for state in self.all_states
                            for letter in self.alphabet}
        for edge in set(edges):
            if (edge[0] in self.all_states) and (edge[2] in self.all_states) \
                    and (edge[1] in self.alphabet):
                self.next_states[(edge[0], edge[1])].add(edge[2])


# Returns a NFA labelled by integers equivalent to the input NFA whose states
# and edges are labelled by generic objects; the various arguments are similar
# to the arguments of the constructor NFA.
def symbolic_nfa_builder(all_states, initial_states, final_states, alphabet,
                         edges):
    real_all_states = range(0, len(all_states))
    real_initial_states = [all_states.index(initial_state) for initial_state
                           in initial_states]
    real_final_states = [all_states.index(final_state) for final_state
                         in final_states]
    real_edges = []
    state_labels = {all_states.index(state): str(state)
                    for state in all_states}
    for edge in edges:
        real_edges.append((all_states.index(edge[0]), edge[1],
                           all_states.index(edge[2])))
    return NFA(real_all_states, real_initial_states, real_final_states,
               alphabet, real_edges, state_labels)
