#!/usr/bin/python3
#!encoding='utf-8'

import socket
import _thread
from codecs import decode, encode

if __name__ == '__main__':
    clientSocket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    clientSocket.connect(('127.0.0.1',40097))
    while True:
        data = input("Input a text:")
        clientSocket.send(encode(data))
