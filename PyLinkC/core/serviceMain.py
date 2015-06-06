#!/usr/bin/python3
#!encoding='utf-8'

import sys
import _thread
sys.path.append("%s//..//"%sys.path[0])
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
class serviceThread(threading.Thread):
    def __init__(self):
        self.core               = gurgle(gurgle.GURGLE_SERVER)
        self.grgl_mysql         = grgl_mysql_controllor()
        self.is_authenticated   = "Unauthenticated"
        self.senddata           = "something to send"
        self.target             = ""
        self.username           = None
        threading.Thread.__init__(self)
    def setup(self,_Socket, _Addr):
        self.addr               = _Addr
        self.core.set_socket(_Socket)
        self.core.set_remote_host(self.addr[0])
        self.core.set_remote_port(self.addr[1]);
    def run(self):
        request_id         = 0
        while True:
            try:
                data = self.core.recv(1024)
            except gurgle_network_error:
                _thread.exit()
            if not 'id' in data:
                self.core.write_log("Data without ID",gurgle.GURGLE_LOG_MODE_ERROR)
                self.core.emergency_quit('SyntaxError','This package has no ID!')
                _thread.exit()
            request_id = int(data['id'])
            if 'cmd' in data:                           # 
                cmd = str(data['cmd'])
                if cmd  == 'ping':              # ping
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
                elif cmd == 'connect':
                    if 'params' in data:
                        if 'protocol' in data['params']:
                            if str(data['params']['protocol'])!='gurgle':
                                senddata = json.dumps({
                                    "id"    : request_id,
                                    "reply" : {
                                        "status" : "connection failed"
                                    }
                                })
                                self.core.send(encode(senddata))
                                self.core.write_log('Protocol %s is not supported'
                                                %data['params']['protocol']
                                        ,gurgle.GURGLE_LOG_MODE_ERROR)
                                self.core.emergency_quit(                    \
                                        'ProtocolError',                \
                                        'Protocol is not supported'     \
                                )
                                _thread.exit()
                        else:
                            senddata = json.dumps({
                                "id"    : request_id,
                                "reply" : {
                                   "status" : "connection failed"
                                }
                            })
                            self.core.send(encode(senddata))
                            self.core.write_log( \
                                    'Connection does not specify protocol'
                                    ,gurgle.GURGLE_LOG_MODE_ERROR)
                            self.core.emergency_quit(                    \
                                    'ProtocolError',                \
                                    'Connection does not specify protocol'\
                            )
                            _thread.exit()
                        if 'version' in data['params']:
                            if str(data['params']['version']) !=self.core.get_version():
                                senddata = json.dumps({
                                    "id"    : request_id,
                                    "reply" : {
                                        "status" : "connection failed"
                                    }
                                })
                                self.core.send(encode(senddata))
                                self.core.write_log( \
                                        "Protocol's version[%s] does not match"
                                            %str(data['params']['version'])
                                        ,gurgle.GURGLE_LOG_MODE_ERROR)
                                self.core.emergency_quit(                    \
                                        'ProtocolError',                \
                                        "Protocol's version[%s] does not match"
                                            %str(data['params']['version'])
                                )
                                _thread.exit()
                        else:
                            senddata = json.dumps({
                                "id"    : request_id,
                                "reply" : {
                                    "status" : "connection failed"
                                }
                            })
                            self.core.send(encode(senddata))
                            self.core.write_log( \
                                    'Connection does not specify protocol'
                                    ,gurgle.GURGLE_LOG_MODE_ERROR)
                            self.core.emergency_quit(                    \
                                    'ProtocolError',                \
                                    'Connection does not specify protocol'\
                            )
                            _thread.exit()
                        senddata = json.dumps({
                                    "id"    : request_id,
                                    "reply" : {
                                        "status" : "connection establised"
                                    }
                                })
                elif cmd == 'query':              # Query
                    if not data['params']:
                        self.core.write_log('Query without params'
                                ,gurgle.GURGLE_LOG_MODE_ERROR)
                        self.core.emergency_quit(
                                'SyntaxError',
                                'Query without params',
                                request_id
                            )
                        _thread.exit()
                    if 'target' in data['params']:
                        target = str(data['params']['target'])
                        if target == 'auth_method':
                            senddata = json.dumps({
                                    "id"    : request_id,
                                    "params":{
                                        "auth_method":self.core.get_auth_method()
                                    }
                                })
                        elif target == 'auth_status':
                            senddata = json.dumps({
                                    "id"    : request_id,
                                    "params": {
                                        "auth_status" : self.is_authenticated
                                    }
                                })
                        elif target == 'version':
                            senddata = json.dumps({
                                    "id"    : request_id,
                                    "params": {
                                        "version" : self.core.get_version()
                                    }
                            })
                        else:   #end if of [query]
                            self.core.emergency_quit(    \
                                    'UnknownTarget',
                                    "Target[%s] isn't supported"
                                        %target,
                                     request_id
                                )
                            self.core.write_log("Querying target[%s] isn't supported"
                                    %target
                                    ,gurgle.GURGLE_LOG_MODE_ERROR)
                            _thread.exit()
                    else:   #end if of [params]
                        self.core.write_log("Such params[%s] isn't supported"
                                    %data['params']
                                ,gurgle.GURGLE_LOG_MODE_ERROR)
                        self.core.emergency_quit(    \
                                'UnknownParams',
                                "Params[%s] isn't supported"
                                    %data['params'],
                                request_id
                            )
                        _thread.exit()
                elif cmd == 'auth':
                    if not 'from' in data:
                        self.core.write_log("Auth without from",
                                gurgle.GURGLE_LOG_MODE_ERROR)
                        self.core.emergency_quit(    \
                                'SyntaxError',
                                'Auth without the from field',
                                request_id
                            )
                        _thread.exit()
                    FullSignInID    = data['from']
                    if FullSignInID.find(':') == -1:
                        self.core.emergency_quit(
                                'SyntaxError',
                                'ID syntax error',
                                request_id
                            )
                        _thread.exit()
                    (protocol,ID)   = FullSignInID.split(':',1)
                    isProtocolSupported = False
                    if protocol == 'grgl':
                        if ID.find('@') == -1:
                            self.core.emergency_quit(    \
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
                                self.core.emergency_quit(    \
                                        'SyntaxError',
                                        'ID syntax error',
                                        request_id
                                    )
                                _thread.exit()
                        if suffix.find('/') == -1:
                            self.core.emergency_quit(    \
                                    'SyntaxError',
                                    'ID syntax error',
                                    request_id
                                )
                            _thread.exit()
                        (domain,terminal) = suffix.split('/',1)
                        if 'params' not in data:
                            self.core.write_log("Auth without params",
                                    gurgle.GURGLE_LOG_MODE_ERROR)
                            self.core.emergency_quit(    \
                                    'SyntaxError',
                                    'Auth without the params field',
                                    request_id
                                )
                            _thread.exit()
                        if 'method' not in data['params']:
                            method = self.core.get_auth_method()
                        if 'password' not in data['params']:
                            self.core.write_log("Auth without password"
                                    ,gurgle.GURGLE_LOG_MODE_ERROR)
                            self.core.emergency_quit(    \
                                    'SyntaxError',
                                    'Auth without the password field',
                                    request_id
                                )
                            _thread.exit()
                        password = data['params']['password']
                        if (password == None) or (username == None):
                            self.core.emergency_quit(    \
                                    'SyntaxError',
                                    'ID syntax error',
                                    request_id
                                )
                            _thread.exit()
                        result = self.grgl_mysql.plain_password_authenticate(
                            username,password)
                        if result == grgl_mysql_controllor.AUTH_SUCCESS:
                            senddata = json.dumps({
                                    "id"    : request_id,
                                    "to"    : FullSignInID,
                                    "error" : None
                                })
                            self.is_authenticated = 'Authenticated'
                        elif result == self.grgl_mysql_controllor.AUTH_INCORRECT:
                            senddata = json.dumps({
                                    "id"    : request_id,
                                    "to"    : FullSignInID,
                                    "error" :"%s"
                                        %"Username or password is incorrect"
                                })
                            self.is_authenticated = 'Unauthenticated'
                        else:
                            senddata = json.dumps({
                                    "id"    : request_id,
                                    "to"    : FullSignInID,
                                    "error" :"%s"
                                   %"Your account has been disabled or deactivated"
                                })
                            self.is_authenticated = 'Unauthenticated'
                        if not self.core.send(encode(senddata)):
                            self.core.disconnect_from_remote()
                            _thread.exit()
                        continue
                    self.core.write_log("Protocol[%s] is not supported yet"
                            %protocol,gurgle.GURGLE_LOG_MODE_ERROR)
                    self.core.emergency_quit(    \
                            "ProtocolUnSupported",
                            "Protocol[%s] is not supported yet"
                                %protocol,
                            request_id
                        )
                    _thread.exit()
                elif cmd == 'push':
                    if self.is_authenticated   == "Unauthenticated":
                        self.core.reply_error(   \
                            request_id,
                            "PermissionDenied",
                            "Unauthenticated")
                        continue
                    if 'params' not in data:
                        self.core.reply_error(   \
                            request_id,
                            "SyntaxError",
                            "Push without params")
                        continue;
                    if 'target' not in data['params']:
                        self.core.reply_error(   \
                            request_id,
                            "SyntaxError",
                            "Please specify the target")
                        continue;
                    target = str(data['params']['target'])
                    if target == 'presence':
                        update_dict = {}
                        if 'last_name' in data['params']:
                            update_dict['last_name'] =str(data['params']['last_name'])
                        if 'first_name' in data['params']:
                            update_dict['first_name']=str(data['params']['first_name'])
                        if 'status' in data['params']:
                            if data['params']['status'] != None:
                                flag = False
                                for i in self.core.GURGLE_STATUS_SUPPORTED:
                                    if data['params']['status'] == i:
                                        flag = True
                                        break
                                if flag == True:        # if found
                                    update_dict['status'] = i
                        if 'mood' in data['params']:
                            update_dict['mood'] = str(data['params']['mood'])
                        if update_dict == None:
                            self.core.reply_error(   \
                                request_id,
                                "SyntaxError",
                                "Please specify what you want to modify")
                            continue;
                        try:
                            self.grgl_mysql.update_user_presence(username,update_dict)
                        except grgl_mysql_controllor_error as e:
                            self.core.reply_error(   \
                                request_id,
                                "UnknownError",
                                "Unkonwn"
                            )
                            continue
                        self.core.reply_ok(request_id)
                elif cmd == 'quit':
                    if 'params' in data:
                        if 'reason' in data['params']:
                            self.core.write_log('Client quited because %s'
                                    %str(data['params']['reason']),
                                    gurgle.GURGLE_LOG_MODE_DEBUG)
                    else:
                        self.core.write_log('Client quited without reason',
                                gurgle.GURGLE_LOG_MODE_DEBUG)
                    senddata = json.dumps({"id":"%d"%request_id,"reply":"bye"})
                    self.core.send(encode(senddata))
                    self.core.disconnect_from_remote()
                    _thread.exit()
                else:   # end if of [cmd]
                    self.core.write_log("Such cmd[%s] isn't supported"%data['cmd']
                            ,gurgle.GURGLE_LOG_MODE_ERROR)
                    self.core.emergency_quit(    \
                            'UnknownCmd',
                            "Cmd[%s] isn't supported"%data['cmd'],
                            request_id
                        )
                    _thread.exit()
            if self.core.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
                self.core.emergency_quit()
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
        newThread = serviceThread()
        newThread.setup(clientSocket,addr)
        newThread.start()
