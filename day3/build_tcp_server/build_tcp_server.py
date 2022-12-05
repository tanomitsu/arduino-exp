#! usr/bin/env python
# coding: utf-8
# tcp_server

import socket
import threading

bind_ip = '0.0.0.0'
bind_port = 10036 # 割り当てられたポート
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((bind_ip, bind_port))
server.listen(5)
print(f"[*] listen {bind_ip}: {bind_port}")

def handle_client(client_socket):
    bufsize = 1024
    request = client_socket.recv(bufsize)
    print(f"[*] recv: {request}")
    client_socket.send("Hey Client!\n")
    client_socket.close()

while True:
    client, addr = server.accept()
    print(f"[*] connected from: {addr[0]}: {addr[1]}")
    client_handler = threading.Thread(target=handle_client, args=(client,))
    client_handler.start()