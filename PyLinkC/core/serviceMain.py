#!/usr/bin/python3
#!encoding='utf-8'

import sys
import _thread
sys.path.append("%s//..//"%sys.path[0])
from protocol.gurgle import *
from database.grgl_mysql import *
from codecs import decode, encode



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
        self.alias = ['127.0.0.1','localhost','192.168.1.1','117.59.15.96'];
        self.global_domain = 'localhost'
        self.grgl_mysql.set_server_alias(self.alias,self.global_domain)
    def __del__(self):
        del self.core
        del self.grgl_mysql
    def get_user_id(self):
        pass
    def grgl_forward_message(self,name,data,domain=None,terminal = None):
        if domain == None:
            domain = global_domain
        found = False
        flag = False
        for i in self.alias:
            if i == domain:
                flag = True
        if flag == True:
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
                            "reply":{
                                "auth_method":self.core.get_auth_method()
                            }
                        })
                elif obj == 'auth_status':
                    senddata = json.dumps({
                            "id"    : message_id,
                            "reply": {
                                "auth_status" : self.is_authenticated
                            }
                        })
                elif obj == 'version':
                    senddata = json.dumps({
                            "id"    : message_id,
                            "reply": {
                                "version" : self.core.get_version()
                            }
                    })
                elif obj == 'server_alias':
                    senddata = json.dumps({
                            "id"    : message_id,
                            "reply": {
                                "count" : len(self.alias),
                                "value" : self.alias
                            }
                    })
                elif obj == 'presence':
                    if 'who' not in params:
                        if self.is_authenticated != "Authenticated":
                            self.core.reply_error(message_id,"SyntaxError","Whose presence you want to query?")
                            continue
                        try:
                            (last_name,first_name,status,mood) = self.grgl_mysql.get_user_presence(self.username)
                        except grgl_mysql_controllor_error as err:
                            self.core.reply_error(message_id,'DatabaseError',err)
                            continue
                    else:
                        tmpVar = self.core.analyse_full_id(params['who'])
                        if tmpVar == None:
                            self.core.reply_error(message_id,"SyntaxError","Bad gid in params:{'who':'%s'}"%params['who'])
                            continue
                        try:
                            (last_name,first_name,status,mood) = self.grgl_mysql.get_user_presence(tmpVar[1])
                        except grgl_mysql_controllor_error as err:
                            self.core.reply_error(message_id,'DatabaseError',err)
                            continue
                    senddata = json.dumps({
                        "id"    : message_id,
                        "reply" : {
                            "last_name" : last_name,
                            "first_name": first_name,
                            "status"    : status,
                            "mood"      : mood,
                            "error"     : None
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
                        "id"    : message_id,
                        "reply" : {
                            "value" : tmpVar
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
                    if self.core.is_username_acceptable(self.username) == False:
                        self.core.reply_error(message_id,'SyntaxError','Unallowed character was founded')
                        continue
                    if params == None:
                        self.core.reply_error(message_id,'SyntaxError','Auth without the params field')
                        continue
                    if 'method' not in params:
                        method = self.core.get_auth_method()
                    if 'password' not in params:
                        self.core.reply_error(message_id,'SyntaxError','No password')
                        continue
                    if type(params['password']) != str:
                        self.core.reply_error(message_id,'SyntaxError','Bad password')
                        continue
                    password = params['password']
                    if password == '':
                        self.core.reply_error(message_id,'SyntaxError','No password')
                        continue
                    if self.core.is_password_acceptabel(password) == False:
                        self.core.reply_error(message_id,'SyntaxError','Username or password incorrect')
                        continue
                    result = self.grgl_mysql.plain_password_authenticate(self.username,password)
                    self.FullSignInID = self.core.make_up_full_id(self.username,domain,self.terminal)
                    if result == grgl_mysql_controllor.AUTH_SUCCESS:
                        try:
                            self.userid = self.grgl_mysql.get_user_order(self.username)
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
                            tmpVar = self.grgl_mysql.get_offline_message(self.FullSignInID)
                        except grgl_mysql_controllor_error as err:
                            self.core.reply_error(message_id,"DatabaseError","Cannot fetch offline message[%s]"%err)
                            continue
                        if tmpVar == None:
                            continue;
                        (id_list,data_list) = tmpVar;
                        i = 0
                        while i < len(data_list):
                            try:
                                self.core.send(encode(data_list[i]))
                            except gurgle_network_error as err:
                                self.core.emergency_quit()
                                _thread.exit()
                            i += 1
                        try:
                            (id_list,data_list) = self.grgl_mysql.get_offline_message(self.FullSignInID,message_type=None,delete=True)
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
                        update_dict['mood'] = str(params['mood'])
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
            elif cmd == 'update':
                if obj == None:
                    self.core.reply_error(message_id,"SyntaxError","Tell what you want to update use 'obj' field")
                    continue
                if params == None:
                    self.core.reply_error(message_id,"SyntaxError","Describe waht you want to update use 'params' field")
                    continue
                if type(params) != dict:
                    self.core.reply_error(message_id,"SyntaxError","Bad params")
                    continue
                if obj == 'roster':
                    if 'gid' not in params:
                        self.core.reply_error(message_id,"SyntaxError","Bad params[No gid specified]")
                        continue
                    target_user = params['gid']
                    if self.core.analyse_full_id(target_user) == None:
                        target_user = self.core.make_up_full_id(target_user,self.global_domain)
                    if target_user == None:
                        self.core.reply_error(message_id,"SyntaxError","Bad GID in params")
                        continue
                    update_dict = {};
                    if 'nickname' in params:
                        update_dict['nickname'] = str(params['nickname'])
                    if 'groups'  in params:
                        update_dict['groups']   = str(params['groups'])
                    flag = False
                    if 'sub_to' in params:
                        flag = True
                        try:
                            subscribed_status = self.grgl_mysql.is_user_subscribed(self.userid,target_user,self.alias)
                        except grgl_mysql_controllor_error as err:
                            self.core.reply_error(message_id,"DatabaseError",err)
                            continue
                        if bool(params['sub_to']) == True:
                            if subscribed_status < 1:
                                try:
                                    status = self.grgl_mysql.subscribe(self.userid,target_user,self.alias)
                                except grgl_mysql_controllor_error as err:
                                    self.core.reply_error("DatabaseError",err)
                                    continue
                                if status == False:
                                    self.core.reply_error("UnknownError","Cannot subscribe him/her")
                                    continue
                                senddata = {
                                    "id"    : self.core.create_id(),
                                    "cmd"   : "push",
                                    "obj"   : "subscribed_message",
                                    "params": {
                                        "message"           : "%s just subscribed you"%self.FullSignInID,
                                        "subscribed_status" : True
                                    }
                                }
                                if 'subscribed_message' in params:
                                    senddata['params']['message'] = params['subscribed_message']
                                tmpVar = self.core.analyse_full_id(target_user)
                                if tmpVar == None:
                                    self.core.reply_error(message_id,"UnknownError",None)
                                    continue
                                senddata = json.dumps(senddata)
                                try:
                                    status = self.grgl_forward_message(tmpVar[1],encode(senddata),tmpVar[2],tmpVar[3])
                                except gurgle_network_error as err:
                                    self.core.reply_error(message_id,"NetworkError",err)
                                    continue
                                if status == False:
                                    try:
                                        self.grgl_mysql.insert_offline_message(senddata,0,'subscribed_message',target_user)
                                    except grgl_mysql_controllor_error as err:
                                        self.core.reply_error(message_id,"DatabaseError",err)
                                        continue
                        else:
                            if subscribed_status == 1:
                                try:
                                    self.grgl_mysql.unsubscribe(self.userid,target_user)
                                except grgl_mysql_controllor_error as err:
                                    self.core.reply_error("DatabaseError",err)
                                    continue
                                    senddata = {
                                        "id"    : self.core.create_id(),
                                        "cmd"   : "push",
                                        "obj"   : "subscribed_message",
                                        "params": {
                                            "message"           : "%s just unsubscribed you"%self.FullSignInID,
                                            "subscribed_status" : False
                                        }
                                    }
                                    if 'subscribed_message' in params:
                                        senddata['params']['message'] = params['subscribed_message']
                                    tmpVar = self.core.analyse_full_id(target_user)
                                    if tmpVar == None:
                                        self.core.reply_error(message_id,"UnknownError",None)
                                        continue
                                    senddata = json.dumps(senddata)
                                    try:
                                        status = self.grgl_forward_message(tmpVar[1],encode(senddata),tmpVar[2],tmpVar[3])
                                    except gurgle_network_error as err:
                                        self.core.reply_error(message_id,"NetworkError",err)
                                        continue
                                    if status == False:
                                        try:
                                            self.grgl_mysql.insert_offline_message(senddata,0,'subscribed_message',target_user)
                                        except grgl_mysql_controllor_error as err:
                                            self.core.reply_error(message_id,"DatabaseError",err)
                                            continue
                    if update_dict == {}:
                        if flag:
                            self.core.reply_ok(message_id);
                            continue
                        self.core.reply_error(message_id,"SyntaxError","Please specify what you want to modify")
                        continue;
                    try:
                        self.grgl_mysql.update_roster_info(self.get_user_id(),target_user,update_dict)
                    except grgl_mysql_controllor_error as err:
                        self.core.reply_error(message_id,"DatabaseError",err)
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
                            status = self.grgl_mysql.insert_offline_message(senddata,message_id,userid=tmpData[1])
                        except grgl_mysql_controllor_error as err:
                            self.core.reply_error(message_id,"DatabaseError","Failed to insert offline message[%s]"%err)
                            self.core.write_log("DatabaseError[%s]"%err,gurgle.GURGLE_LOG_MODE_ERROR)
                            continue
                    if status == False:
                        self.core.reply_error(message_id,"Failed to forward message(Is user unreachable?)")
                        continue
                else:
                    self.core.reply_error(message_id,"ProtocolUnSupported","Protocol[%s] is not supported yet"%tmpData[0])
                    continue
                self.core.reply_ok(message_id);
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
                senddata = json.dumps({"id":message_id,"reply":"bye"})
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
