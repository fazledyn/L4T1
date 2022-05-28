from aes.aes import AES
from rsa.rsa import RSA
import json, socket


SECRET_DIR = "Don't Open This"
HOST = "127.0.0.1"
PORT = 32000
CHUNK_SIZE = 4096


def main():
    
    print("=============================")
    print("======    Receiver    =======")
    print("=============================")

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((HOST, PORT))
    sock.listen()
    conn, addr = sock.accept()

    with conn:
        # print(f"Connected by {conn}")
        while True:
            data = conn.recv(CHUNK_SIZE)
            
            if data:
                data = json.loads(data)
                print(f"Received Data: {data}\n")

                secret = open(SECRET_DIR + "/secret.json", "r")
                rsa_key = json.loads(secret.read())
                secret.close()

                print("Got RSA key from file")
                print(rsa_key, end='\n\n')

                aes_enc_key = data.get("EK")
                ciphertext = data.get("CT")
                print("AES Encrypted Key:", aes_enc_key, "\n")

                rsa = RSA()
                d = rsa_key.get("d")
                n = rsa_key.get("n")

                aes_key = rsa.decrypt(aes_enc_key, d, n)
                print("AES Decrypted Key:", aes_key)

                aes = AES(aes_key)
                plaintext = aes.decrypt(ciphertext)
                print("Decrypted message:", plaintext)

                plainfile = open(SECRET_DIR + "/plain.txt", "w")
                plainfile.write(plaintext)
                plainfile.close()

                conn.sendall(b"ACK")
            
            if not data:
                break


if __name__ == "__main__":
    main()