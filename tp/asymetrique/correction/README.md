TP CRYPTO S4
=============

    IUT Bordeaux 1 2013/2014
    Julien JOSSEAUME
    Pierre ROUX

NOTES
========

 * crypto_module : module python regroupant les classes pour utiliser
 les méthodes de chiffrement Feistel, RSA, et d'échange de clés
 DiffieHellman et RSA/Feistel.

 * dh_level1.py : cas d'utilisation de Diffie Hellman pour l'échange de
 clés et de Feistel pour le chiffrement.

 * rsa_level1.py : cas théorique d'utilisation de RSA pour chiffrer des valeurs
 numériques. (cf TP)

 * rsa_level2.py : cas pratique d'utilisation de RSA pour chiffrer des valeurs
 numériques. On se contente de l'échange de clés entre 2 parties.
 On remarque que les valeurs qui sont supérieurs au modulo n=p*n ne peuvent pas
 etre chiffrées correctement.

 * rsa_level3.py : cas pratique d'utilisation pour l'échange de clés
 asymétriques (RSA) qui servent à chiffrer la clé symétrique (Feistel) qui
 elle-meme sert à chiffrer/dechiffrer les vraies données.
