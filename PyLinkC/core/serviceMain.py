#!/usr/bin/python3
#!encoding='utf-8'

import sys
import _thread
sys.path.append("..")
from protocol.gurgle import *
from codecs import decode, encode

def serviceMain(_Socket , _Addr):
    addr = _Addr
    core = gurgle(gurgle.GURGLE_SERVER)
    core.set_socket(_Socket)
    core.set_remote_host(addr[0])
    core.set_remote_port(addr[1]);
    while True:
        buf = core.recv(1024)
        if buf is None:
            _thread.exit()
        data = json.loads(json.loads(decode(buf)))

        senddata = json.dumps('{"id":"%d", "version":"%s"}' % (core.create_id(),core.get_version()))    ## !!!!
        if core.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
            _thread.exit()
        if(data['version'] != core.get_version()):
            sys.stderr.write("Protocol's version is not the same!")
            _thread.exit()
    _thread.exit()


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
