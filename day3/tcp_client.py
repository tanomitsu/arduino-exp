#! /usr/bin/env python
# coding: utf-8
# tcp_client

import socket

target_url = '0.0.0.0'
target_port = 10036

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((target_url, target_port))

s.send("Hello Server!!!\n")
response = s.recv(4096)

print(response)