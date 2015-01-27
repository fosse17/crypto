
import sys
import string
import random

from rsa_simple import RSA_Simple
from feistel_simple import Feistel_Simple


class RSAKey_FeistelData_Simple(RSA_Simple):

    def __init__(self, owner=''):
        RSA_Simple.__init__(self, owner)


    def _get_new_feistel(self, passwd_clear):
        return Feistel_Simple(passwd_clear, owner=self.owner)


    def _generate_passwd(self, n=16):
        return ''.join(random.choice(string.printable) for _ in xrange(n))


    def send_message(self, dst, m_clear):
        assert(type(dst) == str)
        assert(type(m_clear) == str)
        if dst not in self.pubkeys.keys():
            self.log("No public key for %s" % dst)
            sys.exit(-1)

        passwd_clear = self._generate_passwd()
        fsk = self._get_new_feistel(passwd_clear)
        m_crypt = fsk.encipher_string(m_clear)
        passwd_crypt = self.encipher_string(dst, passwd_clear)

        return {'k': passwd_crypt, 'm': m_crypt}


    def receive_message(self, message):
        assert(type(message) == dict)
        m_crypt = message['m']
        passwd_crypt = message['k']
        assert(type(m_crypt) == str)
        assert(type(passwd_crypt) == str)
        passwd_clear = self.decipher_string(passwd_crypt)
        fsk = self._get_new_feistel(passwd_clear)
        m_clear = fsk.decipher_string(m_crypt)

        return {'k': passwd_clear, 'm': m_clear}

