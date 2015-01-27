# -*- encoding: utf-8 -*-

import sys

from crypto_simple.rsa_feistel import RSAKey_FeistelData_Simple

"""
    Scenario: Alice veut envoyer un message à Bob.
    Il se trouve que Bobby a déja généré un couple de clés
    RSA (k_ASYM_pub, k_ASYM_priv) et sa clé publique k_ASYM_pub est
    disponible librement.

        1. Alice choisit le message m_clear qu'elle veut transmettre
        2. Alice récupère la clé publique k_ASYM_pub de Bob
        3. Alice choisit une clé k_SYM pour Feistel
        4. Alice chiffre son message m_clear avec la clé k_SYM : m_SYMcrypt
        5. Alice chiffre sa clé k_SYM avec la clé publique de Bobby k_ASYM_pub : k_SYM_ASYMcrypt
        6. Alice envoie son message chiffré m_SYMcrypt et la clé k_SYM_ASYMcrypt

        7. Bobby recoit le message m_SYMcrypt et la clé k_SYM_ASYMcrypt
        8. Bobby déchiffre la clé k_SYM_ASYMcrypt avec sa clé privée k_ASYM_priv : k_SYM_decrypt (== k_SYM)
        9. Bobby déchiffre le message m_SYMcrypt avec la clé k_SYM_decrypt : m_decrypt (== m_clear)
        10. Bobby peut lire le message m_decrypt
"""

### Initialisation

# Création des clés
Alice = RSAKey_FeistelData_Simple('Alice')
Bobby = RSAKey_FeistelData_Simple('Bobby')

# Echange (sur et sécurisé) des clés publiques
Alice.add_pubkey('Bobby', Bobby.get_pubkey())
Bobby.add_pubkey('Alice', Alice.get_pubkey())

### Conversation

## Message 1

# Chiffrement du message par Alice pour Bobby
A_m_Aclear = "Hello Bob !! How are you ?"
print "Message en clair     : %s" % repr(A_m_Aclear)
A_m_Acrypt = Alice.send_message('Bobby', A_m_Aclear)
print "Message chiffré      : %s" % repr(A_m_Acrypt)

# Echange du message
B_m_Acrypt = A_m_Acrypt

# Déchiffrement du message d'Alice par Bobby
B_m_Adecrypt = Bobby.receive_message(B_m_Acrypt)
print "Message déchiffré    : %s" % repr(B_m_Adecrypt['m'])

## Message 2

# Chiffrement du message par Bobby pour Alice
B_m_Bclear = "Hello Alice !! I'm fine. Have a good day !"
print "Message en clair     : %s" % repr(B_m_Bclear)
B_m_Bcrypt = Bobby.send_message('Alice', B_m_Bclear)
print "Message chiffré      : %s" % repr(B_m_Bcrypt)

# Echange du message
A_m_Bcrypt = B_m_Bcrypt

# Déchiffrement du message d'Alice par Bobby
A_m_Bdecrypt = Alice.receive_message(A_m_Bcrypt)
print "Message déchiffré    : %s" % repr(A_m_Bdecrypt['m'])
