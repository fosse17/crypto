# -*- encoding: utf-8 -*-

import sys

from primeutils import is_prime, find_prime, bezout, get_generator_ZpZ, pgcd, egcd

import pdb

"""
Etant donné que l'on va chiffrer byte par byte, il faut qu le mininum des nombres
premiers possibles pour la clé privée soit supérieur à ceil(sqrt(255)).
Cela vient du fait que l'on ne peut pas chiffrer de données plus grandes que
le modulo n = p*q.
Pour la forme, nous choisissons 100.
"""

PRIME_MIN = 10**2
PRIME_MAX = 10**4

class RSA_Simple:
    """
        Module permettant de générer sa propre clé privée
        de partager sa clé publique et de stocker les
        publiques des autres.

        Pour chiffrer un message, il faut avoir la clé publique du
        destinataire et fournir son nom.

        Pour déchiffrer un message, il faut etre le propriétaire de
        la clé privée correspondant à la clé publique qui a permis
        de chiffrer le message.
    """

    pubkeys = {}

    def __init__(self, owner=''):
        self.p, self.q = self._rsa_compute_privkey()
        self.e, self.n, self.phi = self._rsa_compute_pubkey(self.p, self.q)
        self.d = self._compute_d(self.e, self.phi)

        self.owner = owner

        self.log("---- Init : begin ----")
        self.log("Private key:")
        self.log("p= %d q= %d" %(self.p, self.q))
        self.log("Public key:")
        self.log("e= %d n= %d" %(self.e, self.n))
        self.log("---- Init : end   ----")


    def log(self, line):
        sys.stdout.write("[%s](RSA) %s\n" %(self.owner, line))

    def _rsa_compute_privkey(self):
        p = q = 1
        while p == q:
            p, q = find_prime(PRIME_MIN, PRIME_MAX), find_prime(PRIME_MIN, PRIME_MAX)
        return p, q

    def _rsa_compute_pubkey(self, p, q):
        phi = (p - 1) * (q - 1)
        #e, n = get_generator_ZpZ(phi), p*q
        e, n = self._compute_e(phi), p*q
        return e, n, phi

    def _compute_d(self, e, phi):
        r, u, v = egcd(e, phi)
        if u < 0:
            return phi + u
        else:
            return u

    def _compute_e(self, phi):
        e = phi - 1
        while pgcd(e, phi) != 1:
            e -= 1
        return e

    def add_pubkey(self, name, pubkey):
        assert(type(name) == str)
        assert(len(pubkey) == 2 and type(pubkey['e']) in (int, long) and type(pubkey['n']) in (int, long))
        self.pubkeys[name] = pubkey
        self.log("Receives pubkey from %s" % name)

    def get_pubkey(self):
        self.log("Send pubkey")
        return {'e': self.e, 'n': self.n}

    def _encipher_byte(self, name, ci):
        assert(type(ci) in (int, long))
        #assert(ci < 0x100)
        #return (ci ** self.pubkeys[name]['e']) % self.pubkeys[name]['n']
        return pow(ci, self.pubkeys[name]['e'], self.pubkeys[name]['n'])

    def _decipher_byte(self, ci):
        assert(type(ci) in (int, long))
        #assert(ci < 0x100)
        #return (ci ** self.d) % self.n
        return pow(ci, self.d, self.n)

    def encipher_string(self, name, data):
        assert(type(name) == str)
        assert(type(data) == str)
        return ''.join(hex(self._encipher_byte(name, ord(c))) for c in data)

    def decipher_string(self, data):
        assert(type(data) == str)
        #pdb.set_trace()
        data = data.split('0x')
        #map(lambda c: if '0x'+c, data)
        data.pop(0)
        return ''.join(chr(self._decipher_byte(long(c, 16))) for c in data)

