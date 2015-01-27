# -*- encoding: utf-8 -*-

import sys

from crypto_simple.rsa_simple import RSA_Simple


### Initialisation

# Création des clés
Alice_RSA = RSA_Simple('Alice')
Bobby_RSA = RSA_Simple('Bobby')

# Echange (sur et sécurisé) des clés publiques
Alice_RSA.add_pubkey('Bobby', Bobby_RSA.get_pubkey())
Bobby_RSA.add_pubkey('Alice', Alice_RSA.get_pubkey())

### Conversation

# Chiffrement du message par Alice pour Bobby
A_m_Aclear = (100, 165, 239, 2, 107256, 78, 254, 255, 256, 257, 258, 259, 300, 301, 450, Bobby_RSA.n-1, Bobby_RSA.n, Bobby_RSA.n+1, Bobby_RSA.n * 3)
print "Message en clair     : %s" % repr(A_m_Aclear)
A_m_Acrypt = [Alice_RSA._encipher_byte('Bobby', m) for m in A_m_Aclear]
print "Message chiffré      : %s" % repr(A_m_Acrypt)

# Echange du message
B_m_Acrypt = A_m_Acrypt

# Déchiffrement du message d'Alice par Bobby
B_m_Adecrypt = [Bobby_RSA._decipher_byte(m) for m in B_m_Acrypt]
print "Message déchiffré    : %s" % repr(B_m_Adecrypt)

"""
Remarque:
=========

Il est normal qu'il y ait des erreurs pour les messages supérieurs à PRIME_MIN.

"""

