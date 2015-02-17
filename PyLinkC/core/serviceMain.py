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
        if not 'id' in data:
            sys.stderr.write("Data without ID")
            sendata = json.dumps('{"id":"0", "cmd":"kill", "error":"%s", "reason":"%s"}'
                    %'SyntaxError',
                        "This package has no ID! Emergency quit!")
            core.send(encode(senddata))
            _thread.exit()
        if 'version' in data:
            senddata = json.dumps('{"id":"%d", "version":"%s"}'%(int(data['id']),core.get_version()))
            if core.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
                _thread.exit()
            if(data['version'] != core.get_version()):
                sys.stderr.write("Protocol's version is not the same!")
                _thread.exit()
            continue                            # 不论如何这个回合都会结束
        if 'cmd' in data:                           # 命令
            if data['cmd']  == 'ping':              # ping
                if 'payload' in data:
                    senddata = json.dumps('{"id":"%d", "cmd":"pong", "payload":"%s"}'
                            %(core.create_id(),data['payload']))
                else:
                    senddata = json.dumps('{"id":"%d", "cmd":"pong"}'
                            %(core.create_id()))
            elif data['cmd'] == 'query':              # 请求
                if not data['params']:
                    sys.stderr.write('Query without params,ID = [%d] will be droped'%data['id'])
                    sendata = json.dumps('{"id":"%d", "cmd":"kill", "error":"%s", "reason":"%s"}'
                            %(int(data['id']),
                                'SyntaxError',
                                'Query without params'))
                    core.send(encode(senddata))
                    _thread.exit()
                if 'query' in data['params']:
                    if data['params']['query'] == 'auth_method':
                        senddata = json.dumps('{"id":"%d", "params":{"answer":"%s"}}'
                                %(int(data['id']),core.get_auth_method()))
                    else:   #end if of [query]
                        sys.stderr.write("Such query[%s] isn't supported"%data['params']['query'])
                        sendata = json.dumps('{"id":"%d", "cmd":"kill", "error":"%s", "reason":"%s"}'
                                %(int(data['id']),
                                    "UnknownQuery",
                                    "Query[%s] isn't supported"%data['params']['query']))
                        core.send(encode(senddata))
                        _thread.exit()
                else:   #end if of [params]
                    sys.stderr.write("Such params[%s] isn't supported"%data['params'])
                    sendata = json.dumps('{"id":"%d", "cmd":"kill", "error":"%s", "reason":"%s"}'
                            %(int(data['id']),
                                "UnknownParams",
                                "Params[%s] isn't supported"%data['params']))
                    core.send(encode(senddata))
                    _thread.exit()
            elif data['cmd'] == 'auth':
                if not 'from' in data:
                    sys.stderr.write("Auth without from")
                    sendata = json.dumps('{"id":"%d", "cmd":"kill", "error":"%s", "reason":"%s"}'
                            %(int(data['id']),
                                'SyntaxError',
                                'Auth without the from field'))
                    core.send(encode(senddata))
                    _thread.exit()
            else:   # end if of [cmd]
                sys.stderr.write("Such cmd[%s] isn't supported"%data['cmd'])
                sendata = json.dumps('{"id":"%d", "cmd":"kill", "error":"%s", "reason":"%s"}'
                        %(int(data['id']),
                            "UnknownCmd",
                            "Cmd[%s] isn't supported"%data['cmd']))
                core.send(encode(senddata))
                _thread.exit()
        if core.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
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
