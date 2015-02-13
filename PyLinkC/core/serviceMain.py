#!/usr/bin/python3
#!encoding='utf-8'

import sys
import _thread
sys.path.append("..")
from protocol.gurgle import *
from codecs import decode, encode

def serviceMain(_Socket , _Addr):
    addr = _Addr
    s_socket = _Socket
    core = gurgle(gurgle.GURGLE_SERVER)
    core.set_remote_host(addr[0])
    core.set_remote_port(addr[1]);
    print ('Connect from ',  addr)
    #while True:
    try:
        buf = s_socket.recv(1024)
    except socket.error as e:
        print('Error receiving data: %s' % e)
        _thread.exit()
    if not len(buf):
        _thread.exit()
    data = json.loads(json.loads(decode(buf)))

    senddata = json.dumps('{"id":"%d", "version":"%s"}' % (self.create_id(),self.get_version()))    ## !!!!
    try:
        self.socket.send(encode(senddata))
    except socket.error as e:
        print('Error sending data:%s' %e)
    if(data['version'] != core.get_version()):
        print ("Protocol's version is not the same!")
        _thread.exit()
    else:
        print ("Protocol's version is the same!")
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
