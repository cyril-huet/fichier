#[Q9]
def union(E, F):
    res = set()
    for x in E:
        res.add(x)
    for y in F:
        res.add(y)
    return res
#[/Q9]

#[Q10]
def intersection(E, F):
    res = set()
    for x in E:
        if x in F:
            res.add(x)
    return res
#[/Q10]

#[Q11]
def subtraction(E, F):
    res = set()
    for x in E:
        if x not in F:
            res.add(x)
    return res
#[/Q11]

#[Q12]
def diff(E, F):
    res = E
    for x in F:
        if x in res:
            res.remove(x)
        else:
            res.add(x)
    return res
#[/Q12]

#[Q13]
def sublists(l):
    if l == []:
        return [[]]
    else:
        subs = sublists(l[1:])
        res = subs.copy()
        for x in subs:
            res.append([l[0]]+x)
        return res
#[/Q13]

#[Q14]
def power_set(E):
    res = []
    for x in sublists(list(E)):
        res.append(set(x))
    return res
#[/Q14]
