import socket
import threading


BUFFER_SIZE = 1024

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(('127.0.0.1', 8080))
print('<Connected to the server>')

def receive():
    while True:
        try:
            message = client.recv(BUFFER_SIZE).decode()
            
            print(f'Server response: sum is  {message}')
        except:
            print("An error occuered")
            client.close()
            break

def send():
    while True:
        message = input()
        if message == 'quit':
            client.close()
            break

        client.send(message.encode())

receive_thread = threading.Thread(target=receive)
receive_thread.start()

sending_thread = threading.Thread(target=send)
sending_thread.start()