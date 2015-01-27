# -*- encoding: utf-8 -*-

from crypto_simple.feistel_simple import Feistel_Simple
from crypto_simple.primeutils import new_int, is_prime, find_prime, bezout, pgcd, get_generator_ZpZ

import pdb

##################################
# Diffie Helman
##################################


def dh_init():
    p = find_prime()
    print "p=", p
    g = get_generator_ZpZ(p)
    print "g=", g
    return p, g

def dh_choose_skey(p, g):
    a = new_int(1, 10**5)
    return a, (g ** a) % p

def dh_compute_final_key(dh_params):
    a = dh_params['mykey']
    p = dh_params['p']
    gbp = dh_params['olkey']
    dh_params['finalkey'] = (gbp ** a) % p

#pdb.set_trace()
# Paramètres communs
p, g = dh_init()
# Choix d'Alice :
a, gap = dh_choose_skey(p, g)
Alice_params =  {'p': p, 'g' : g, 'mykey' : a, 'mlkey': gap, 'olkey': None, 'finalkey': None }
# Choix de Bob :
b, gbp = dh_choose_skey(p, g)
Bob_params =    {'p': p, 'g' : g, 'mykey' : b, 'mlkey': gbp, 'olkey': None, 'finalkey': None}
# Echanges des clés
Alice_params['olkey'] = Bob_params['mlkey']
Bob_params['olkey'] = Alice_params['mlkey']
# Calcul de la clé finale
#Alice_params['finalkey'] = Alice_params['olkey'] ** Alice_params['mykey'] % Alice_params['p']
#Bob_params['finalkey'] = Bob_params['olkey'] ** Bob_params['mykey'] % Bob_params['p']
dh_compute_final_key(Alice_params)
dh_compute_final_key(Bob_params)

assert(Alice_params['finalkey'] == Bob_params['finalkey'])
print "Alice : ", Alice_params
print "Bob   : ", Bob_params

Alice_base = Feistel_Simple(Alice_params['finalkey'], owner='Alice')
Bob_base = Feistel_Simple(Bob_params['finalkey'], owner='Bobby')

## A cause du padding implémenté dans feistel,
## les caractères \x00 en fin de message vont etre supprimés
## Pour éviter cela, il faudrait envoyer la taille du message.

A_text_clear = "Hello Bob !! How are you ? 1234\x0050\x000\x00\x00\x00"
A_text_crypt = Alice_base.encipher_string(A_text_clear)
print "Alice (clear) write: %s" % repr(A_text_clear)
print "Alice (crypt) send : %s" % repr(A_text_crypt)
print "Alice (crypt) send : %s" % A_text_crypt

B_text_crypt = A_text_crypt

B_text_clear = Bob_base.decipher_string(B_text_crypt)
print "Bob (crypt) receive: %s" % B_text_crypt
print "Bob (crypt) receive: %s" % repr(B_text_crypt)
print "Bob (clear) read   : %s" % repr(B_text_clear)



B_text_clear = "Hello Alice ! I'm fine thank you very much !"
B_text_crypt = Bob_base.encipher_string(B_text_clear)
print "Bob (clear) write: %s" % repr(B_text_clear)
print "Bob (crypt) send : %s" % repr(B_text_crypt)
print "Bob (crypt) send : %s" % B_text_crypt

A_text_crypt = B_text_crypt

A_text_clear = Alice_base.decipher_string(A_text_crypt)
print "Alice (crypt) receive: %s" % A_text_crypt
print "Alice (crypt) receive: %s" % repr(A_text_crypt)
print "Alice (clear) read   : %s" % repr(A_text_clear)
