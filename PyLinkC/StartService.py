#!/usr/bin/python3
#!encoding='utf-8'

import socket
import time
import codecs

from protocol.Gurgle import *

if __name__ == '__main__':
    SERVICE_PORT = 40097
    ADDR = ('',SERVICE_PORT)
    sevSocket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    sevSocket.bind(ADDR)
    sevSocket.listen(5)
    while True:
        clientSocket,addr = sevSocket.accept()
        print ("Connected by", addr)
