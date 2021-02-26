# TDP015 Programming Assignment 1
# Logic
# Skeleton Code

import itertools


class Exp(object):
    """A Boolean expression.

    A Boolean expression is represented in terms of a *reserved symbol* (a
    string) and a list of *subexpressions* (instances of the class `Exp`).
    The reserved symbol is a unique name for the specific type of
    expression that an instance of the class represents. For example, the
    constant `True` uses the reserved symbol `1`, and logical and uses `∧`
    (the Unicode symbol for conjunction). The reserved symbol for a
    variable is its name, such as `x` or `y`.

    Attributes:
        sym: The reserved symbol of the expression (a string).
        sexps: The list of subexpressions (instances of the class `Exp`).
    """

    def __init__(self, sym, *sexps):
        """Constructs a new expression.

        Args:
            sym: The reserved symbol for this expression.
            sexps: The list of subexpressions.
        """
        self.sym = sym
        self.sexps = sexps

    def value(self, assignment):
        """Returns the value of this expression under the specified truth
        assignment.

        Args:
            assignment: A truth assignment, represented as a dictionary
            that maps variable names to truth values.

        Returns:
            The value of this expression under the specified truth
            assignment: either `True` or `False`.
        """
        raise ValueError()

    def variables(self):
        """Returns the (names of the) variables in this expression.

        Returns:
           The names of the variables in this expression, as a set.
        """
        variables = set()
        for sexp in self.sexps:
            variables |= sexp.variables()
        return variables


class Var(Exp):
    """A variable."""

    def __init__(self, sym):
        super().__init__(sym)

    def value(self, assignment):
        assert len(self.sexps) == 0
        return assignment[self.sym]

    def variables(self):
        assert len(self.sexps) == 0
        return {self.sym}


class Nega(Exp):
    """Logical not."""

    def __init__(self, sexp1):
        super().__init__('¬', sexp1)

    def value(self, assignment):
        assert len(self.sexps) == 1
        return not self.sexps[0].value(assignment)

    # TODO: Complete this class

class Conj(Exp):
    """Logical and."""

    def __init__(self, sexp1, sexp2):
        super().__init__('∧', sexp1, sexp2)

    def value(self, assignment):
        assert len(self.sexps) == 2
        return \
            self.sexps[0].value(assignment) and \
            self.sexps[1].value(assignment)


class Disj(Exp):
    """Logical or."""

    def __init__(self, sexp1, sexp2):
        super().__init__('∨', sexp1, sexp2)

    def value(self, assignment):
        assert len(self.sexps) == 2
        return \
            self.sexps[0].value(assignment) or \
            self.sexps[1].value(assignment)

    # TODO: Complete this class


class Impl(Exp):
    """Logical implication."""

    def __init__(self, sexp1, sexp2):
        super().__init__('→', sexp1, sexp2)
    # TODO: Complete this class

    def value(self, assignment):
        assert len(self.sexps) == 2

        if self.sexps[0].value(assignment) == False and\
            self.sexps[1].value(assignment) == True:
            return False
        else:
            return True


class Equi(Exp):
    """Logical equivalence."""

    def __init__(self, sexp1, sexp2):
        super().__init__('↔', sexp1, sexp2)

    def value(self, assignment):
        assert len(self.sexps) == 2
        return \
            self.sexps[0].value(assignment) == \
            self.sexps[1].value(assignment)


def assignments(variables):
    """Yields all truth assignments to the specified variables.

    Args:
        variables: A set of variable names.

    Yields:
        All truth assignments to the specified variables. A truth
        assignment is represented as a dictionary mapping variable names to
        truth values. Example:

        {'x': True, 'y': False}
    """
    output = []

    test = itertools.product(range(2), repeat=len(variables))
    for permutation in test:
        temp = {}
        for id,val in enumerate(variables):
            temp["{}".format(val)] = permutation[id]
        yield temp



    # TODO: Complete this function. Use the itertools module!


def satisfiable(exp):
    """Tests whether the specified expression is satisfiable.

    An expression is satisfiable if there is a truth assignment to its
    variables that makes the expression evaluate to true.

    Args:
        exp: A Boolean expression.

    Returns:
        A truth assignment that makes the specified expression evaluate to
        true, or False in case there does not exist such an assignment.
        A truth assignment is represented as a dictionary mapping variable
        names to truth values.
    """
    # TODO: Complete this function
    assignment_list = assignments(exp.variables())
    for assignment in assignment_list:
        if exp.value(assignment) == True:
            return True
    return False


def tautology(exp):
    """Tests whether the specified expression is a tautology.

    An expression is a tautology if it evaluates to true under all
    truth assignments to its variables.

    Args:
        exp: A Boolean expression.

    Returns:
        True if the specified expression is a tautology, False otherwise.
    """
    # TODO: Complete this function
    assignment_list = assignments(exp.variables())
    for assignment in assignment_list:
        if not exp.value(assignment):
            return False
    return True

def equivalent(exp1, exp2):
    """Tests whether the specified expressions are equivalent.

    Two expressions are equivalent if they have the same truth value under
    each truth assignment.

    Args:
        exp1: A Boolean expression.
        exp2: A Boolean expression.

    Returns:
        True if the specified expressions are equivalent, False otherwise.
    """
    assignment_list = assignments(exp2.variables().union(exp1.variables()))

    for assignment in assignment_list:
        if exp1.value(assignment) != exp2.value(assignment):
            return False
    return True
        # TODO: Complete this function


def test1():
    """
    Test equivalent function. Returns false.
    """
    a = Var('a')
    b = Var('b')
    c = Var('c')
    exp1 = Impl(Impl(a, b), c)
    exp2 = Conj(Disj(a, c), Disj(Nega(b), c))
    return equivalent(exp1, exp2)

def test2():
    """
    Test satisfiable function. Returns true.
    """
    a = Var('a')
    exp = Disj(a, Nega(a))
    return satisfiable(exp)

def test3():
    """
    Test Tautology function. Returns true.
    """
    a = Var('a')
    exp = Equi(a, a)
    return tautology(exp)

def test4():
    """
    Test Tautology function. Returns true.
    """
    a = Var('a')
    exp = Equi(a, Disj(a, a))
    return tautology(exp)

def test5():
    """
    Test equivalent function. Returns false.
    """
    p = Var('p')
    q = Var('q')
    r = Var('r')

    exp1 = Conj(p, Nega(q))
    exp2 = Disj(Nega(r), Conj(Nega(p), q))
    return equivalent(exp1, exp2)

if __name__ == "__main__":
    print("Test 1")
    print(test1())
    print("{}\nTest2".format("-"*10))
    print(test2())
    print("{}\nTest3".format("-"*10))
    print(test3())
    print("{}\nTest4".format("-"*10))
    print(test4())
    print("{}\nTest5".format("-"*10))
    print(test5())
    # TODO: Add some more test cases
