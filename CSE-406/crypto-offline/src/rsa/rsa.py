from Crypto.Util import number
from BitVector import *
import time


class RSA:

    def __init__(self) -> None:
        self.time_keygen = 0
        self.time_enc = 0
        self.time_dec = 0


    def get_primes(self, k):
        num = BitVector(intVal=(2**k - 1))
        primes = []

        while len(primes) < 2:
            if num.test_for_primality() > 0.99:
                primes.append(num.intValue())
            num = BitVector(intVal=num.intValue() - 1)
        
        return primes


    def generate_key(self, k: int):
        start = time.time()

        p, q = self.get_primes(int(k/2))
        n = p * q

        phi = (p-1) * (q-1)
        e = 65537
        d = number.inverse(e, phi)

        end = time.time()
        self.time_keygen = (end - start)

        return {
            "e": e,
            "d": d,
            "n": n
        }


    def encrypt_char(self, char, e, n):
        char = ord(char)
        return pow(char, e, n)


    def decrypt_char(self, val, d, n):
        val = pow(val, d, n)
        return chr(val)


    def encrypt(self, message, e, n):
        start = time.time()
        ret = [ self.encrypt_char(each, e, n) for each in message ]
        end = time.time()
        self.time_enc = (end - start)
        return ret


    def decrypt(self, cipher_list, d, n):
        start = time.time()
        cipher_list = [ self.decrypt_char(each, d, n) for each in cipher_list ]
        end = time.time()
        self.time_dec = (end - start)
        return "".join(cipher_list)


    def print_time(self):
        print("\nExecution Time:")
        print(f"Key Generation: {self.time_keygen} seconds")
        print(f"Encryption: {self.time_enc} seconds")
        print(f"Decryption: {self.time_dec} seconds\n")




def main():
    message = "CanTheyDoTheirFest??"

    rsa = RSA()
    keys = rsa.generate_key(16)
    print(keys)

    cipher_list = rsa.encrypt(message, keys.get("e"), keys.get("n"))
    print(cipher_list)

    decrypted = rsa.decrypt(cipher_list, keys.get("d"), keys.get("n"))
    print(decrypted)

    rsa.print_time()



if __name__ == "__main__":
    main()