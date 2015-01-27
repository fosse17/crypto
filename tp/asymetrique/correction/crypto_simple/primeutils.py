# -*- encoding: utf-8 -*-
import sys
import math
import random

import pdb


def new_int(low=10**3, high=10**12):
    return random.randint(low, high)

def is_prime(num):
    for j in xrange(2, int(math.sqrt(num))+2):
        if (num % j) == 0:
            return False
    return True


def find_prime(low=10 ** 3, high=10 ** 6):
    p = new_int(low, high)
    while not is_prime(p):
        #print "%d is not prime" % p
        p = new_int(low, high)
    return p


def bezout(a, b):
    a0, b0 = a, b

    p, q = 1, 0
    r, s = 0, 1

    while b != 0:
        c = a % b
        #  division entière
        quotient = a / b
        a = b
        b = c
        r_new = p - quotient * r
        s_new = q - quotient * s
        p = r
        q = s
        r, s = r_new, s_new

    print "pgcd(%d, %d) = (%d) * %d + (%d) * %d = %d" % (a0, b0, p, a0, q, b0, a)
    return a, p, q

def egcd(a, b):
    x, y, u, v = 0, 1, 1, 0
    while a != 0:
        q, r = b//a, b%a
        m, n = x-u*q, y-v*q
        b,a, x,y, u,v = a,r, u,v, m,n
    gcd = b
    return gcd, x, y


def pgcd(a, b):
    p, q = 1, 0
    r, s = 0, 1
    while b != 0:
        c = a % b
        #  division entière
        quotient = a / b
        a = b
        b = c
        r_new = p - quotient * r
        s_new = q - quotient * s
        p = r
        q = s
        r, s = r_new, s_new
    return abs(a)


def get_generator_ZpZ(p):
    """"
    i = 2
    g = p ** i

    #pdb.set_trace()
    while pgcd(p, (g**i) % p) != 1:
        i += 1
        #print i, "...",
        #g = p ** i
        #print g, "ok"
    return i
    """
    g = new_int() % (p/2)
    r, u, v = bezout(p, g)
    while r != 1:
        g += 1
        r, u, v = bezout(p, g)

    return g


