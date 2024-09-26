from thlr_regex import *

#[Q7]
def regex_to_string(regex):
    if regex.root == ".":
        return "(" + regex_to_string(regex.children[0]) + "." + regex_to_string(regex.children[1]) + ")"
    if regex.root == "+":
        return "(" + regex_to_string(regex.children[0]) + "+" + regex_to_string(regex.children[1]) + ")"
    if regex.root == "*":
        return "(" + regex_to_string(regex.children[0]) + ")*"
    return regex.root
#[/Q7]
