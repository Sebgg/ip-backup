# TDP015 Programming Assignment 3
# Number Theory
# Skeleton Code

import random

# In this assignment you are asked to implement a simple version of
# the RSA cryptosystem.
#
# https://en.wikipedia.org/wiki/RSA_(cryptosystem)

# ## Problem 1
#
# At its core, the RSA cryptosystem deals with integers. In order to
# encrypt and decrypt strings, we first need to convert them into
# numbers. To do that we first encode each string into a byte array,
# and then convert each byte into an integer. Here is an example:
#
# String: 'foo'
# Byte array: b'foo'
# Integers: [102, 111, 111]
#
# In real implementations of RSA, a single integer corresponds to a
# block of bytes. Here is an example where we encode blocks of 2 bytes
# instead of single bytes. In order for this to work, we need to pad
# the original byte string with a zero byte:
#
# String: 'foo'
# Byte array: b'foo'
# Byte array, padded: b'foo\x00'
# Integers: [26223, 28416]
#
# To encode a string into a byte array, use
# https://docs.python.org/3/library/stdtypes.html#str.encode
#
# To decode a byte array into a string, use
# https://docs.python.org/3/library/stdtypes.html#str.decode
#
# To convert a byte array to an integer, use
# https://docs.python.org/3/library/stdtypes.html#int.from_bytes
#
# To convert an integer into a byte array, use
# https://docs.python.org/3/library/stdtypes.html#int.to_bytes


def text2ints(text, m):
    """Encode a string into a list of integers.

    Args:
        text: A string.
        m: The size of a block in bytes.

    Returns:
        A list of integers.

    """

    text = text.encode()

    while len(text)%m != 0 and m != 1:
        text += b'\x00'

    out = []
    for index in range(0, len(text), m):
        out.append(int.from_bytes(text[index:index+m], byteorder='big'))

    return out

def ints2text(ints, m):
    """Decode a list of integers into a string.

    Args:
        ints: A list of integers.
        m: The size of a block in bytes.

    Returns:
        A string.

    """

    out = ""
    for number in ints:
        temp = number.to_bytes(m, byteorder = 'big')
        temp.rstrip(b'\x00')
        out += temp.decode()

    return out

# ## Problem 2
#
# Your next task is to implement the Euclidean algorithm for computing
# the greatest common divisor (gcd) of two integers `a` and `b`. You
# will actually start by implementing an extended version of this
# algorithm that computes not only the gcd but also a pair of
# so-called Bezout coefficients. These are integers `x` and `y`
# satisfying the equation
#
# ax + by = gcd(a, b)
#
# The extended Euclidean algorithm is described here:
# https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm
#
# Once you have implemented the extended Euclidean algorithm,
# implementing the standard algorithm is trivial.


def xgcd(a, b):
    """Computes the greatest common divisor (gcd) and a pair of Bezout
    coefficients for the specified integers.

    Args:
        a: An integer.
        b: An integer.

    Returns:
        A triple `(g, x, y)` where `g = gcd(a, b)` and `x`, `y` are
        Bezout coefficients for `a` and `b`.

    Algorithm taken from: https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm
    """
    prevx, x = 1, 0;  prevy, y = 0, 1
    while b:
        q, r = divmod(a,b)
        x, prevx = prevx - q*x, x
        y, prevy = prevy - q*y, y
        a, b = b, r
    return a, prevx, prevy
    """
    s = 0
    t = 1
    r = b
    x = 1
    y = 0
    g = a

    while r > 0:
        quotient = g // r
        (g, r) = (r, g - quotient * r)
        (x, s) = (s, x - quotient * s)
        (y, t) = (t, y - quotient * t)

    return g, x, y
"""
#print(xgcd())


def gcd(a, b):
    """Computes the greatest common divisor (gcd) of the specified
    integers.

    Args:
        a: An integer.
        b: An integer.

    Returns:
        The greatest common divisor of the specified integers.

    Algorithm based on Marcos algorithm for gcd
    """

    return a if b == 0 else gcd(b, a % b)


