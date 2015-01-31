#!/usr/bin/python3
#!encoding='utf-8'

import sys
import _thread
sys.path.append("..")
from protocol.gurgle import *

def serviceMain(_Socket , _Addr):
    Addr = _Addr
    Socket = _Socket
    core = gurgle()
    core.set_remote_host(_Addr[0])
    core.set_remote_port(_Addr[1]);
    print ('Connect from ',  _Addr)
    while True:
        pass

if __name__ == '__main__':
    SERVICE_PORT = 40097
    ADDR = ('',SERVICE_PORT)
    sevSocket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    sevSocket.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
    sevSocket.bind(ADDR)
    sevSocket.listen(5)
    while True:
        clientSocket,addr = sevSocket.accept()
        _thread.start_new_thread(serviceMain,(clientSocket,addr))
