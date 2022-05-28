from .key import *
from .utils import *

import time


class AES:

    def __init__(self, key) -> None:
        self.aes_key = AESKey(key)
        self.time_enc = 0
        self.time_dec = 0


    def encrypt_block(self, message):
        block = string_to_grid(message)
        
        # round 0
        block = xor_grid(block, self.aes_key.round_key[0])

        # round 1-9
        for round in range(1, 10):
            block = sub_bytes(block)
            block = shift_left(block)
            block = mix_columns(block)
            block = xor_grid(block, self.aes_key.round_key[round])

        # round 10
        block = sub_bytes(block)
        block = shift_left(block)
        block = xor_grid(block, self.aes_key.round_key[10])

        return block


    def encrypt(self, message):        
        start = time.time()

        block_list = [ message[i:i+16] for i in range(0, len(message), 16) ]
        if len(block_list[-1]) != 16:
            block_list[-1] = block_list[-1].ljust(16, '\0')

        ret = [ self.encrypt_block(block) for block in block_list ]
        ret = "".join([ grid_to_string(each) for each in ret ])

        end = time.time()
        self.time_enc = (end - start)
        return ret


    def decrypt(self, cipher):
        start = time.time()

        block_list = [ cipher[i:i+16] for i in range(0, len(cipher), 16) ]        
        block_list = [ string_to_grid(each) for each in block_list ]
        block_list = [ self.decrypt_block(block) for block in block_list ]

        text = "".join([ grid_to_string(each) for each in block_list ])
        text = text.rstrip('\x00')

        end = time.time()
        self.time_dec = (end - start)
        return text


    def decrypt_block(self, cipher):

        # round 0 : key 10
        block = xor_grid(cipher, self.aes_key.round_key[10])
        block = shift_right(block)
        block = inv_sub_bytes(block)

        # round 1-9 : key 9->1
        for round in range(1, 10):
            block = xor_grid(block, self.aes_key.round_key[10-round])
            block = inv_mix_columns(block)
            block = shift_right(block)
            block = inv_sub_bytes(block)
        
        # round 10 : key 0
        block = xor_grid(block, self.aes_key.round_key[0])

        return block


    def print_time(self):
        print("Execution Time:")
        print(f"Key scheduling: {self.aes_key.time_key} seconds")
        print(f"Encryption time: {self.time_enc} seconds")
        print(f"Decryption time: {self.time_dec} seconds")
