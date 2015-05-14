#!/usr/bin/python3
#!encoding='utf-8'

import sys
import _thread
sys.path.append("..")
from protocol.gurgle import *
from database.grgl_mysql import *
from codecs import decode, encode
passwordAllowed = [ 'a','b','c','d','e','f','g','h','i','j','k','l','m',
                    'n','o','p','q','r','s','t','u','v','w','x','y','z',
                    'A','B','C','D','E','F','G','H','I','J','K','L','M',
                    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                    '!','@','#','$','%','^','&','*','-','_','=','<','>',
                    '+','?','/','1','2','3','4','5','6','7','8','9','0']

usernameAllowed = [ 'a','b','c','d','e','f','g','h','i','j','k','l','m',
                    'n','o','p','q','r','s','t','u','v','w','x','y','z',
                    'A','B','C','D','E','F','G','H','I','J','K','L','M',
                    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                    '+','-','_','1','2','3','4','5','6','7','8','9','0']

protocolSupported=[ 'grgl'  ]
def serviceMain(_Socket , _Addr):
    addr = _Addr
    core = gurgle(gurgle.GURGLE_SERVER)
    core.set_socket(_Socket)
    core.set_remote_host(addr[0])
    core.set_remote_port(addr[1]);
    grgl_mysql = grgl_mysql_controllor()
    is_authenticated = "Unauthenticated"
    while True:
        try:
            data = core.recv(1024)
        except gurgle_network_error:
            _thread.exit()
        if not 'id' in data:
            core.write_log("Data without ID",gurgle.GURGLE_LOG_MODE_ERROR)
            core.emergency_quit(    \
                    'SyntaxError',
                    'This package has no ID!'
                )
            _thread.exit()
        request_id = int(data['id'])
        if 'version' in data:
            senddata = json.dumps({
                "id":request_id, 
                "version":core.get_version()
                })
            if core.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
                core.disconnect_from_remote()
                _thread.exit()
            if(data['version'] != core.get_version()):
                core.write_log("Protocol's version is not the same!"
                        ,gurgle.GURGLE_LOG_MODE_ERROR)
            continue                            # 不论如何这个回合都会结束
        if 'cmd' in data:                           # 命令
            if data['cmd']  == 'ping':              # ping
                if 'payload' in data:
                    senddata = json.dumps({
                            "id"     : request_id,
                            "cmd"    : "pong",
                            "payload": data['payload']
                        })
                else:
                    senddata = json.dumps({
                            "id"    : request_id, 
                            "cmd"   : "pong"
                        })
            elif data['cmd'] == 'query':              # 请求
                if not data['params']:
                    core.write_log('Query without params'
                            ,gurgle.GURGLE_LOG_MODE_ERROR)
                    core.emergency_quit(
                            'SyntaxError',
                            'Query without params',
                            request_id
                        )
                    _thread.exit()
                if 'query' in data['params']:
                    if data['params']['query'] == 'auth_method':
                        senddata = json.dumps({
                                "id"    : request_id,
                                "params":{
                                    "answer" : core.get_auth_method()
                                }
                            })
                    elif data['params']['query'] == 'auth_status':
                        senddata = json.dumps({
                                "id"    : request_id,
                                "params": {
                                    "answer" : is_authenticated
                                }
                            })
                    else:   #end if of [query]
                        core.emergency_quit(    \
                                'UnknownQuery',
                                "Query[%s] isn't supported"
                                    %data['params']['query'],
                                 request_id
                            )
                        core.write_log("Such query[%s] isn't supported"
                                %data['params']['query']
                                ,gurgle.GURGLE_LOG_MODE_ERROR)
                        _thread.exit()
                else:   #end if of [params]
                    core.write_log("Such params[%s] isn't supported"
                                %data['params']
                            ,gurgle.GURGLE_LOG_MODE_ERROR)
                    core.emergency_quit(    \
                            'UnknownParams',
                            "Params[%s] isn't supported"
                                %data['params'],
                            request_id
                        )
                    _thread.exit()
            elif data['cmd'] == 'auth':
                if not 'from' in data:
                    core.write_log("Auth without from",
                            gurgle.GURGLE_LOG_MODE_ERROR)
                    core.emergency_quit(    \
                            'SyntaxError',
                            'Auth without the from field',
                            request_id
                        )
                    _thread.exit()
                FullSignInID    = data['from']
                if FullSignInID.find(':') == -1:
                    core.emergency_quit(
                            'SyntaxError',
                            'ID syntax error',
                            request_id
                        )
                    _thread.exit()
                (protocol,ID)   = FullSignInID.split(':',1)
                isProtocolSupported = False
                if protocol == 'grgl':
                    if ID.find('@') == -1:
                        core.emergency_quit(    \
                                'SyntaxError',
                                'ID syntax error',
                                request_id
                            )
                        _thread.exit()
                    (username,suffix) = ID.split('@',1)
                    ch = 'a'
                    t  = 'a'
                    isFound = False
                    for ch in username:
                        isFound = False
                        for a in usernameAllowed:
                            if a == ch:
                                isFound = True
                                break
                        if isFound == False:
                            core.emergency_quit(    \
                                    'SyntaxError',
                                    'ID syntax error',
                                    request_id
                                )
                            _thread.exit()
                    if suffix.find('/') == -1:
                        core.emergency_quit(    \
                                'SyntaxError',
                                'ID syntax error',
                                request_id
                            )
                        _thread.exit()
                    (domain,terminal) = suffix.split('/',1)
                    if 'params' not in data:
                        core.write_log("Auth without params",
                                gurgle.GURGLE_LOG_MODE_ERROR)
                        core.emergency_quit(    \
                                'SyntaxError',
                                'Auth without the params field',
                                request_id
                            )
                        _thread.exit()
                    if 'method' not in data['params']:
                        method = core.get_auth_method()
                    if 'password' not in data['params']:
                        core.write_log("Auth without password"
                                ,gurgle.GURGLE_LOG_MODE_ERROR)
                        core.emergency_quit(    \
                                'SyntaxError',
                                'Auth without the password field',
                                request_id
                            )
                        _thread.exit()
                    password = data['params']['password']
                    if (password == None) or (username == None):
                        core.emergency_quit(    \
                                'SyntaxError',
                                'ID syntax error',
                                request_id
                            )
                        _thread.exit()
                    result = grgl_mysql.authenticate(username,password)
                    if result == grgl_mysql_controllor.AUTH_SUCCESS:
                        senddata = json.dumps({
                                "id"    : request_id,
                                "to"    : FullSignInID,
                                "error" : "null"
                            })
                        is_authenticated = 'Authenticated'
                    elif result == grgl_mysql_controllor.AUTH_INCORRECT:
                        senddata = json.dumps({
                                "id"    : request_id,
                                "to"    : FullSignInID,
                                "error" :"%s"
                                    %"Username or password is incorrect"
                            })
                        is_authenticated = 'Unauthenticated'
                    else:
                        senddata = json.dumps({
                                "id"    : request_id,
                                "to"    : FullSignInID,
                                "error" :"%s"
                               %"Your account has been disabled or deactivated"
                            })
                        is_authenticated = 'Unauthenticated'
                    if not core.send(encode(senddata)):
                        core.disconnect_from_remote()
                        _thread.exit()
                    continue
                core.write_log("Protocol[%s] is not supported yet"
                        %protocol,gurgle.GURGLE_LOG_MODE_ERROR)
                core.emergency_quit(    \
                        "ProtocolUnSupported",
                        "Protocol[%s] is not supported yet"
                            %protocol,
                        request_id
                    )
                _thread.exit()
            elif data['cmd'] == 'quit':
                if 'reason' in data:
                    core.write_log('Client quited because %s'%data['reason'],
                            gurgle.GURGLE_LOG_MODE_DEBUG)
                else:
                    core.write_log('Client quited without reason',
                            gurgle.GURGLE_LOG_MODE_DEBUG)
                senddata = json.dumps({"id":"%d","cmd":"bye"})
                core.send(encode(senddata))
                core.disconnect_from_remote()
                _thread.exit()
            else:   # end if of [cmd]
                core.write_log("Such cmd[%s] isn't supported"%data['cmd']
                        ,gurgle.GURGLE_LOG_MODE_ERROR)
                core.emergency_quit(    \
                        'UnknownCmd',
                        "Cmd[%s] isn't supported"%data['cmd'],
                        request_id
                    )
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
