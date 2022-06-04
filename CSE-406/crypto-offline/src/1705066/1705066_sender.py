from aes.aes import AES
from rsa.rsa import RSA
import socket, json, os


SECRET_DIR = "Don't Open This"
HOST = "127.0.0.1"
PORT = 32000
CHUNK_SIZE = 4096


def main():

    print("=============================")
    print("======     Sender     =======")
    print("=============================")
    
    plaintext = input("Enter plaintext: ")
    aes_key = input("Enter key: ")

    k = 128
    rsa = RSA()
    aes = AES(aes_key)

    rsa_key = rsa.generate_key(k)
    e = rsa_key.get("e")
    n = rsa_key.get("n")

    if not os.path.exists(SECRET_DIR):
        os.makedirs(SECRET_DIR)

    secret = open(SECRET_DIR + "/secret.json", "w")
    secret.write(json.dumps(rsa_key))
    secret.close()
    print("RSA private key stored in file")
    print(rsa_key, end='\n\n')

    ciphertext = aes.encrypt(plaintext)

    aes_enc_key = rsa.encrypt(aes_key, e, n)
    print("AES Encrypted Key:", aes_enc_key)

    data = {
        "CT": ciphertext,
        "EK": aes_enc_key,
        "PUK": {
            "e": e,
            "n": n
        },
    }

    data = json.dumps(data).encode()

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((HOST, PORT))

    sock.sendall(data)
    data = sock.recv(CHUNK_SIZE)

    if data.decode('utf-8') == "ACK":
        # check the local file
        plainfile = open(SECRET_DIR + "/plain.txt", "r")
        decrypted_text = plainfile.read()
        plainfile.close()

        if plaintext == decrypted_text:
            print("\n** Sent and received messages are same! **")
        else:
            print("\n** Sent and received messages aren't same! **")



if __name__ == "__main__":
    main()