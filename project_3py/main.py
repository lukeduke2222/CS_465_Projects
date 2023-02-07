from math import ceil
import hashlib
import os
from random import randint
from itertools import zip_longest



def split_bytes_in_blocks(x, blocksize):
    nb_blocks = ceil(len(x)/blocksize)
    return [x[blocksize*i:blocksize*(i+1)] for i in range(nb_blocks)]

def sha1(msg, state=None, message_added_length=None):
    MAX_WORD = 0xFFFFFFFF

    def S(X, n):
        'circular left shift (a.k.a "rotate left")'
        return ((X << n) | (X >> (32 - n))) & MAX_WORD

    if message_added_length == None:
        padded_msg = sha1_padding(msg)
    else:
        forced_msg_byte_length = len(msg) + message_added_length
        padded_msg = sha1_padding(msg, forced_msg_byte_length)

    words = [int.from_bytes(w, byteorder='big')
             for w in split_bytes_in_blocks(padded_msg, 4)]

    n = len(words) / 16
    assert n.is_integer()
    n = int(n)

    M = split_bytes_in_blocks(words, 16)

    def f(t, B, C, D):
        if 0 <= t <= 19:
            return (B & C) | ((~B) & D)
        elif 20 <= t <= 39 or 60 <= t <= 79:
            return B ^ C ^ D
        elif 40 <= t <= 59:
            return (B & C) | (B & D) | (C & D)
        else:
            raise Exception('t must be between 0 and 79 inclusive')

    def K(t):
        if 0 <= t <= 19:
            return 0x5A827999
        elif 20 <= t <= 39:
            return 0x6ED9EBA1
        elif 40 <= t <= 59:
            return 0x8F1BBCDC
        elif 60 <= t <= 79:
            return 0xCA62C1D6
        else:
            raise Exception('t must be between 0 and 79 inclusive')

    if state == None:
        H0 = 0x67452301
        H1 = 0xEFCDAB89
        H2 = 0x98BADCFE
        H3 = 0x10325476
        H4 = 0xC3D2E1F0
    else:
        # SHA-1 state cloning
        assert isinstance(state, tuple)
        assert len(state) == 5
        assert all(isinstance(x, int) for x in state)

        H0, H1, H2, H3, H4 = state

    for i in range(len(M)):
        W = M[i]
        assert len(W) == 16

        for t in range(16, 80):
            W.append(S(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16],
                       n=1))

        A, B, C, D, E = H0, H1, H2, H3, H4

        for t in range(80):
            TEMP = (S(A, 5) + f(t, B, C, D) + E + W[t] + K(t)) & MAX_WORD

            E = D
            D = C
            C = S(B, 30)
            B = A
            A = TEMP

        H0 = (H0 + A) & MAX_WORD
        H1 = (H1 + B) & MAX_WORD
        H2 = (H2 + C) & MAX_WORD
        H3 = (H3 + D) & MAX_WORD
        H4 = (H4 + E) & MAX_WORD

    result = b''.join(H.to_bytes(4, byteorder='big') for H in [H0, H1, H2, H3, H4])

    return result


def secret_prefix_sha1_mac(msg, key):
    return sha1(key + msg)

def sha1_padding(msg, forced_msg_byte_length=None):

    assert isinstance(msg, bytes)

    # message length in bits
    if forced_msg_byte_length == None:
        msg_length = len(msg)*8
    else:
        msg_length = forced_msg_byte_length*8

    m = -(msg_length + 1 + 64) % 512

    padded_msg = (msg
                  + bytes([0b10000000])
                  + b'\x00'*(m//8)
                  + msg_length.to_bytes(8, byteorder='big')
                 )

    return padded_msg


original_msg = (b"No one has completed lab 2 so give them all a 0")

# hexmac = 0xe384efadf26767a613162142b5ef0efbb9d7659a
mac = bytes.fromhex('e384efadf26767a613162142b5ef0efbb9d7659a')
print(mac.hex())

H0, H1, H2, H3, H4 = [int.from_bytes(x, byteorder='big')
                      for x in split_bytes_in_blocks(mac, blocksize=4)]

keysize = 16

glue_padding = sha1_padding(os.urandom(keysize) + original_msg)[keysize + len(original_msg):]

message_added_length = keysize + len(original_msg) + len(glue_padding)
extra_text = b': give Luke Albin an A'
forged_mac = sha1(extra_text, (H0, H1, H2, H3, H4), message_added_length)

forged_msg = original_msg + glue_padding + extra_text

print("forged message:",forged_msg.hex())
print("forged mac:",forged_mac.hex())

