#!/usr/bin/python3
#!encoding='utf-8'

import socket
import _thread      # its ugly do u think so?
import time
import codecs

if __name__ == '__main__':
    SERVICE_PORT = 40097
    ADDR = ('',SERVICE_PORT)
    sevSocket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    sevSocket.bind(ADDR)
    sevSocket.listen(5)
    while True:
        print ("Waiting for connection......")
        clientSocket,addr = sevSocket.accept()
        print ("Connected by", addr)
        while True:
            data = clientSocket.recv(1024)
            if not data:
                break
            print (time.ctime(),':',codecs.decode(data,'utf-8'))
