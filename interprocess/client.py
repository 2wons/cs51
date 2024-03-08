import socket
import threading


nickname = input("Enter your nickname: ")

BUFFER_SIZE = 1024

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(('127.0.0.1', 8080))

def receive():
    while True:
        try:
            message = client.recv(BUFFER_SIZE).decode()
            """ if message == 'NICK':
                client.send(nickname.encode('ascii'))
            elif message == 'ROOM':
                client.send(room.encode('ascii'))
            elif nickname in message:
                pass
            else: """
            print(f'Server response: {message}')
        except:
            print("An error occuered")
            client.close()
            break

def send():
    while True:
        message = f"{nickname}: {input('')}"

        if message.lower().split(':')[1] == 'quit':
            break

        client.send(message.encode('ascii'))

receive_thread = threading.Thread(target=receive)
receive_thread.start()

sending_thread = threading.Thread(target=send)
sending_thread.start()