# TDP015 Programmeringsuppgift N
# Numeriska metoder
# Skelettkod
from math import isclose
# Er uppgift är att implementera Newtons metod för att approximera
# nollställen till en funktion och att sedan tillämpa denna
# implementation för att lösa tre numeriska beräkningsproblem. En
# beskrivning av metoden hittar du här:
#
# https://sv.wikipedia.org/wiki/Newtons_metod

# ## Del 1

# ### Problem 1
#
# Implementera ett steg av Newton-approximationen.


def newton_one(f, f_prime, x0):
    """Compute the next Newton approximation to a root of the function `f`,
    based on an initial guess `x0`.

    Args:
        f: A float-valued function.
        f_prime: The function's derivative.
        x0: An initial guess for a root of the function.

    Returns:
        The next Newton approximation to a root of the function `f`, based on
        the initial guess `x0`.
    """
    # TODO: Replace the next line with your own code.
    result = x0 - (f(x0)/f_prime(x0))

    return result

# ### Problem 2
#
# Implementera en fullständig Newton-approximation.

def newton(f, f_prime, x0, n_digits=6):
    """Compute the Newton approximation to a root of the function `f`, based
    on an initial guess `x0`.

    Args:
        f: A float-valued function.
        f_prime: The function's derivative.
        x0: An initial guess for a root of the function.
        n_digits: The desired precision of the approximation.

    Returns:
        A pair consisting of the Newton approximation to a root of the
        function `f` and the number of iterations needed to reach numerical
        stability. The precision of the approximation should be the specified
        number `n_digits` of digits after the decimal place. That is, the
        iterative process should end when the approximation does not alter the
        first `n_digits` after the decimal place.
    """
    # TODO: Replace the next line with your own code.
    temp = 0.0
    counter = 0
    result = newton_one(f, f_prime, x0)

    while not isclose(temp, result, abs_tol=10**-n_digits):
        temp = result
        result = newton_one(f, f_prime, result)
        counter += 1

    return result, counter

# ## Del 2

# ### Problem 3
#
# Använd din kod för att approximera nollstället av funktionen
#
# f(x) = x^3 - x + 1
#
# Ange Python-funktioner `f` och `f_prime` och ett lämpligt startvärde `x0`
# och skriv ut ert numeriska resultat inklusive antalet iterationer.
# Noggrannheten ska vara sex siffror efter kommat.


def problem3():
    # TODO: Replace the following lines with your own code.
    f = lambda x: (x**3) - x + 1
    f_prime = lambda x: (2*x**2) - 1
    x0 = 3.0
    a, i = newton(f, f_prime, x0)
    print("approximation: {:.6f}, number of iterations: {}".format(a, i))


# ### Problem 4
#
# Använd er kod för att approximativt beräkna den femte roten ur 5. Börja med
# att ange den funktion vars nollställe ni vill approximera. Fortsätt sedan
# som i föregående uppgift.


def problem4():
    # TODO: Replace the following lines with your own code.
    f = lambda x: x**5 - 5
    f_prime = lambda x: 5*x**4
    x0 = 5
    a, i = newton(f, f_prime, x0)
    print("approximation: {:.6f}, number of iterations: {}".format(a, i))


# ### Problem 5
#
# Ange en funktion och två olika startvärden sådana att Newtons metod räknar
# ut två helt olika approximationer.


def problem5():
    # TODO: Replace the following lines with your own code.
    f = lambda x: x**3 - 7*x**2 + 3*x
    f_prime = lambda x: 3*x**2 - 14*x + 3
    x01 = 2
    x02 = 9
    a1, _ = newton(f, f_prime, x01)
    a2, _ = newton(f, f_prime, x02)
    print("approximation 1: {:.6f}".format(a1))
    print("approximation 2: {:.6f}".format(a2))


if __name__ == "__main__":
    print("Problem 3")
    problem3()

    print("")
    print("Problem 4")
    problem4()

    print("")
    print("Problem 5")
    problem5()