# ## Problem 3
#
# Your next task is to implement a function that generates an RSA key
# pair from a pair of two prime numbers, `p` and `q`. A key pair
# consists of a public key `(e, n)` (`e` stands for "encrypt") and a
# private key `(d, n)` (`d` stands for "decrypt").  Use the following
# recipe (which is slightly simplified from the real RSA cryptosystem):
#
# 1. Compute n = p * q
# 2. Compute phi = (p-1) * (q-1)
# 3. Choose 1 < e < phi such that e and phi are coprime.
# 4. Determine d as the modular multiplicative inverse of e modulo phi.
#
# For step 3, use a random number generator and the Euclidean
# algorithm from Problem 2 to generate numbers in the relevant range
# until you find a number that satisfies the criterion.
#
# For step 4, note that the modular multiplicative inverse of e modulo
# phi is simply the Bezout coefficient for e, modulo phi. This means
# that you can use the extended Euclidean algorithm from Problem 2.

def generate_keypair(p, q):
    """Generate an RSA key pair.

    An RSA key pair consists of a public key `(e, n)` and a private
    key `(d, n)`.

    Args:
        p: A prime number.
        q: A prime number, distinct from `p`.

    Returns:
        An RSA keypair.

    """
    n = p * q

    phi = (p-1)*(q-1)

    e = random.randint(2, phi-1)
    while gcd(e, phi) != 1:
        e = random.randint(2, phi-1)

    temp = xgcd(e, phi)
    d = max(temp[1], temp[2])
    return (e, n), (d, n)

# ## Problem 4
#
# Implement functions for encryption and decryption. The encryption
# and decryption of a single integer i is very simple:
#
# Encryption: i^e % n
# Decryption: i^d % n
#
# (You can implement this efficiently using the `pow()` function.)
#
# How to choose the block size? On the one hand, it should be as large
# as possible. On the other hand, both encryption and decryption are
# modulo n, so one cannot use block sizes that yield integers larger
# than that value. Therefore, as the block size we should choose the
# largest number of bytes b such that 2^(8*b)-1 < n. To give a
# concrete example, if n >= 256 then any integer with 1 byte (maximal
# value 255) will fit into n, but if n < 65536 then not every integer
# with 2 bytes (maximal value 65535) will fit into n -- so for n
# within this range, the block size should be 1.


def encrypt(pubkey, plaintext):
    """Encrypt a plaintext message using a public key.

    Args:
        pubkey: A key.
        plaintext: A plaintext message (a string).

    Returns:
        A ciphertext message (a list of integers).

    """
    m = 0
    while pow(2,8*(m+1))-1 < pubkey[1]:
        m += 1

    converted = text2ints(plaintext, m)
    for i in range(len(converted)):
        converted[i] = pow(converted[i], pubkey[0], pubkey[1])
    return converted

def decrypt(seckey, ciphertext):
    """Decrypt a ciphertext message using a secret key.

    Args:
        seckey: A key.
        ciphertext: A ciphertext message (a list of integers).

    Returns:
        A plaintext message (a string).

    """
    m = 0
    while pow(2, 8*(m+1))-1 < seckey[1]:
        m += 1

    converted = [pow(i, seckey[0], seckey[1]) for i in ciphertext]
    out = ints2text(converted, m)

    return out


# To test your implementation, you can use the following code, which
# will allow you to generate a key pair and encrypt messages.


if __name__ == "__main__":
    p = int(input("Enter prime number p: "))
    q = int(input("Enter prime number q: "))
    print("Generating keypair")
    pubkey, seckey = generate_keypair(p, q)
    print("The public key is", pubkey, "and the private key is", seckey)
    message = input("Enter a message to encrypt with the public key: ")
    encrypted_msg = encrypt(pubkey, message)
    print("The encrypted message is: ", end="")
    print(" ".join(map(lambda x: str(x), encrypted_msg)))
    print("The decrypted message is: ", end="")
    print(decrypt(seckey, encrypted_msg))
    print(decrypt((91307, 268483), [259114, 14038, 13667, 74062, 148955, 50062, 36907, 18603, 93303, 170481, 7991]))

# As a further test, we have generated a ciphertext for you. With a
# working implementation, you should be able to decode that ciphertext
# using the secret key (91307, 268483).
#
# 259114 14038 13667 74062 148955 50062 36907 18603 93303 170481 7991
