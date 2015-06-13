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
global_ip = '127.0.0.1'
global_domain = 'localhost'
class serviceThread(threading.Thread):
    def setup(self,_Socket, _Addr):
        self.core               = gurgle(gurgle.GURGLE_SERVER)
        self.grgl_mysql         = grgl_mysql_controllor()
        self.addr               = _Addr
        self.core.set_socket(_Socket)
        self.core.set_remote_host(self.addr[0])
        self.core.set_remote_port(self.addr[1]);
        self.is_authenticated   = "Unauthenticated"
        self.senddata           = "something to send"
        self.setName("TemporaryConnection")
        self.username = None
        self.terminal = None
        self.FullSignInID = None
    def get_user_id(self):
        pass
    def grgl_forward_message(self,name,data,domain=None,terminal = None):
        if domain == None:
            domain = global_domain
        found = False
        if domain == global_domain or domain == global_ip:
            if terminal == None:
                for i in threading.enumerate():
                    tName = i.getName()
                    if tName == "MainThread":
                        continue
                    elif tName == "TemporaryConnection":
                        continue
                    else:
                        if i.username == name:
                            try:
                                i.core.send(data)
                            except gurgle_network_error as err:
                                raise gurgle_network_error(err)
                            return True
            else:
                for i in threading.enumerate():
                    tName = i.getName()
                    if tName == "MainThread":
                        continue
                    elif tName == "TemporaryConnection":
                        continue
                    else:
                        if i.username == name and i.terminal == terminal:
                            i.core.send(data)
                            return True
            return False
    def run(self):
        message_id         = 0
        while True:
            try:
                data = self.core.recv(1024)
            except gurgle_network_error:
                _thread.exit()
            if not 'id' in data:
                self.core.write_log("Data without ID",gurgle.GURGLE_LOG_MODE_ERROR)
                self.core.emergency_quit('SyntaxError','This package has no ID!')
                _thread.exit()
            message_id = int(data['id'])
            if 'cmd' in data:
                cmd = str(data['cmd'])
                if cmd  == 'ping':              # ping
                    if 'payload' in data:
                        senddata = json.dumps({
                                "id"     : message_id,
                                "cmd"    : "pong",
                                "payload": data['payload']
                            })
                    else:
                        senddata = json.dumps({
                                "id"    : message_id, 
                                "cmd"   : "pong"
                            })
                elif cmd == 'connect':
                    if 'params' in data:
                        if 'protocol' in data['params']:
                            if str(data['params']['protocol'])!='gurgle':
                                senddata = json.dumps({
                                    "id"    : message_id,
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
                                "id"    : message_id,
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
                                    "id"    : message_id,
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
                                "id"    : message_id,
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
                                    "id"    : message_id,
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
                                message_id
                            )
                        _thread.exit()
                    if 'target' in data['params']:
                        target = str(data['params']['target'])
                        if target == 'auth_method':
                            senddata = json.dumps({
                                    "id"    : message_id,
                                    "params":{
                                        "auth_method":self.core.get_auth_method()
                                    }
                                })
                        elif target == 'auth_status':
                            senddata = json.dumps({
                                    "id"    : message_id,
                                    "params": {
                                        "auth_status" : self.is_authenticated
                                    }
                                })
                        elif target == 'version':
                            senddata = json.dumps({
                                    "id"    : message_id,
                                    "params": {
                                        "version" : self.core.get_version()
                                    }
                            })
                        elif target == 'presence':
                            if self.is_authenticated == False:
                                self.core.reply_error(message_id,"PermissionDenied","Unauthenticated")
                                continue
                            try:
                                (last_name,first_name,status,mood) = self.grgl_mysql.get_user_presence(self.username)
                            except grgl_mysql_controllor_error as err:
                                self.core.reply_error(message_id,'DatabaseError',err)
                                continue
                            senddata = json.dumps({
                                'id'    : message_id,
                                'reply' : {
                                    'last_name' : last_name,
                                    'first_name': first_name,
                                    'status'    : status,
                                    'mood'      : mood,
                                    'error'     : None
                                }    
                            })
                        elif target == 'roster':
                            if self.is_authenticated == False:
                                self.core.reply_error(message_id,"PermissionDenied","Unauthenticated")
                                continue
                            if 'limit' in data['params']:
                                limit = int(data['params']['limit'])
                            try:
                                tmpVar = self.grgl_mysql.get_roster(self.username,limit)
                            except grgl_mysql_controllor_error as err:
                                self.core.reply_error(message_id,"DatabaseError",err)
                                continue
                            senddata = json.dumps({
                                'id'    : message_id,
                                'reply' : {
                                    'count' : len(tmpVar),
                                    'value' : tmpVar
                                }
                             })
                        else:   #end if of [query]
                            self.core.emergency_quit(    \
                                    'UnknownTarget',
                                    "Target[%s] isn't supported"
                                        %target,
                                     message_id
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
                                message_id
                            )
                        _thread.exit()
                elif cmd == 'auth':
                    if not 'from' in data:
                        self.core.write_log("Auth without from",
                                gurgle.GURGLE_LOG_MODE_ERROR)
                        self.core.reply_error(message_id,'SyntaxError','No from field')
                        continue
                    self.FullSignInID    = data['from']
                    if self.FullSignInID.find(':') == -1:
                        self.core.reply_error(message_id,'SyntaxError',"No ':' found in ID")
                        continue
                    (protocol,ID)   = self.FullSignInID.split(':',1)
                    if protocol == 'grgl':
                        tmp_data = self.core.analyse_full_id(self.FullSignInID)
                        if str(tmp_data) == 'SyntaxError':
                            self.core.reply_error(message_id,'SyntaxError','ID cannot be analysed')
                            continue
                        (protocol,self.username,domain,self.terminal) = tmp_data
                        if self.terminal == None:
                            self.terminal = self.core.create_terminal_id()
                        for ch in self.username:
                            isFound = False
                            for a in usernameAllowed:
                                if a == ch:
                                    isFound = True
                                    break
                            if isFound == False:
                                self.core.reply_error(message_id,'SyntaxError','Unallowed character founded')
                                continue
                        if 'params' not in data:
                            self.core.reply_error(message_id,'SyntaxError','Auth without the params field')
                            continue
                        if 'method' not in data['params']:
                            method = self.core.get_auth_method()
                        if 'password' not in data['params']:
                            self.core.reply_error(message_id,'SyntaxError','No password')
                            continue
                        password = data['params']['password']
                        if password == None:
                            self.core.reply_error(message_id,'SyntaxError','No password')
                            continue
                        for ch in password:
                            isFound = False
                            for a in passwordAllowed:
                                if a == ch:
                                    isFound = True
                                    break
                            if isFound == False:
                                self.core.reply_error(message_id,'SyntaxError','Username or password is incorrect')
                                continue
                        result = self.grgl_mysql.plain_password_authenticate(self.username,password)
                        self.FullSignInID = self.core.make_up_full_id(self.username,domain,self.terminal)
                        if result == grgl_mysql_controllor.AUTH_SUCCESS:
                            senddata = json.dumps({
                                    "id"    : message_id,
                                    "to"    : self.FullSignInID,
                                    "reply" : {
                                        "error" : None
                                    }
                                })
                            self.is_authenticated = 'Authenticated'
                            self.setName(self.FullSignInID)
                        elif result == grgl_mysql_controllor.AUTH_INCORRECT:
                            self.is_authenticated = 'Unauthenticated'
                            self.core.reply_error(message_id,'AuthFailed','Username or password is incorrect')
                            continue
                        else:
                            self.core.reply_error(message_id,'AuthFailed','Your account has been disabled or deactivated')
                            self.is_authenticated = 'Unauthenticated'
                            continue
                    else:
                        self.core.write_log("Protocol[%s] is not supported yet"
                                %protocol,gurgle.GURGLE_LOG_MODE_ERROR)
                        self.core.emergency_quit(    \
                                "ProtocolUnSupported",
                                "Protocol[%s] is not supported yet"
                                    %protocol,
                                message_id
                            )
                        self.core.reply_error(message_id,'ProtocolUnSupported',"Protocol[%s] is not supported yet"%protocol)
                        self.core.emergency_quit(None,None,0)
                        _thread.exit()
                elif cmd == 'push':
                    if self.is_authenticated   == "Unauthenticated":
                        self.core.reply_error(message_id, "PermissionDenied","Unauthenticated")
                        continue
                    if 'params' not in data:
                        self.core.reply_error(message_id,"SyntaxError","Push without params")
                        continue;
                    if 'target' not in data['params']:
                        self.core.reply_error(message_id,"SyntaxError","Please specify the target")
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
                                    if data['params']['status'].lower() == i:
                                        flag = True
                                        break
                                if flag == True:        # if found
                                    update_dict['status'] = i
                        if 'mood' in data['params']:
                            update_dict['mood'] = str(data['params']['mood']).lower()
                        if update_dict == {}:
                            self.core.reply_error(message_id,"SyntaxError","Please specify what you want to modify")
                            continue;
                        try:
                            self.grgl_mysql.update_user_presence(self.username,update_dict)
                        except grgl_mysql_controllor_error as err:
                            self.core.reply_error(message_id,"UnknownError",err)
                            continue
                        self.core.reply_ok(message_id)
                        continue
                elif cmd == 'forward':  # forward messages
                    if 'to' not in data:
                        self.core.reply_error(message_id,"SyntaxError","Forwarding without to field")
                    tmpData = self.core.analyse_full_id(data['to'])
                    if str(tmpData) == "SyntaxError":
                        self.core.reply_error(message_id,"SyntaxError","Address cannnot be analysed")
                    if tmpData[0] == 'grgl':
                        self.grgl_forward_message(tmpData[1],tmpData[2],tmpData[3])
                    else:
                        self.core.reply_error(message_id,"ProtocolUnSupported","Protocol[%s] is not supported yet"%tmpData[0])
                    continue
                elif cmd == 'subscribe':
                    if 'params' not in data:
                        self.core.reply_error(message_id,"SyntaxError","No params")
                        continue
                    if 'to' not in data['params']:
                        self.core.reply_error(message_id,"SyntaxError","Tell me who you want to subscribe(use 'to' field)")
                        continue
                    tmpVar = self.core.analyse_full_id(data['params']['to'])
                    if tmpVar == 'SyntaxError':
                        self.core.reply_error(message_id,"BadID","Bad Id syntax")
                        continue
                    if 'addition' not in data['params']:
                        addition = None
                    else:
                        addition = data['params']
                    senddata = json.dumps({
                        "id"    : self.core.create_id(),
                        "cmd"   : "push",
                        "params": {
                            "target"    : "subscribe_request",
                            "from"      : self.FullSignInID,
                            "addition"  : addition
                        }
                    })
                    try:
                        status = self.grgl_mysql.insert_offline_message(senddata,'request',tmpVar[1])
                    except grgl_mysql_controllor_error as err:
                        self.core.reply_error(message_id,"DatabaseError",err)
                        continue
                    if status == False:
                        self.core.reply_error(message_id,"ForwardError","Unable to forward to such user")
                        continue
                    try:
                        self.grgl_forward_message(tmpVar[1],encode(senddata))
                    except gurgle_network_error as err:
                        self.core.emergency_quit()
                        self.core.write_log("NetworkError : %s"%err,
                            gurgle.GURGLE_LOG_MODE_ERROR)
                        _thread.exit()
                    continue
                elif cmd == 'quit':
                    if 'params' in data:
                        if 'reason' in data['params']:
                            self.core.write_log('Client quited because %s'
                                    %str(data['params']['reason']),
                                    gurgle.GURGLE_LOG_MODE_DEBUG)
                    else:
                        self.core.write_log('Client quited without reason',
                                gurgle.GURGLE_LOG_MODE_DEBUG)
                    senddata = json.dumps({"id":"%d"%message_id,"reply":"bye"})
                    self.core.send(encode(senddata))
                    self.core.disconnect_from_remote()
                    _thread.exit()
                else:   # end if of [cmd]
                    self.core.write_log("Such cmd[%s] isn't supported"%data['cmd']
                            ,gurgle.GURGLE_LOG_MODE_ERROR)
                    self.core.emergency_quit(    \
                            'UnknownCmd',
                            "Cmd[%s] isn't supported"%data['cmd'],
                            message_id
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
        newThread = serviceThread(None,None,None,(),{})
        newThread.setup(clientSocket,addr)
        newThread.start()
