from rsa.rsa import RSA


def main():

    print("=============================")
    print("======   Main - RSA   =======")
    print("=============================")

    plaintext = input("Enter message: ")

    key_len = [ 16, 32, 64, 128 ]
    rsa = RSA()

    for k in key_len:

        key = rsa.generate_key(k)
        e = key.get("e")
        d = key.get("d")
        n = key.get("n")

        print(f"K = {k}")
        print(f"Key: {key}")

        ciphertext = rsa.encrypt(plaintext, e, n)
        decryptext = rsa.decrypt(ciphertext, d, n)
        
        if plaintext == decryptext:
            print("** Input and Output are same! **")
        
        rsa.print_time()
        print("---")


if __name__ == "__main__":
    main()