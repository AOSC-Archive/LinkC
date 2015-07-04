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
        self.session  = None
        self.FullSignInID = None
        self.userid = 0
    def __del__(self):
        del self.core
        del self.grgl_mysql
    def get_user_id(self):
        pass
    def grgl_forward_message(self,name,data,domain=None,terminal = None):
        if domain == None:
            domain = grgl_mysql_controllor.global_domain
        found = False
        if domain == grgl_mysql_controllor.global_domain or domain == grgl_mysql_controllor.global_ip:
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
        message_id  = 0
        obj         = None
        cmd         = None
        params      = {}
        while True:
            try:
                data = self.core.recv(1024)
            except gurgle_network_error:
                _thread.exit()
            if not 'id' in data:
                self.core.write_log("Data without ID",gurgle.GURGLE_LOG_MODE_ERROR)
                self.core.emergency_quit('SyntaxError','This package has no ID!')
                _thread.exit()
            if type(data['id']) != int:
                self.core.write_log("Bad ID",gurgle.GURGLE_LOG_MODE_ERROR)
                self.core.emergency_quit('SyntaxError','Bad id')
                _thread.exit()
            message_id = int(data['id'])
            if 'cmd' not in data:
                self.core.reply_error(message_id,"SyntaxError","No CMD")
                continue
            if type(data['cmd']) != str:
                self.core.reply_error(message_id,"SyntaxError","Bad cmd")
                continue
            cmd = data['cmd']
            if 'obj' not in data:
                obj = None
            else:
                if data['obj'] == None:
                    obj = None
                elif type(data['obj']) == str:
                    obj = data['obj']
                else:
                    self.core.reply_error(message_id,"SyntaxError","Bad obj")
                    continue
            if 'params' in data:
                if data['params'] == None:
                    params = None
                elif type(data['params']) != dict:
                    self.core.reply_error(message_id,"SyntaxError","Bad params")
                    continue
                else:
                    params = data['params']
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
                if obj == 'session':
                    if params == None:
                        self.core.reply_error(message_id,"SyntaxError","No params")
                        continue
                    if 'protocol' not in params:
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
                    else: # there is protocol specified in params
                        if str(params['protocol'])!='gurgle':
                            senddata = json.dumps({
                                "id"    : message_id,
                                "reply" : {
                                    "status" : "connection failed"
                                }
                            })
                            self.core.send(encode(senddata))
                            self.core.write_log('Protocol %s is not supported'
                                            %params['protocol']
                                    ,gurgle.GURGLE_LOG_MODE_ERROR)
                            self.core.emergency_quit(                    \
                                    'ProtocolError',                \
                                    'Protocol is not supported'     \
                            )
                            _thread.exit()

                        # gurgle protocol connection's start
                    if 'version' in params:
                        if str(params['version']) !=self.core.get_version():
                            senddata = json.dumps({
                                "id"    : message_id,
                                "reply" : {
                                    "status" : "connection failed"
                                }
                            })
                            self.core.send(encode(senddata))
                            self.core.write_log( \
                                    "Protocol's version[%s] does not match"
                                        %str(params['version'])
                                    ,gurgle.GURGLE_LOG_MODE_ERROR)
                            self.core.emergency_quit(                    \
                                    'ProtocolError',                \
                                    "Protocol's version[%s] does not match"
                                        %str(params['version'])
                            )
                            _thread.exit()
                    tmpSession = None
                    if 'session' in params:
                        tmpSession = str(params['session'])[0:8]
                    else:
                        tmpSession = self.core.create_terminal_id()
                    for i in threading.enumerate():
                        tName = i.getName()
                        if tName == "MainThread":
                            continue
                        if i.session == tmpSession:
                            self.core.emergency_quit("SessionConflicted","Session conflicted, plz try again")
                            _thread.exit()
                    self.session = tmpSession
                    senddata = json.dumps({
                                "id"    : message_id,
                                "reply" : {
                                    "status" : "connection establised",
                                    "session": self.session
                                }
                            })
            elif cmd == 'query':              # Query
                if obj == 'auth_method':
                    senddata = json.dumps({
                            "id"    : message_id,
                            "params":{
                                "auth_method":self.core.get_auth_method()
                            }
                        })
                elif obj == 'auth_status':
                    senddata = json.dumps({
                            "id"    : message_id,
                            "params": {
                                "auth_status" : self.is_authenticated
                            }
                        })
                elif obj == 'version':
                    senddata = json.dumps({
                            "id"    : message_id,
                            "params": {
                                "version" : self.core.get_version()
                            }
                    })
                elif obj == 'presence':
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
                elif obj == 'roster':
                    if self.is_authenticated == False:
                        self.core.reply_error(message_id,"PermissionDenied","Unauthenticated")
                        continue
                    limit = 100
                    if params != None:
                        if 'limit' in params:
                            limit = int(params['limit'])
                    try:
                        tmpVar = self.grgl_mysql.get_roster(self.username,limit = limit)
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
                            'UnknownObj',
                            "Obj[%s] isn't supported"
                                %obj,
                                message_id
                        )
                    self.core.write_log("Querying obj[%s] isn't supported"
                            %obj
                            ,gurgle.GURGLE_LOG_MODE_ERROR)
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
                    if tmp_data == None:
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
                    if params == None:
                        self.core.reply_error(message_id,'SyntaxError','Auth without the params field')
                        continue
                    if 'method' not in params:
                        method = self.core.get_auth_method()
                    if 'password' not in params:
                        self.core.reply_error(message_id,'SyntaxError','No password')
                        continue
                    password = params['password']
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
                        try:
                            self.userid = self.grgl_mysql.get_user_id(self.username)
                        except grgl_mysql_controllor_error as err:
                            self.core.reply_error(message_id,"DatabaseError","Cannot get your ID")
                            continue
                        senddata = json.dumps({
                                "id"    : message_id,
                                "to"    : self.FullSignInID,
                                "reply" : {
                                    "error" : None
                                }
                            })
                        self.is_authenticated = 'Authenticated'
                        self.setName(self.FullSignInID)
                        try:
                            self.core.send(encode(senddata))
                        except gurgle_network_error as err:
                            self.core.write_log(err,gurgle.GURGLE_LOG_MODE_ERROR)
                            self.core.emergency_quit()
                            _thread.exit()
                        try:
                            (id_list,data_list) = self.grgl_mysql.get_offline_message(self.FullSignInID)
                        except grgl_mysql_controllor_error as err:
                            self.core.reply_error(message_id,"DatabaseError","Cannot fetch offline message[%s]"%err)
                            continue
                        i = 0
                        while i < len(data_list):
                            try:
                                self.core.send(encode(data_list[i]))
                            except gurgle_network_error as err:
                                self.core.emergency_quit()
                                _thread.exit()
                            i += 1
                        try:
                            (id_list,data_list) = self.grgl_mysql.get_offline_message(self.FullSignInID,message_type='request',delete=False)
                        except grgl_mysql_controllor_error as err:
                            self.core.reply_error(message_id,"DatabaseError","Cannot fetch offline message[%s]"%err)
                            continue
                        i = 0
                        while i < len(data_list):
                            try:
                                self.core.send(encode(data_list[i]))
                            except gurgle_network_error as err:
                                self.core.emergency_quit()
                                _thread.exit()
                            i+=1
                        continue
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
                if params == None:
                    self.core.reply_error(message_id,"SyntaxError","Push without params")
                    continue;
                if obj == 'presence':
                    update_dict = {}
                    if 'last_name' in params:
                        update_dict['last_name'] =str(params['last_name'])
                    if 'first_name' in params:
                        update_dict['first_name']=str(params['first_name'])
                    if 'status' in params:
                        if params['status'] != None:
                            flag = False
                            for i in self.core.GURGLE_STATUS_SUPPORTED:
                                if params['status'].lower() == i:
                                    flag = True
                                    break
                            if flag == True:        # if found
                                update_dict['status'] = i
                    if 'mood' in params:
                        update_dict['mood'] = str(params['mood']).lower()
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
                if self.is_authenticated   == "Unauthenticated":
                    self.core.reply_error(message_id, "PermissionDenied","Unauthenticated")
                    continue
                if params == None:
                    self.core.reply_error(message_id,"SyntaxError","Use 'params' field to tell us what you want to forward")
                    continue
                if 'to' not in params:
                    self.core.reply_error(message_id,"SyntaxError","Forwarding without to field")
                tmpData = self.core.analyse_full_id(params['to'])
                if tmpData == None:
                    self.core.reply_error(message_id,"SyntaxError","Address cannnot be analysed")
                    continue
                if obj != 'message':
                    pass    # something other to send
                if 'message' not in  params:
                    self.core.reply_error(message_id,"SyntaxError","Forward without message","Please use 'message' field to tell us what you want to forward",True)
                    continue
                try:
                    senddata = json.dumps({
                        "id"    : message_id,
                        "cmd"   : "push",
                        "obj"   : "message",
                        "params": {
                            "from"  : self.FullSignInID,
                            "message":params['message']
                        }
                    })
                except (TypeError, ValueError) as err:
                    self.core.reply_error(message_id,"ValueError","Cannot forward such message")
                    continue
                if tmpData[0] == 'grgl':
                    try:
                        status = self.grgl_forward_message(tmpData[1],encode(senddata),tmpData[2],tmpData[3])
                    except gurgle_network_error as err:
                        self.core.reply_error(message_id,"NetworkError","Cannot forward message")
                        continue
                    if status == False:
                        try:
                            self.grgl_mysql.insert_offline_message(senddata,message_id,username=tmpData[1])
                        except grgl_mysql_controllor_error as err:
                            self.core.reply_error(message_id,"DatabaseError","Failed to insert offline message[%s]"%err)
                            self.core.write_log("DatabaseError[%s]"%err,gurgle.GURGLE_LOG_MODE_ERROR)
                            continue
                else:
                    self.core.reply_error(message_id,"ProtocolUnSupported","Protocol[%s] is not supported yet"%tmpData[0])
                    continue
                continue
            elif cmd == 'subscribe':
                if self.is_authenticated   == "Unauthenticated":
                    self.core.reply_error(message_id, "PermissionDenied","Unauthenticated")
                    continue
                if params == None:
                    self.core.reply_error(message_id,"SyntaxError","No params")
                    continue
                if 'to' not in params:
                    self.core.reply_error(message_id,"SyntaxError","Tell me who you want to subscribe(use 'to' field)")
                    continue
                tmpVar = self.core.analyse_full_id(params['to'])
                if tmpVar == None:
                    self.core.reply_error(message_id,"BadID","Bad Id syntax")
                    continue
                try:
                    status = self.grgl_mysql.check_subscribed_list(self.username,tmpVar[1])
                except grgl_mysql_controllor_error as err:
                    self.core.reply_error(message_id,"DatabaseError",err)
                    continue
                if status == True:
                    self.core.reply_error(message_id,"DatabaseError","You have subscribed %s"%params['to'])
                    continue
                if 'addition' not in params:
                    addition = None
                else:
                    addition = params
                try:
                    senddata = json.dumps({
                        "id"    : message_id,
                        "cmd"   : "push",
                        "obj"    : "subscribed_request",
                        "params": {
                            "from"      : self.core.make_up_full_id(self.username,grgl_mysql_controllor.global_domain),
                            "addition"  : addition
                        }
                    })
                except (TypeError, ValueError) as err:
                    self.core.reply_error(message_id,"ValueError","Cannot forward such message")
                    continue
                try:
                    status = self.grgl_mysql.insert_offline_message(senddata,message_id,'request',tmpVar[1])
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
            elif cmd == 'subscribed_reply':
                if self.is_authenticated   == "Unauthenticated":
                    self.core.reply_error(message_id, "PermissionDenied","Unauthenticated")
                    continue
                if params == None:
                    self.core.reply_error(message_id,"SyntaxError","No params")
                    continue
                if 'status' not in params:
                    self.core.reply_error(message_id,"SyntaxError","No status in params")
                    continue
                if 'addition' in params:
                    addition = params['addition']
                else:
                    addition = None
                if 'to' in params:
                    to_id = params['to']
                else:
                    addition = None
                try:
                    tmpVar = self.grgl_mysql.get_offline_message(None,self.userid,None,data['id'],True)
                except grgl_mysql_controllor_error as err:
                    self.core.reply_error(message_id,"DatabaseError",err)
                    continue
                if tmpVar == None:
                    self.core.reply_error(message_id,"DatabaseError","There's no such Request")
                    continue
                (tmpId,tmpData) = tmpVar
                try:
                    tmpData = json.loads(tmpData[0])
                except (TypeError, ValueError) as err:
                    self.core.reply_error(message_id,"ValueError","Cannot forward such message")
                    continue
                tmpId   = tmpId[0]
                if tmpData['cmd'] != 'push':
                    self.core.reply_error(message_id,"UnknowError","UnkonwError in subscribed_reply[1]")
                    continue
                if tmpData['obj'] != "subscribed_request":
                    self.core.reply_error(message_id,"UnknowError","UnkonwError in subscribed_reply[2]")
                    continue
                if 'from' not in tmpData['params']:
                    self.core.reply_error(message_id,"UnknowError","UnkonwError in subscribed_reply[3]")
                    continue
                if  tmpData['params']['from'] == None:
                    self.core.reply_error(message_id,"UnknowError","UnkonwError in subscribed_reply[4]")
                    continue
                if self.core.is_id_match(tmpData['params']['from'],to_id,grgl_mysql_controllor.global_domain,grgl_mysql_controllor.global_ip) == False:
                    self.core.reply_error(message_id,"UnknowError","UnkonwError in subscribed_reply[5]")
                    continue
                if params['status'].lower() == 'accepted':
                    try:
                        senddata = json.dumps({
                            "id"    : message_id,
                            "cmd"   : "reply",
                            "params":{
                                "status"    : "accepted",
                                "addition"  : addition
                            }
                        })
                    except (TypeError, ValueError) as err:
                        self.core.reply_error(message_id,"ValueError","Cannot forward such message")
                        continue
                elif params['status'] == 'ignore':
                    continue
                else:
                    try:
                        senddata = json.dumps({
                            "id"    : message_id,
                            "cmd"   : "reply",
                            "params":{
                                "status"    : "refused",
                                "addition"  : addition
                            }
                        })
                    except (TypeError, ValueError) as err:
                        self.core.reply_error(message_id,"ValueError","Cannot forward such message")
                        continue
                tmpVar = self.core.analyse_full_id(to_id)
                if tmpVar == None:
                    self.core.reply_error("SyntaxError","Bad id 'to'")
                    continue
                to_name = tmpVar[1]
                try:
                    status = self.grgl_forward_message(to_name,encode(senddata))
                except gurgle_network_error as err:
                    self.core.reply_error(tmpId,'ForwardError',err)
                if status == False:
                    try:
                        self.grgl_mysql.insert_offline_message(senddata,tmpId,'message',to_name)
                    except grgl_mysql_controllor_error as err:
                        self.core.reply_error(message_id,'DatabaseError','Cannot insert offline meesage')
                        continue
                if params['status'].lower() == 'accepted':
                    try:
                        self.grgl_mysql.accept_subscribed_request(self.username,to_name)
                    except grgl_mysql_controllor_error as err:
                        self.core.reply_error(message_id,'DatabaseError','Cannot insert offline meesage')
                        continue
                self.core.reply_ok(message_id)
                continue
            elif cmd == 'unsubscribe':
                if self.is_authenticated   == "Unauthenticated":
                    self.core.reply_error(message_id, "PermissionDenied","Unauthenticated")
                    continue
                if params == None:
                    self.core.reply_error(message_id,"SyntaxError","No params with unsubscribe")
                    continue
                if 'to' not in params:
                    self.core.reply_error(message_id,"SyntaxError","Use 'to' field to specify who you want to unsubscribe")
                    continue
                tmpVar = self.core.analyse_full_id(params['to'])
                if tmpVar == None:
                    self.core.reply_error(message_id,"SyntaxError","Bad id in 'to'")
                    continue
            elif cmd == 'quit':
                if 'params' in data:
                    if 'reason' in params:
                        self.core.write_log('Client quited because %s'
                                %str(params['reason']),
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
