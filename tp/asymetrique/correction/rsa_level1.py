# -*- encoding: utf-8 -*-

from crypto_simple.primeutils import is_prime, find_prime, bezout, get_generator_ZpZ, pgcd, egcd

import pdb


PRIME_MIN = 10**1
PRIME_MAX = 10**2

def compute_d(e, phi):
    r, u, v = egcd(e, phi)
    #ed = u * phin + 1
    #d = ed / e
    #return d
    if u < 0:
        return phi + u
    else:
        return u


def compute_e(phi):
    e = phi - 1
    while pgcd(e, phi) != 1:
        e -= 1
    return e


def rsa_encipher(data, e, n):
    return (int(data) ** e) % n


def rsa_decipher(data, d, n):
    return (int(data) ** d) % n


# Message :
m1_clr = 100
m2_clr = 165

"""
# Clé privée
p, q = 11, 29
# Clé publique :
e, n = 11, 319
"""

# Clé privée :
p, q = find_prime(PRIME_MIN, PRIME_MAX), find_prime(PRIME_MIN, PRIME_MAX)
# Fonction indicatrice euler
phi = (p - 1) * (q - 1)
# Clé publique :
#e, n = get_generator_ZpZ(phi), p*q
e, n = compute_e(phi), p*q


print "Clé publique:"
print "e=", e, "n=", n
print "Clé privée:"
print "p=", p, "q=", q

m1_enc = rsa_encipher(m1_clr, e, n)
print "chiffrement de %d = %d" % (m1_clr, m1_enc)
m2_enc = rsa_encipher(m2_clr, e, n)
print "chiffrement de %d = %d" % (m2_clr, m2_enc)

d = compute_d(e, phi)
print "calcul de d = %d" % d

m1_dec = rsa_decipher(m1_enc, d, n)
print "déchiffrement de %d = %d" % (m1_enc, m1_dec)
m2_dec = rsa_decipher(m2_enc, d, n)
print "déchiffrement de %d = %d" % (m2_enc, m2_dec)
