#!/usr/bin/python3
#!encoding='utf-8'

import sys
import _thread
sys.path.append("..")
from protocol.gurgle import *
from codecs import decode, encode
passwordAllowed = [ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                    '!', '@', '#', '$', '%', '^', '&', '*', '-', '_', '=', '<', '>',
                    '+', '?', '/', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0']

usernameAllowed = [ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                    '+', '-', '_', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0']

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
            core.write_log("Data without ID",gurgle.GURGLE_LOG_MODE_ERROR)
            core.emergency_quit('SyntaxError','This package has no ID! Emergency quit!')
            _thread.exit()
        if 'version' in data:
            senddata = json.dumps('{"id":"%d", "version":"%s"}'
                    %(int(data['id']),core.get_version()))
            if core.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
                _thread.exit()
            if(data['version'] != core.get_version()):
                core.write_log("Protocol's version is not the same!"
                        ,gurgle.GURGLE_LOG_MODE_ERROR)
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
                    core.write_log('Query without params,ID = [%d] will be droped'%data['id']
                            ,gurgle.GURGLE_LOG_MODE_ERROR)
                    core.emergency_quit('SyntaxError','Query without params')
                    _thread.exit()
                if 'query' in data['params']:
                    if data['params']['query'] == 'auth_method':
                        senddata = json.dumps('{"id":"%d", "params":{"answer":"%s"}}'
                                %(int(data['id']),core.get_auth_method()))
                    else:   #end if of [query]
                        core.emergency_quit('UnknownQuery',
                            "Query[%s] isn't supported"%data['params']['query'])
                        core.write_log("Such query[%s] isn't supported"%data['params']['query']
                                ,gurgle.GURGLE_LOG_MODE_ERROR)
                        _thread.exit()
                else:   #end if of [params]
                    core.write_log("Such params[%s] isn't supported"%data['params']
                            ,gurgle.GURGLE_LOG_MODE_ERROR)
                    core.emergency_quit('UnknownParams',
                            "Params[%s] isn't supported"%data['params'])
                    _thread.exit()
            elif data['cmd'] == 'auth':
                if not 'from' in data:
                    core.write_log("Auth without from",gurgle.GURGLE_LOG_MODE_ERROR)
                    core.emergency_quit('SyntaxError','Auth without the from field')
                    _thread.exit()
                FullGurgleID    = data['from']
            elif data['cmd'] == 'quit':
                core.disconnect_from_remote()
                _thread.exit()
            else:   # end if of [cmd]
                core.write_log("Such cmd[%s] isn't supported"%data['cmd']
                        ,gurgle.GURGLE_LOG_MODE_ERROR)
                core.emergency_quit('UnknownCmd', "Cmd[%s] isn't supported"%data['cmd'])
                _thread.exit()
        if core.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
            core.emergency_quit()
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
