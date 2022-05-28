from aes.aes import AES


def main():

    print("Plaintext:")
    plain_text = input()
    print(plain_text.encode('utf-8').hex(), " (hex)\n")

    print("Key:")
    key = input()
    print(key.encode('utf-8').hex(), " (hex)\n")

    aes = AES(key)
    cipher_text = aes.encrypt(plain_text)

    print("Ciphertext:")
    print(cipher_text)
    print(cipher_text.encode('utf-8').hex(), " (hex)\n")

    # Transmission
    cipher_copy = cipher_text
    # Complete

    decrypted_text = aes.decrypt(cipher_copy)
    print("Deciphered text:")
    print(decrypted_text)
    print(decrypted_text.encode('utf-8').hex(), " (hex)\n")

    aes.print_time()

    assert plain_text == decrypted_text, "Input Output doesn't match!"


if __name__ == "__main__":
    main()