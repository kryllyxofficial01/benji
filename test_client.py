import socket

host = "127.0.0.1"
port = int(input("port: "))

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((host, port))

try:
    message = "cpu_all;"
    print(f"Sending: {message}")
    client_socket.send(message.encode())

    response = client_socket.recv(1024)
    print(f"Server response: {response.decode()}")
finally:
    client_socket.close()
