from .utils import RoundConst, Sbox, string_to_grid
import time


class AESKey:

    def __init__(self, key) -> None:

        if len(key) <= 16:
            round = 10
        elif len(key) > 16 and len(key) <= 24:
            round = 12
        elif len(key) > 24:
            round = 14

        self.N_ROUND = round
        self.key_string = key[0:16].ljust(16, '\0')     
        self.key_matrix = string_to_grid(self.key_string)
        self.round_key = [ self.key_matrix ]

        print(f"Round: {self.N_ROUND}")
        start = time.time()
        
        for n_round in range(1, self.N_ROUND + 1):
            new_key = self.generate_round_key(n_round)
            self.round_key.append(new_key)
        
        end = time.time()
        self.time_key = (end - start)



    def g_func(self, word, n_round):
        new_word = word[1:] + word[:1]                                          #   Circular left shift
        new_word = [ Sbox[each] for each in new_word ]                          #   Substitute
        new_word = [ new_word[i] ^ RoundConst[n_round][i] for i in range(4) ]   #   Add Round Const
        return new_word


    def generate_round_key(self, n_round):

        old_0 = [ self.round_key[n_round-1][i][0] for i in range(4) ]
        old_1 = [ self.round_key[n_round-1][i][1] for i in range(4) ]
        old_2 = [ self.round_key[n_round-1][i][2] for i in range(4) ]
        old_3 = [ self.round_key[n_round-1][i][3] for i in range(4) ]
        g_old_3 = self.g_func(old_3, n_round)

        new_1 = [ old_0[i] ^ g_old_3[i] for i in range(4) ]
        new_2 = [ old_1[i] ^ new_1[i] for i in range(4) ]
        new_3 = [ old_2[i] ^ new_2[i] for i in range(4) ]
        new_4 = [ old_3[i] ^ new_3[i] for i in range(4) ]

        return [
            [ new_1[0], new_2[0], new_3[0], new_4[0] ],
            [ new_1[1], new_2[1], new_3[1], new_4[1] ],
            [ new_1[2], new_2[2], new_3[2], new_4[2] ],
            [ new_1[3], new_2[3], new_3[3], new_4[3] ],
        ]

    