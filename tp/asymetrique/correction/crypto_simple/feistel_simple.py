# -*- encoding: utf-8 -*-

import sys

import hashlib
import binascii

import pdb

"""
S4 Crypto mars 2014
===================
S4B
JOSSEAUME Julien
ROUX Pierre


Réseaux de Feistel :
    - implémenté avec pour chaque round : une sous-clé différente.
    - les sous-clés sont générées à partir d'une clé générale.
    - padding du message avec des 0 si la longueur du message n'est pas
    multiple de 64 byte.
    - l'unité n'est pas le bit, mais le byte (soit un char), par conséquent les
    blocs ne sont sont pas de longueur 64 bits = 2 * 32 bits, mais plutôt de
    longueur 64 bytes = 2 * 32 bytes = 2 * 32 * 8 bits = 2 * 256 bits = 512 bits
    A noter que 64 bytes ne correspond pas forcément à 64 charactères en clairs
    suivant l'utilisation d'un encodage (accents, cédille, ...)

    Les sous-blocs font 256 bits, d'où la génération des sous-clés par sha256.

"""


def num2passwd(num):
    passwd = []
    while num != 0:
        passwd += [num & 0xff]
        num >>= 8
    passwd.reverse()
    return ''.join(chr(inum) for inum in passwd)

def passwd2num(passwd):
    num = 0
    for c in passwd:
        num += ord(c) & 0xff
        num <<= 8
    num >>= 8
    return num

class Feistel_Simple:
    nbrounds = 7
    blocklength = 64
    scalp = blocklength / 2

    def __init__(self, passwd, nbrounds=nbrounds, owner=''):
        if type(passwd) == int or type(passwd) == long:
            passwd = num2passwd(passwd)
        assert(type(passwd) == str)
        self.subkeys = [hashlib.sha512(passwd + str(i*i)).digest() for i in range(nbrounds)]
        self.nbrounds = nbrounds
        self.owner = owner
        self.log("passwd : %s" % repr(passwd))

    def log(self, line):
        if self.owner:
            sys.stdout.write("[%s](Feistel) %s\n" %(self.owner, line))

    def _data_to_chunks(self, data, n):
        return [data[i:i + n] for i in xrange(0, len(data), n)]

    def _chunks_to_data(self, chunks, n):
        return [subitem for subdata in chunks for subitem in subdata]

    #def _bin_to_ascii(self, databin):
    #    return binascii.unhexlify("%x" % int(databin, 2))

    #def _data_to_bin(self, data):
    #    return bin(int(binascii.hexlify(data), 16))  # weird

    def _data_to_string(self, data):
        return ''.join(chr(idata) for idata in data)

    def _string_to_data(self, string):
        return [ord(c) for c in string]

    def encipher_string(self, input_text):

        # Transformation, padding et vérification de la longueur
        input_data = self._string_to_data(input_text)
        nprefill = len(input_data) % self.blocklength
        #print "input_data (before len=%d) :\n %s" % (len(input_data), input_data)
        if nprefill:
            #print "nprefill :", nprefill
            input_data += [0x00] * (self.blocklength - nprefill)
        #print "input_data (after len=%d) :\n %s" % (len(input_data), input_data)
        assert((len(input_data) % self.blocklength) == 0)

        # Chriffrement
        output_data = []
        i = 0
        for datablock in self._data_to_chunks(input_data, self.blocklength):
            output_data += [self._encipher(datablock)]
            #print "[%d] new chunk (len=%d):\n %s" % (i, len(self._encipher(datablock)) ,output_data)
            i += 1

        # Applatissement
        #print "output_data (before):\n %s" % output_data
        output_data = self._chunks_to_data(output_data, self.blocklength)
        #print "output_data (after):\n %s" % output_data

        # Reconstruction
        output_string = self._data_to_string(output_data)
        #print "output_string: %s" % output_string
        return output_string


    def decipher_string(self, input_text):

        # Transformation et vérification de la longueur
        input_data = [ord(c) for c in input_text]
        assert((len(input_data) % self.blocklength) == 0)

        # Déchiffrement
        output_data = []
        for datablock in self._data_to_chunks(input_data, self.blocklength):
            output_data += [self._decipher(datablock)]

        # Enlèvement du padding
        output_data = self._chunks_to_data(output_data, self.blocklength)
        while output_data[-1] == 0x00:
            del output_data[-1]

        #print "output data: %s" % output_data

        # Reconstruction
        output_string = ''.join(chr(x) for x in output_data)
        #print "output_string: %s" % output_string
        return output_string

    def _encipher(self, data):
        assert(len(data) == 64)
        block1 = data[:self.scalp]
        block2 = data[self.scalp:]
        for sk in self.subkeys:
            block1, block2 = self.round(sk, block1, block2)
        #print "end cipher : ", block1+block2
        return block1 + block2

    def _decipher(self, data):
        assert(len(data) == 64)
        block1 = data[:self.scalp]
        block2 = data[self.scalp:]
        for sk in reversed(self.subkeys):
            block2, block1 = self.round(sk, block2, block1)
        return block1 + block2

    def round(self, subkey, block1, block2):
        """ Ronde de Feistel avec sous-clé """
        subkey = map(ord, subkey)
        return block2, [d ^ p for d, p in zip(block1, subkey)]


if __name__ == '__main__':

    def usage():
        sys.stderr.write("""Usage: %s
            -c <passwd> <input-clear-file> <output-enc-file> : to encypher a file
            -d <passwd> <input-enc-file> <output-clear-file> : to decypher a file
\n""" % sys.argv[0])
        sys.exit(-1)

    if len(sys.argv) != 5:
        usage()

    f = Feistel_Simple(sys.argv[2])

    with open(sys.argv[3], 'rb') as input_file:
        input_string = input_file.read()

    if sys.argv[1] == '-c':
        output_string = f.encipher_string(input_string)
    elif sys.argv[1] == '-d':
        output_string = f.decipher_string(input_string)
    else:
        usage()

    with open(sys.argv[4], 'wb') as output_file:
        output_file.write(output_string)

    sys.exit(0)
